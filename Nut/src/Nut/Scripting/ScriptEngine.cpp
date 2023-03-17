#include "ntpch.h"
#include "ScriptEngine.h"

#include "ScriptApi.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"
#include "mono/metadata/object.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include <filewatch/FileWatch.h>

#include "Nut/Scene/Entity.h"
#include "Nut/Core/Application.h"
#include "Nut/Core/Timer.h"
#include "Nut/Core/FileSystem.h"
#include "../Project/Project.h"


namespace Nut {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },

		{ "Hazel.Vector2", ScriptFieldType::Vector2 },
		{ "Hazel.Vector3", ScriptFieldType::Vector3 },
		{ "Hazel.Vector4", ScriptFieldType::Vector4 },

		{ "Hazel.Entity", ScriptFieldType::Entity },
	};

	ScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
	{
		std::string typeName = mono_type_get_name(monoType);

		auto it = s_ScriptFieldTypeMap.find(typeName);
		if (it == s_ScriptFieldTypeMap.end())
		{
			NT_CORE_ERROR("Unknown type: {}", typeName);
			return ScriptFieldType::None;
		}

		return it->second;
	}
	
	struct ScriptingEngineData {
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* GameAssembly = nullptr;
		MonoImage* GameAssemblyImage = nullptr;

		Scene* SceneContext = nullptr;

		Ref<ScriptClass> EntityCLass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptEntity>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityScriptFields;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;

		bool EnableDebugging = true;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;
	};

	static ScriptingEngineData* s_Data;

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event changeType)
	{
		if (!s_Data->AssemblyReloadPending && changeType == filewatch::Event::modified)
		{
			s_Data->AssemblyReloadPending = true;

			Application::Get().SubmitToMainThread([]() {
				s_Data->AppAssemblyFileWatcher.reset();
				ScriptEngine::ReloadAssembly();
				});
		}
	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptingEngineData;
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeUpdate(float ts)
	{
		
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->EntityInstances.clear();
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Creating a root domain
		if (!s_Data->RootDomain) {
			if (s_Data->EnableDebugging)
			{
				const char* argv[2] = {
					"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
					"--soft-breakpoints"
				};

				mono_jit_parse_options(2, const_cast<char**>(argv));
				mono_debug_init(MONO_DEBUG_FORMAT_MONO);
			}

			MonoDomain* rootDomain = mono_jit_init("NutScriptRuntime");
			NT_CORE_ASSERT(rootDomain, "Initializing mono-jit failed");

			// Store the root domain pointer
			s_Data->RootDomain = rootDomain;

			if (s_Data->EnableDebugging)
				mono_debug_domain_create(s_Data->RootDomain);

			mono_thread_set_main(mono_thread_current());
		}



		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("NutAppDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssemblyFilepath = "Assets/Scripts/Nut-ScriptCore/Nut-ScriptCore.dll";

		s_Data->CoreAssembly = LoadMonoAssembly(s_Data->CoreAssemblyFilepath.string(), s_Data->EnableDebugging);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		s_Data->EntityCLass = std::make_shared<ScriptClass>("Nut.Scene", "Entity");
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	std::unordered_map<std::string, Nut::Ref<Nut::ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	Nut::Scene* ScriptEngine::GetCurrentScene()
	{
		return s_Data->SceneContext;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (EntityClassExists(sc.Name)) 
		{
			void* id = &entity.GetComponent<IDComponent>().ID;
			const Ref<ScriptEntity> script = std::make_shared<ScriptEntity>(s_Data->EntityClasses[sc.Name], 1, &id);
			s_Data->EntityInstances[*static_cast<UUID*>(id)] = script;

			if (s_Data->EntityScriptFields.find(*static_cast<UUID*>(id)) != s_Data->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(*static_cast<UUID*>(id));
				for (const auto& [name, fieldInstance] : fieldMap)
					script->SetFieldValueInternal(name, fieldInstance.m_Buffer);
			}

			script->OnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, const Timestep ts)
	{
		UUID id = entity.GetComponent<IDComponent>().ID;
		NT_CORE_ASSERT(s_Data->EntityInstances.find(id) != s_Data->EntityInstances.end(), "Script not found");

		const Ref<ScriptEntity> script = s_Data->EntityInstances[id];
		script->OnUpdate(ts);
	}

	void ScriptEngine::OnDestroyEntity(Entity entity)
	{
		const UUID id = entity.GetComponent<IDComponent>().ID;
		NT_CORE_ASSERT(s_Data->EntityInstances.find(id) != s_Data->EntityInstances.end(), "Script not found");

		const Ref<ScriptEntity> script = s_Data->EntityInstances[id];
		script->OnDestroy();
	}

	void ScriptEngine::ReloadAssembly()
	{
		mono_assembly_close(s_Data->GameAssembly);
		mono_image_close(s_Data->GameAssemblyImage);

		s_Data->AppAssemblyFilepath = Project::GetProjectDirectory() / "bin/Sandbox/Sandbox.dll";
		auto path = s_Data->AppAssemblyFilepath;
		s_Data->GameAssembly = LoadMonoAssembly(s_Data->AppAssemblyFilepath.string(), s_Data->EnableDebugging);
		s_Data->GameAssemblyImage = mono_assembly_get_image(s_Data->GameAssembly);

		s_Data->AppAssemblyFileWatcher = std::make_unique<filewatch::FileWatch<std::string>>(s_Data->AppAssemblyFilepath.string(), OnAppAssemblyFileSystemEvent);
		s_Data->AssemblyReloadPending = false;

		ScriptApi::RegisterComponents();
		ScriptApi::RegisterInternalCalls();

		s_Data->EntityClasses.clear();
		LoadEntityClasses(s_Data->GameAssembly);
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(name);
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		NT_CORE_ASSERT(entity, "Entity was null");

		const UUID entityId = entity.GetComponent<IDComponent>().ID;
		return s_Data->EntityScriptFields[entityId];
	}

	MonoObject* ScriptEngine::GetManagedInstance(const UUID uuid)
	{
		if(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end())
			return s_Data->EntityInstances.at(uuid)->GetManagedObject();
		return nullptr;
	}

	MonoAssembly* ScriptEngine::LoadMonoAssembly(const std::filesystem::path& assemblyPath, const bool loadPdb)
	{
		ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			NT_CORE_ERROR(mono_image_strerror(status));
			return nullptr;
		}

		if (loadPdb)
		{
			std::filesystem::path pdbPath = assemblyPath;
			pdbPath.replace_extension(".pdb");

			if (std::filesystem::exists(pdbPath))
			{
				ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(pdbPath.string());
				mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.Size());
				NT_CORE_INFO("Loaded PDB {}", pdbPath);
			}
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, reinterpret_cast<const char*>(assemblyPath.c_str()), &status, 0);
		mono_image_close(image);

		return assembly;
	}

	void ScriptEngine::LoadEntityClasses(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		const int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		const ScriptClass entityClass("Nut.Scene", "Entity");

		NT_CORE_INFO("Script Classes found and loaded:");
		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName = fmt::format("{}.{}", nameSpace, className);

			if (ScriptClass klass(nameSpace, className);
				mono_class_is_subclass_of(klass.GetClass(), entityClass.GetClass(), false) && std::string(className) != "Entity") {
				s_Data->EntityClasses[fullName] = std::make_shared<ScriptClass>(nameSpace, className);
			}
			else continue;

			int fieldCount = mono_class_num_fields(s_Data->EntityClasses[fullName]->GetClass());
			NT_CORE_WARN("{} has {} fields:", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(s_Data->EntityClasses[fullName]->GetClass(), &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				if (const uint32_t flags = mono_field_get_flags(field); flags & 0x0006 /*FIELD_ATTRIBUTE_PUBLIC*/)
				{
					MonoType* type = mono_field_get_type(field);
					const ScriptFieldType fieldType = MonoTypeToScriptFieldType(type);
					NT_CORE_WARN("  {} ({})", fullName, Utils::ScriptFieldTypeToString(fieldType));

					s_Data->EntityClasses[fullName]->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}

		}
	}

	MonoClass* ScriptEngine::GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName, className);

		NT_CORE_ASSERT(klass, "Failed to get MonoClass by name");

		return klass;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoAssembly* assembly, const char* namespaceName, const char* className)
	{
		// Get a reference to the class we want to instantiate
		MonoClass* testingClass = GetClassInAssembly(assembly, "Nut", "Main");

		// Allocate an instance of our class
		MonoObject* classInstance = mono_object_new(s_Data->AppDomain, testingClass);

		NT_CORE_ASSERT(classInstance, "Failed to instanciate class");

		// Call the parameterless (default) constructor
		mono_runtime_object_init(classInstance);
		return classInstance;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	Ref<ScriptObject> ScriptEngine::GetEntityScriptInstance(UUID entityId)
	{
		auto it = s_Data->EntityInstances.find(entityId);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	ScriptClass::ScriptClass(const std::string& namespaceName, const std::string& className)
		: m_Namespace(namespaceName), m_Name(className)
	{
		MonoClass* klass = mono_class_from_name(s_Data->CoreAssemblyImage, namespaceName.c_str(), className.c_str());
		
		if(!klass)
			klass = mono_class_from_name(s_Data->GameAssemblyImage, namespaceName.c_str(), className.c_str());
		
		NT_CORE_ASSERT(klass, "Failed to load class");
		m_Class = klass;
	}

	ScriptClass::~ScriptClass()
	{
		//mono_free(m_Class);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, const uint32_t parameterCount) const
	{
		MonoMethod* method = mono_class_get_method_from_name(m_Class, methodName.c_str(), parameterCount);
		NT_CORE_ASSERT(method, "Failed to load method");
		return method;
	}
	
	ScriptObject::ScriptObject(const Ref<ScriptClass>& klass)
		: m_Class(klass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, m_Class->GetClass());
		mono_runtime_object_init(instance);
		m_Instance = instance;
	}

	ScriptObject::ScriptObject(const Ref<ScriptClass>& klass, int paramCount, void** params)
		: m_Class(klass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, m_Class->GetClass());
		m_Instance = instance;
		MonoObject* exc;
		mono_runtime_invoke(s_Data->EntityCLass->GetMethod(".ctor", 1), m_Instance, params, &exc);
	}

	bool ScriptObject::GetFieldValueInternal(const std::string& name, void* buffer) const
	{
		const auto& fields = m_Class->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptObject::SetFieldValueInternal(const std::string& name, const void* value) const
	{
		const auto& fields = m_Class->GetFields();
		const auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const auto& [Type, Name, ClassField] = it->second;
		mono_field_set_value(m_Instance, ClassField, const_cast<void*>(value));
		return true;
	}

	void ScriptObject::Invoke(const std::string& methodName, const uint32_t parameterCount, void** params) const
	{
		MonoMethod* method = m_Class->GetMethod(methodName, parameterCount);
		MonoObject* exc;
		mono_runtime_invoke(method, m_Instance, params, &exc);
		// TODO: Handle exception
	}

	ScriptObject::~ScriptObject()
	{
		//mono_free(m_Instance);
	}


	ScriptEntity::ScriptEntity(const Ref<ScriptClass>& klass)
		: ScriptObject(klass)
	{
		m_OnCreate = s_Data->EntityCLass->GetMethod("OnCreate", 0);
		m_OnDestroy = s_Data->EntityCLass->GetMethod("OnDestroy", 0);
		m_OnUpdate = s_Data->EntityCLass->GetMethod("OnUpdate", 1);

		m_OnCreate = mono_object_get_virtual_method(m_Instance, m_OnCreate);
		m_OnDestroy = mono_object_get_virtual_method(m_Instance, m_OnDestroy);
		m_OnUpdate = mono_object_get_virtual_method(m_Instance, m_OnUpdate);
	}

	ScriptEntity::ScriptEntity(const Ref<ScriptClass>& klass, const int paramCount, void** params)
		: ScriptObject(klass, paramCount, params)
	{
		m_OnCreate = s_Data->EntityCLass->GetMethod("OnCreate", 0);
		m_OnDestroy = s_Data->EntityCLass->GetMethod("OnDestroy", 0);
		m_OnUpdate = s_Data->EntityCLass->GetMethod("OnUpdate", 1);

		m_OnCreate = mono_object_get_virtual_method(m_Instance, m_OnCreate);
		m_OnDestroy = mono_object_get_virtual_method(m_Instance, m_OnDestroy);
		m_OnUpdate = mono_object_get_virtual_method(m_Instance, m_OnUpdate);
	}

	ScriptEntity::~ScriptEntity()
	{

	}

	void ScriptEntity::OnCreate() const
	{
		MonoObject* exc;
		mono_runtime_invoke(m_OnCreate, m_Instance, nullptr, &exc);
	}

	void ScriptEntity::OnDestroy() const
	{
		MonoObject* exc;
		mono_runtime_invoke(m_OnDestroy, m_Instance, nullptr, &exc);
	}

	void ScriptEntity::OnUpdate(float ts) const
	{
		void* timestep = &ts;
		MonoObject* exc;
		mono_runtime_invoke(m_OnUpdate, m_Instance, &timestep, &exc);
	}

}
