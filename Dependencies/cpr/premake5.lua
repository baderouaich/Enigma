project "cpr"
	kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

    includedirs 
    {
     	"include/",
     	"%{IncludeDir.curl}", -- curl include
    }

    files
    {
    	"cpr/*.cpp"
    }

    defines	
    {
    	"CURL_STATICLIB"
    }

    links
    {
    	"curl" -- link curl .lib
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
		links
		{
			--"zlib",	-- link zlib for curl lib on linux
			--"mbedtls", -- link mbedtls for curl lib on linux
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
