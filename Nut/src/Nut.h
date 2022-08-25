#pragma once

//Inspired by TheCherno's Hazel Engine

#include "ntpch.h"
#include "Nut/Core/Core.h"
#include "Nut/Core/Log.h"
#include "Nut/Core/LayerStack.h"
#include "Nut/Core/Application.h"
#include "Nut/Events/MouseEvent.h"
#include "Nut/Events/KeyEvents.h"
#include "Nut/Events/AppEvent.h"
#include "Nut/ImGui/ImGuiLayer.h"
#include "Nut/Core/KeyCodes.h"
#include "Nut/Core/Input.h"
#include "Nut/Renderer/Buffer.h"
#include "Nut/Renderer/OrthographicCamera.h"
#include "Nut/Renderer/RenderCommand.h"
#include "Nut/Renderer/Renderer.h"
#include "Nut/Renderer/Shader.h"
#include "Nut/Core/Timestep.h"
#include "Nut/Renderer/Texture.h"
#include "Nut/Renderer/OrthoGraphicCameraController.h"

#include "../vendor/imgui/imgui_internal.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/glm/glm/glm.hpp"