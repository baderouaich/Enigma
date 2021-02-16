project "mbedtls"
	language    "C"
	kind        "StaticLib"
	warnings    "off"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{ 
		"include/",
		"%{IncludeDir.zlib}",
	}

	defines 
	{
		 'MBEDTLS_ZLIB_SUPPORT' 
	}
	
	files
	{
		"include/**.h",
		"library/**.c"
	}

    links
    {
    	"zlib"
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
