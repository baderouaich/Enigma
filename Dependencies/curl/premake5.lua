-- # Thanks to premake5 again <3 https://github.com/premake/premake-core/blob/master/contrib/curl/premake5.lua
project "curl"
	kind "StaticLib"
    language "C"
    cdialect "C11"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

    includedirs 
    {
     	"include/", 
     	"lib/",
    }
	files
	{
		"**.h",
		"**.c"
	}
    defines	
    {
		"BUILDING_LIBCURL",
		"CURL_STATICLIB",
		--"HTTP_ONLY"
    }
	warnings "off"

	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
			"USE_SCHANNEL", 
			"USE_WINDOWS_SSPI"
		}
		links 
	 	{
	 		"Ws2_32",
	 		"Wldap32",
	 		"Crypt32"
	    }
 
	filter "system:linux"
		defines
		{
			"CURL_HIDDEN_SYMBOLS"
		}
		links
		{
		}

	filter "system:macosx"
		defines
		{
			"USE_DARWINSSL"
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