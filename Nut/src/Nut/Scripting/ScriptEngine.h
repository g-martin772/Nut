#pragma once

#include "Nut/Scene/Scene.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
}

namespace Nut {
	class ScriptClass;

	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeUpdate(float ts);
		static void OnRuntimeStop();

		static bool ScriptEngine::EntityClassExists(const std::string& fullClassName);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static Scene* GetCurrentScene();
		static MonoImage* GetCoreAssemblyImage();

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);
		static void OnDestroyEntity(Entity entity);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoAssembly* LoadMonoAssembly(const std::string& assemblyPath);
		static void LoadEntityClasses(MonoAssembly* assembly);

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
		ScriptObject(const Ref<ScriptClass>& klass);
		ScriptObject(const Ref<ScriptClass>& klass, int paramCount, void** params);
		virtual ~ScriptObject();

		inline const Ref<ScriptClass>& GetClass() const { return m_Class; }

		void Invoke(std::string methodName, uint32_t parameterCount, void** params) const;
	protected:
		ScriptObject();
		const Ref<ScriptClass>& m_Class;
		MonoObject* m_Instance;
	};



	class ScriptEntity : public ScriptObject {
	public:
		ScriptEntity(const Ref<ScriptClass>& klass);
		ScriptEntity(const Ref<ScriptClass>& klass, int paramCount, void** params);
		~ScriptEntity();

		void OnCreate();
		void OnDestroy();
		void OnUpdate(float ts);
	private:
		MonoMethod* m_OnCreate;
		MonoMethod* m_OnDestroy;
		MonoMethod* m_OnUpdate;
	};
}