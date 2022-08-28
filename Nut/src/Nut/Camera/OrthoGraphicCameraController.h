#pragma once

#include <Nut/Camera/OrthographicCamera.h>
#include <Nut/Core/Timestep.h>
#include <Nut/Events/KeyEvents.h>
#include <Nut/Events/MouseEvent.h>
#include <Nut/Events/AppEvent.h>

namespace Nut {
	struct OrthographicCameraBound {
		float Left, Right, Top, Bottom;
		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel(float level) const { return m_ZoomLevel; }

		const OrthographicCameraBound& GetBounds() { return m_Bounds; }
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void Resize(float width, float height);
	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
	private:
		float m_AspecrRatio;
		float m_ZoomLevel = 1.0f;

		bool m_rotation = false;
		float m_CameraRotationSpeed = 20.0f;
		float m_CameraRotation = 0.0f;

		float m_CameraMovementSpeed = 20.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		OrthographicCameraBound m_Bounds;
		OrthographicCamera m_Camera;
	};
}