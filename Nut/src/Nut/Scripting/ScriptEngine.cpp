#include "ntpch.h"
#include "ScriptEngine.h"

#include "ScriptApi.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"
#include "mono/metadata/object.h"

namespace Nut {
	

	struct ScrptingEngineData {
		MonoDomain* RootDomain;
		MonoDomain* AppDomain;

		MonoAssembly* CoreAssembly;
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

	void Print(int value)
	{
		std::cout << "C++ prints the value " << value << std::endl;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		// Creating a root domain
		MonoDomain* rootDomain = mono_jit_init("NutScriptRuntime");
		NT_CORE_ASSERT(rootDomain, "Initializing mono-jit failed");

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;

		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("NutAppDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		ScriptApi::RegisterInternalCalls();

		auto assambly = LoadMonoAssembly("Assets/Scripts/Nut-ScriptCore/Nut-ScriptCore.dll");
		s_Data->CoreAssembly = assambly;
		PrintAssemblyTypes(assambly);

		

		auto instance = InstantiateClass(assambly, "Nut", "Main");
		MonoMethod* method = mono_class_get_method_from_name(GetClassInAssembly(assambly, "Nut", "Main"), "PrintTest", 0);
		MonoObject* exception = nullptr;
		mono_runtime_invoke(method, instance, nullptr, &exception);


	}

	void ScriptEngine::ShutdownMono()
	{
		delete s_Data->AppDomain;
		delete s_Data->RootDomain;
	}





	// TODO: Implement proper FILE Api
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

	void ScriptEngine::PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		
		NT_CORE_INFO("Mono Assambly Types:");
		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			NT_CORE_INFO("{0}.{1}", nameSpace, name);
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

	ScriptClass::ScriptClass(std::string namespaceName, std::string className)
		: m_Name(className), m_Namespace(namespaceName)
	{
		MonoImage* image = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* klass = mono_class_from_name(image, namespaceName.c_str(), className.c_str());
		NT_CORE_ASSERT(klass, "Failed to load class");
		m_Class = klass;
	}

	ScriptClass::~ScriptClass()
	{
		mono_free(m_Class);
	}

	MonoMethod* ScriptClass::GetMethod(std::string methodName, uint32_t parameterCount) const
	{
		return mono_class_get_method_from_name(m_Class, methodName.c_str(), parameterCount);
	}
	
	ScriptObject::ScriptObject(const ScriptClass& klass)
		: m_Class(klass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, m_Class.GetClass());
		mono_runtime_object_init(instance);
		m_Instance = instance;
	}

	void ScriptObject::Invoke(std::string methodName, uint32_t parameterCount, void** params) const
	{
		MonoMethod* method = m_Class.GetMethod(methodName, parameterCount);
		MonoObject* exc;
		mono_runtime_invoke(method, m_Instance, params, &exc);
		// TODO: Handle exception
	}

	ScriptObject::~ScriptObject()
	{
		mono_free(m_Instance);
	}
}
