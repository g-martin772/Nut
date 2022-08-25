#include "ntpch.h"
#include "OrthoGraphicCameraController.h"

#include <Nut/Core/KeyCodes.h>
#include <Nut/Core/Input.h>

namespace Nut {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotation) 
        : m_AspecrRatio(aspectRatio), m_rotation(enableRotation), m_Camera(-m_AspecrRatio * m_ZoomLevel, m_AspecrRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(NT_KEY_LEFT)) {
            m_CameraPosition.x -= m_CameraMovementSpeed * ts;
        }
        else if (Input::IsKeyPressed(NT_KEY_RIGHT)) {
            m_CameraPosition.x += m_CameraMovementSpeed * ts;
        }
        if (Input::IsKeyPressed(NT_KEY_UP)) {
            m_CameraPosition.y += m_CameraMovementSpeed * ts;
        }
        else if (Input::IsKeyPressed(NT_KEY_DOWN)) {
            m_CameraPosition.y -= m_CameraMovementSpeed * ts;
        }

        if (m_rotation) {
            if (Input::IsKeyPressed(NT_KEY_Q)) {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }
            else if (Input::IsKeyPressed(NT_KEY_E)) {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            m_Camera.SetRotation(m_CameraRotation);
        }
        
        m_Camera.SetPosition(m_CameraPosition);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(NT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
        dispatcher.Dispatch<WindowResizeEvent>(NT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
    }

    bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset();
        m_Camera.SetProjection(-m_AspecrRatio * m_ZoomLevel, m_AspecrRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        m_AspecrRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspecrRatio * m_ZoomLevel, m_AspecrRatio, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}