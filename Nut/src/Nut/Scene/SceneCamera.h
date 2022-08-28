#pragma once

#include "Nut/Camera/Camera.h"

namespace Nut {
	class SceneCamera : public Camera {
	public:
		enum class ProjectionType {
			Perspective = 0,
			Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic();
		void SetOrthographic(float size, float nearClip, float farClip);

		void SetPerspective();
		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicsSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection();}

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; RecalculateProjection();}

		float GetOrthoFarClip() { return m_OrthographicFar; }
		void SetOrthoFarClip(float clip) { m_OrthographicFar = clip; RecalculateProjection();}
		float GetOrthoNearClip() { return m_OrthographicNear; }
		void SetOrthoNearClip(float clip) { m_OrthographicNear = clip; RecalculateProjection(); }

		float GetPerspectiveFarClip() { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float clip) { m_PerspectiveFar = clip; RecalculateProjection(); }
		float GetPerspectiveNearClip() { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float clip) { m_PerspectiveNear = clip; RecalculateProjection(); }

		ProjectionType GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection();}
	private:
		void RecalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_FOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.001f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRatio = 1.0f;
		float m_Width = 1.0f;
		float m_Height = 1.0f;
	};
}