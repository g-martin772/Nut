include "Dependencies.lua"

workspace "Nut"
    startproject "Nero"
    architecture "x64"

    configurations {
        "Debug",
        "Resease",
        "Dist"
    }

group "Dependencies"
    include "Nut/vendor/GLFW"
    include "Nut/vendor/GLAD"
    include "Nut/vendor/imgui"
    include "Nut/vendor/yaml-cpp"
    include "Nut/vendor/Box2D"
group ""

include "Nut"
include "Nut-ScriptCore"
include "Nero"
include "Sandbox"