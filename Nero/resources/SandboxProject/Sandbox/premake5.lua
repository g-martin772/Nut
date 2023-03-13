project "Sandbox"
    kind "SharedLib"
    language "C#"
	dotnetframework "4.7.2"

    targetdir ("%{wks.location}/bin/%{prj.name}")
    objdir ("%{wks.location}/int/%{prj.name}")

    files {
        "src/**.cs",
    }

    links {
        "../../../../Nero/assets/Scripts/Nut-ScriptCore/Nut-ScriptCore.dll"
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