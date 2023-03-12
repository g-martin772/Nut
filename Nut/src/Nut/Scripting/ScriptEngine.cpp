#include "ntpch.h"
#include "ScriptEngine.h"

#include "ScriptApi.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"
#include "mono/metadata/object.h"
#include "../Scene/Entity.h"

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
	
	struct ScrptingEngineData {
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
	};

	static ScrptingEngineData* s_Data;

	void ScriptEngine::Init()
	{
		s_Data = new ScrptingEngineData;
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
			MonoDomain* rootDomain = mono_jit_init("NutScriptRuntime");
			NT_CORE_ASSERT(rootDomain, "Initializing mono-jit failed");

			// Store the root domain pointer
			s_Data->RootDomain = rootDomain;
		}

		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("NutAppDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadMonoAssembly("Assets/Scripts/Nut-ScriptCore/Nut-ScriptCore.dll");
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		s_Data->GameAssembly = LoadMonoAssembly("resources/SandboxProject/bin/Sandbox/Sandbox.dll");
		s_Data->GameAssemblyImage = mono_assembly_get_image(s_Data->GameAssembly);
		
		ScriptApi::RegisterComponents();
		ScriptApi::RegisterInternalCalls();

		s_Data->EntityClasses.clear();
		LoadEntityClasses(s_Data->GameAssembly);

		s_Data->EntityCLass = std::make_shared<ScriptClass>("Nut.Scene", "Entity");
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		/*mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;*/
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
			Ref<ScriptEntity> script = std::make_shared<ScriptEntity>(s_Data->EntityClasses[sc.Name], 1, &id);
			s_Data->EntityInstances[*(UUID*)id] = script;

			if (s_Data->EntityScriptFields.find(*(UUID*)id) != s_Data->EntityScriptFields.end())
			{
				const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(*(UUID*)id);
				for (const auto& [name, fieldInstance] : fieldMap)
					script->SetFieldValueInternal(name, fieldInstance.m_Buffer);
			}

			script->OnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID id = entity.GetComponent<IDComponent>().ID;
		NT_CORE_ASSERT(s_Data->EntityInstances.find(id) != s_Data->EntityInstances.end(), "Script not found");
			
		Ref<ScriptEntity> script = s_Data->EntityInstances[id];
		script->OnUpdate(ts);
	}

	void ScriptEngine::OnDestroyEntity(Entity entity)
	{
		UUID id = entity.GetComponent<IDComponent>().ID;
		NT_CORE_ASSERT(s_Data->EntityInstances.find(id) != s_Data->EntityInstances.end(), "Script not found");

		Ref<ScriptEntity> script = s_Data->EntityInstances[id];
		script->OnDestroy();
	}

	void ScriptEngine::ReloadAssembly()
	{
		/*mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		s_Data->AppDomain = mono_domain_create_appdomain("HazelScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadMonoAssembly("Assets/Scripts/Nut-ScriptCore/Nut-ScriptCore.dll");
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		s_Data->GameAssembly = LoadMonoAssembly("resources/SandboxProject/bin/Sandbox/Sandbox.dll");
		s_Data->GameAssemblyImage = mono_assembly_get_image(s_Data->GameAssembly);

		LoadEntityClasses(s_Data->GameAssembly);

		ScriptApi::RegisterComponents();

		s_Data->EntityCLass = std::make_shared<ScriptClass>("Hazel", "Entity");*/
		ShutdownMono();
		InitMono();
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

		UUID entityID = entity.GetComponent<IDComponent>().ID;
		return s_Data->EntityScriptFields[entityID];
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid)
	{
		if(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end())
			return s_Data->EntityInstances.at(uuid)->GetManagedObject();
		return nullptr;
	}

	// TODO: Implement proper FILE API
	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		NT_CORE_ASSERT(stream, "");

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* ScriptEngine::LoadMonoAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			NT_CORE_ERROR(mono_image_strerror(status));
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	void ScriptEngine::LoadEntityClasses(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		ScriptClass entityClass("Nut.Scene", "Entity");

		NT_CORE_INFO("Script Classes found and loaded:");
		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName = fmt::format("{}.{}", nameSpace, className);

			ScriptClass klass(nameSpace, className);

			if (mono_class_is_subclass_of(klass.GetClass(), entityClass.GetClass(), false) && std::string(className) != "Entity") {
				s_Data->EntityClasses[fullName] = std::make_shared<ScriptClass>(nameSpace, className);
			}
			else continue;

			int fieldCount = mono_class_num_fields(s_Data->EntityClasses[fullName]->GetClass());
			NT_CORE_WARN("{} has {} fields:", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(s_Data->EntityClasses[fullName]->GetClass(), &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & 0x0006 /*FIELD_ATTRIBUTE_PUBLIC*/)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = MonoTypeToScriptFieldType(type);
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

	Ref<ScriptObject> ScriptEngine::GetEntityScriptInstance(UUID entityID)
	{
		auto it = s_Data->EntityInstances.find(entityID);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	ScriptClass::ScriptClass(std::string namespaceName, std::string className)
		: m_Name(className), m_Namespace(namespaceName)
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

	MonoMethod* ScriptClass::GetMethod(std::string methodName, uint32_t parameterCount) const
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

	bool ScriptObject::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_Class->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptObject::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_Class->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}

	void ScriptObject::Invoke(std::string methodName, uint32_t parameterCount, void** params) const
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

	ScriptEntity::ScriptEntity(const Ref<ScriptClass>& klass, int paramCount, void** params)
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

	void ScriptEntity::OnCreate()
	{
		mono_runtime_invoke(m_OnCreate, m_Instance, nullptr, nullptr);
	}

	void ScriptEntity::OnDestroy()
	{
		mono_runtime_invoke(m_OnDestroy, m_Instance, nullptr, nullptr);
	}

	void ScriptEntity::OnUpdate(float ts)
	{
		void* timestep = &ts;
		mono_runtime_invoke(m_OnUpdate, m_Instance, &timestep, nullptr);
	}

}
