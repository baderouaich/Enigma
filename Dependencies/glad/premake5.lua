project "glad"
	kind "StaticLib"
	language "C"
	cdialect "C11"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	--All files GLAD has
	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
		}
		links
		{
		}


	filter "system:linux"
	    pic "On" -- required fo clang-10
		defines
		{
		}
		links
		{
		}

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