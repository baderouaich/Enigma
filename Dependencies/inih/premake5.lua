project "inih"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	warnings "Off"


    includedirs 
    {
     	"include/", 
    }
    files
    {
    	"src/INIReader.cpp",
    	"src/ini.c"
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
		links
		{
		}
		defines
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
