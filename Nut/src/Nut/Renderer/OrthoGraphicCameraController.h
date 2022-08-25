#pragma once

#include <Nut/Renderer/OrthographicCamera.h>
#include <Nut/Core/Timestep.h>
#include <Nut/Events/KeyEvents.h>
#include <Nut/Events/MouseEvent.h>
#include <Nut/Events/AppEvent.h>

namespace Nut {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel(float level) const { return m_ZoomLevel; }
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_AspecrRatio;
		float m_ZoomLevel = 1.0f;

		bool m_rotation = false;
		float m_CameraRotationSpeed = 5.0f;
		float m_CameraRotation = 0.0f;

		float m_CameraMovementSpeed = 1.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		OrthographicCamera m_Camera;
	};
}