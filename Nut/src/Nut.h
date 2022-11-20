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
#include "Nut/Renderer/Framebuffer.h"
#include "Nut/Scene/Scene.h"
#include "Nut/Scene/Components.h"
#include "Nut/Scene/ScriptableEntity.h"
#include "Nut/Scene/SceneSerializer.h"
#include "Nut/Utils/PlatformUtils.h"

#include "imgui_internal.h"
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <enTT/entt.hpp>
//#include <UUIDv4/uuid_v4.h>
//#include <UUIDv4/endianness.h>
#include <ImGuizmo.h>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
 
#include "Nut/Math/Math.h"