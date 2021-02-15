project "asio"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

    includedirs 
    {
     	"include/",  -- header only
    }
    files
    {
    	"src/asio_added_to_compile_headers.cpp" -- cpp file has asio.hpp include to compile library
    }
    defines	
    {
    	"ASIO_STANDALONE" -- don't depend on Boost
    }

	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"WIN32_LEAN_AND_MEAN",
			"_WIN32_WINNT=0x0A00",
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
		}
		links
		{
			"ws2_32" -- windows sockets lib
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
		symbols "On"
		optimize "Off" -- No optimization will be performed.

	filter "configurations:Release"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.


	filter "configurations:Dist"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance
