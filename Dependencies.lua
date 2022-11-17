-- Nut Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/Nut/vendor/spdlog/include"
IncludeDir["stb"] = "%{wks.location}/Nut/vendor/stb"
IncludeDir["GLFW"] = "%{wks.location}/Nut/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Nut/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Nut/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Nut/vendor/glm"
IncludeDir["stb"] = "%{wks.location}/Nut/vendor/stb"
IncludeDir["entt"] = "%{wks.location}/Nut/vendor/entt"
IncludeDir["yaml"] = "%{wks.location}/Nut/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Nut/vendor/ImGuizmo"
IncludeDir["shaderc"] = "%{wks.location}/Hazel/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazel/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"
Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"
Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Output dir path
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"