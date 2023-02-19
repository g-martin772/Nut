#pragma once

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace Nut {
	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();
	private:
		static void InitMono();
		static void ShutdownMono();
	
		static MonoAssembly* LoadMonoAssembly(const std::string& assemblyPath);
		static void PrintAssemblyTypes(MonoAssembly* assembly);

		static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
		static MonoObject* InstantiateClass(MonoAssembly* assembly, const char* namespaceName, const char* className);
	};

	class ScriptClass {
	public:
		ScriptClass(std::string namespaceName, std::string className);
		~ScriptClass();

		inline MonoClass* GetClass() const { return m_Class; }
		inline std::string GetName() const { return m_Name; }

		MonoMethod* GetMethod(std::string methodName, uint32_t parameterCount) const;
	private:
		std::string m_Namespace, m_Name;
		MonoClass* m_Class;
	};

	class ScriptObject {
	public:
		ScriptObject(const ScriptClass& klass);
		~ScriptObject();

		inline const ScriptClass& GetClass() const { return m_Class; }

		void Invoke(std::string methodName, uint32_t parameterCount, void** params) const;
	private:
		const ScriptClass& m_Class;
		MonoObject* m_Instance;
	};
}