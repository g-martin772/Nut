#include "ntpch.h"
#include "SceneCamera.h"

#include "../vendor/glm/glm/gtc/matrix_transform.hpp"

namespace Nut {
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic()
	{
		m_ProjectionType = ProjectionType::Orthographic;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective()
	{
		m_ProjectionType = ProjectionType::Perspective;
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_FOV = fov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		m_Width = width;
		m_Height = height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Orthographic) {
			float Left = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float Right = m_OrthographicSize * m_AspectRatio * 0.5f;
			float Bottom = -m_OrthographicSize * 0.5f;
			float Top = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(Left, Right, Bottom, Top, m_OrthographicNear, m_OrthographicFar);
		}
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspectiveFov(m_FOV, m_Width, m_Height, m_PerspectiveNear, m_PerspectiveFar);
		}
	}
}
