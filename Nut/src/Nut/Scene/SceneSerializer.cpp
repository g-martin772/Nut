#include "ntpch.h"
#include "SceneSerializer.h"

#include "yaml-cpp/yaml.h"
#include "Nut/Scene/Entity.h"
#include <Nut/Scene/Components.h>

namespace YAML {

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Nut {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		NT_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Invalid Entity");

		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << (uint64_t)entity.GetComponent<IDComponent>().ID;

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag";
			out << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scalation" << YAML::Value << tc.Scalation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>()) {
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();
			auto& c = cc.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)c.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << c.GetFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << c.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << c.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << c.GetOrthographicsSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << c.GetOrthoNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << c.GetOrthoFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cc.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cc.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& sc = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << (glm::vec4)sc.Color;

			if (sc.Texture.get()) {
				out << YAML::Key << "Texture" << YAML::Value << sc.Texture.get()->GetPath();
			}
			else {
				out << YAML::Key << "Texture" << YAML::Value << "none";
			}
			
			out << YAML::Key << "TilingFactor" << YAML::Value << sc.TilingFactor;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleRendererComponent>()) {
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CircleRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << (glm::vec4)cc.Color;
			out << YAML::Key << "Fade" << YAML::Value << cc.Fade;
			out << YAML::Key << "Radius" << YAML::Value << cc.Radius;
			out << YAML::Key << "Thickness" << YAML::Value << cc.Thickness;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>()) {
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;

			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			switch (rb2d.Type)
			{
				case RigidBody2DComponent::BodyType::Static:	out << YAML::Key << "Type" << YAML::Value << "Static"; break;
				case RigidBody2DComponent::BodyType::Dynamic:	out << YAML::Key << "Type" << YAML::Value << "Dynamic"; break;
				case RigidBody2DComponent::BodyType::Kinematic:	out << YAML::Key << "Type" << YAML::Value << "Kinematic"; break;
				default: NT_CORE_ASSERT(false, "Invalid RB2D Type!"); break;
			}
			
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap;

			auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; //Entity
	}

	bool SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entitys";
		out << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };

			if (!entity)
				return;

			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		NT_CORE_ASSERT(false, "Runtimeserializer not implemented!");
		return false;
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		NT_CORE_TRACE("Deserializing Scene '{0}'", sceneName);

		auto entites = data["Entitys"];
		if (entites) {
			for (auto entity : entites)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				NT_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);
				deserializedEntity.GetComponent<IDComponent>().ID = UUID(uuid);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent) {
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scalation = transformComponent["Scalation"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& camProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)camProps["ProjectionType"].as<int>());

					cc.Camera.SetFOV(camProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(camProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(camProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(camProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthoNearClip(camProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthoFarClip(camProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent) {
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["Texture"].as<std::string>() != "none") {
						src.Texture = Texture2D::Create(spriteRendererComponent["Texture"].as<std::string>());
					}
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto cirlceRendererComponent = entity["CircleRendererComponent"];
				if (cirlceRendererComponent) {
					auto& cc = deserializedEntity.AddComponent<CircleRendererComponent>();
					cc.Color = cirlceRendererComponent["Color"].as<glm::vec4>();
					cc.Thickness = cirlceRendererComponent["Thickness"].as<float>();
					cc.Radius = cirlceRendererComponent["Radius"].as<float>();
					cc.Fade = cirlceRendererComponent["Fade"].as<float>();
				}

				auto rigidBody2DComponet = entity["RigidBody2DComponent"];
				if (rigidBody2DComponet) {
					auto& rb2d = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rb2d.FixedRotation = rigidBody2DComponet["FixedRotation"].as<bool>();
					auto type = rigidBody2DComponet["Type"].as<std::string>();
					if(type == "Static") rb2d.Type = RigidBody2DComponent::BodyType::Static;
					if(type == "Dynamic") rb2d.Type = RigidBody2DComponent::BodyType::Dynamic ;
					if(type == "Kinematic")	rb2d.Type = RigidBody2DComponent::BodyType::Kinematic;
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent) {
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2d.Density = circleCollider2DComponent["Density"].as<float>();
					cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}
			}
		}
		
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		NT_CORE_ASSERT(false, "Runtimeserializer not implemented!");
		return false;
	}
}
