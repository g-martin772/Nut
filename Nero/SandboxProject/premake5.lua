local NutRootDir = '../../'

workspace "SandboxProject"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

    flags {
		"MultiProcessorCompile"
	}

include "Sandbox"

group "Nut"
	include (NutRootDir .. "/Nut-ScriptCore")
group ""
