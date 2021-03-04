project "stb_image"
	kind "StaticLib"
	language "C"
	cdialect "C11"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/stb_image.h",
		"src/stb_image.c"
	}
	
	defines
	{
		"STB_IMAGE_STATIC"
	}

	includedirs
	{
		"include"
	}
		
	--- Platform ---
	--[[ Windows ]]--
	filter "system:windows"
		systemversion "latest"
		defines
		{
		}
		links
		{
		}

	--[[ Linux ]]--
	filter "system:linux"
		defines
		{
		}
		links
		{
		}

	--[[ MacOS ]]--
	filter "system:macosx"
		defines
		{
		}	
		links
		{
		}

	--- Configurations ---
	filter "configurations:Debug"
		runtime "Debug"
		symbols "Full" --vs17 and newer | symbols "On"
		optimize "Off" -- No optimization will be performed.

	filter "configurations:Release"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance