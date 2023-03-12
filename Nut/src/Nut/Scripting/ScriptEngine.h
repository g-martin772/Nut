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

	struct ScriptFieldInstance {
		ScriptField Field;

		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
		friend class ScriptInstance;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

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
		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);
		static MonoObject* GetManagedInstance(UUID uuid);

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);
		static void OnDestroyEntity(Entity entity);

		static void ReloadAssembly();
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
			static_assert(sizeof(T) <= 16, "Type too large!");
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			SetFieldValueInternal(name, &value);
		}

		MonoObject* GetManagedObject() { return m_Instance; }

	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	protected:
		ScriptObject();
		const Ref<ScriptClass>& m_Class;
		MonoObject* m_Instance;

		inline static char s_FieldValueBuffer[8];

		friend class ScriptEngine;
		friend struct ScriptFieldInstance;
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






	namespace Utils {

		inline const char* ScriptFieldTypeToString(ScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case ScriptFieldType::None:    return "None";
			case ScriptFieldType::Float:   return "Float";
			case ScriptFieldType::Double:  return "Double";
			case ScriptFieldType::Bool:    return "Bool";
			case ScriptFieldType::Char:    return "Char";
			case ScriptFieldType::Byte:    return "Byte";
			case ScriptFieldType::Short:   return "Short";
			case ScriptFieldType::Int:     return "Int";
			case ScriptFieldType::Long:    return "Long";
			case ScriptFieldType::UByte:   return "UByte";
			case ScriptFieldType::UShort:  return "UShort";
			case ScriptFieldType::UInt:    return "UInt";
			case ScriptFieldType::ULong:   return "ULong";
			case ScriptFieldType::Vector2: return "Vector2";
			case ScriptFieldType::Vector3: return "Vector3";
			case ScriptFieldType::Vector4: return "Vector4";
			case ScriptFieldType::Entity:  return "Entity";
			}
			NT_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")    return ScriptFieldType::None;
			if (fieldType == "Float")   return ScriptFieldType::Float;
			if (fieldType == "Double")  return ScriptFieldType::Double;
			if (fieldType == "Bool")    return ScriptFieldType::Bool;
			if (fieldType == "Char")    return ScriptFieldType::Char;
			if (fieldType == "Byte")    return ScriptFieldType::Byte;
			if (fieldType == "Short")   return ScriptFieldType::Short;
			if (fieldType == "Int")     return ScriptFieldType::Int;
			if (fieldType == "Long")    return ScriptFieldType::Long;
			if (fieldType == "UByte")   return ScriptFieldType::UByte;
			if (fieldType == "UShort")  return ScriptFieldType::UShort;
			if (fieldType == "UInt")    return ScriptFieldType::UInt;
			if (fieldType == "ULong")   return ScriptFieldType::ULong;
			if (fieldType == "Vector2") return ScriptFieldType::Vector2;
			if (fieldType == "Vector3") return ScriptFieldType::Vector3;
			if (fieldType == "Vector4") return ScriptFieldType::Vector4;
			if (fieldType == "Entity")  return ScriptFieldType::Entity;

			NT_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}

	}
}