#include "ntpch.h"
#include "OrthographicCamera.h"

void Nut::OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
	m_ProjectionMatrix = (glm::ortho(left, right, bottom, top, -1.0f, 1.0f));
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
