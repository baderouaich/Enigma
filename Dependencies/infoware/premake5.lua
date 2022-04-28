project "infoware"
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
    	"src/**.cpp",
    }
    excludes
    {
    	"src/pci.cpp",
    	"include/infoware/pci.hpp",
    }

    -- check https://github.com/ThePhD/infoware/blob/main/CMakeLists.txt for compilation settings
    defines
    {
    	"INFOWARE_VERSION=\"0.6.0\"",
   		"INFOWARE_USE_OPENGL=1", --TODO: Use OpenGL to extract GPU info (although not implemented yet by infoware, System info tool displays only minimal data for GPU info from opengl, waiting for furter releases)
    }

	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{

		}
		links
		{
			-- for infoware/system
			"version.lib", 
			"wbemuuid.lib",

			-- for infoware/gpu (if defined  INFOWARE_USE_D3D, but pci data still required)
			--"D3D11.lib", 
			--"DXGI.lib",
		}


	filter "system:linux"
		defines
		{
			"INFOWARE_USE_X11" -- in Linux, use x11 to get display info
		}
		links
		{
			"X11", 
			"Xrandr"
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
