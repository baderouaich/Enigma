project "tabulate"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	warnings "Off"
		
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")


	includedirs 
	{
		"include/",  -- header only
	}
	files
	{
		"include/tabulate.hpp",
		"src/tabulate.cpp" -- has the header only include, to compile project as static lib 
	}

	defines	
	{
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
