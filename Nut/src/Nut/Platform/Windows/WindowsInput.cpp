#include "ntpch.h"
#include "Nut/Core/Input.h"
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include "Nut/Core/Application.h"

namespace Nut {
	bool Input::IsKeyPressed(int keycode)
	{
		auto  window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto  window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	float Input::GetMouseX()
	{
		auto  window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)x;
	}
	float Input::GetMouseY()
	{
		auto  window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return (float)y;
	}
	std::pair<float, float> Input::GetMousePos()
	{
		auto  window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}
}