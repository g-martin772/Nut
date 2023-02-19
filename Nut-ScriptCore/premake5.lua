project "Nut-ScriptCore"
    kind "SharedLib"
    language "C#"
	dotnetframework "4.7.2"

    targetdir ("%{wks.location}/Nero/Assets/Scripts/%{prj.name}")
    objdir ("%{wks.location}/int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.cs",
    }

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"