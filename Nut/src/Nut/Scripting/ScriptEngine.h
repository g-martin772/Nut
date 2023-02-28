#pragma once

#include "Nut/Scene/Scene.h"

#include <map>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Nut {

	enum class ScriptFieldType {
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptField {
		ScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};

	class ScriptClass;
	class ScriptObject;
	class ScriptEntity;

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
		static Ref<ScriptObject> GetEntityScriptInstance(UUID entityID);

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
		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::string m_Namespace, m_Name;
		MonoClass* m_Class;

		std::map<std::string, ScriptField> m_Fields;

		friend class ScriptEngine;
	};



	class ScriptObject {
	public:
		ScriptObject(const Ref<ScriptClass>& klass);
		ScriptObject(const Ref<ScriptClass>& klass, int paramCount, void** params);
		virtual ~ScriptObject();

		inline const Ref<ScriptClass>& GetClass() const { return m_Class; }

		void Invoke(std::string methodName, uint32_t parameterCount, void** params) const;

		Ref<ScriptClass> GetScriptClass() { return m_Class; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	protected:
		ScriptObject();
		const Ref<ScriptClass>& m_Class;
		MonoObject* m_Instance;

		inline static char s_FieldValueBuffer[8];
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