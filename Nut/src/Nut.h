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
#include "Nut/Camera/OrthographicCamera.h"
#include "Nut/Renderer/RenderCommand.h"
#include "Nut/Renderer/Renderer.h"
#include "Nut/Renderer/Renderer2D.h"
#include "Nut/Renderer/Shader.h"
#include "Nut/Core/Timestep.h"
#include "Nut/Renderer/Texture.h"
#include "Nut/Camera/OrthoGraphicCameraController.h"
#include "Nut/Renderer/Framebuffer.h"
#include "Nut/Scene/Scene.h"
#include "Nut/Scene/Components.h"
#include "Nut/Scene/ScriptableEntity.h"
#include "Nut/Scene/SceneSerializer.h"
#include "Nut/Utils/PlatformUtils.h"

#include "../vendor/imgui/imgui_internal.h"
#include "../vendor/imgui/imgui.h"

#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "../vendor/glm/glm/gtc/matrix_transform.hpp"

#include "../vendor/enTT/entt.hpp"
#include "../vendor/UUIDv4/uuid_v4.h"
#include "../vendor/UUIDv4/endianness.h"
#include "../vendor/ImGuizmo/ImGuizmo.h"

#include <../vendor/glm/glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <../vendor/glm/glm/gtx/compatibility.hpp>
 
#include "Nut/Math/Math.h"