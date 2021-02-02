project "cryptopp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		--[[
		Define this to disable ASM, intrinsics and built-ins. The code will be 
		compiled using C++ only. The library code will not include SSE2 (and above),
		NEON, Aarch32, Aarch64, Power4, Power7 or Power8.
		#define CRYPTOPP_DISABLE_ASM 1
		]] 
		--"CRYPTOPP_DISABLE_ASM=1",
		--"CRYPTOPP_ENABLE_NAMESPACE_WEAK", -- enable only when using MD5 to disable warning 'You may be using a weak algorithm that has been retained for backwards compatibility...'
		--"CRYPTOPP_IMPORTS", -- for static build
		--"CRYPTOPP_EXPORTS",  -- for dynamic build
	}

	-- Enable SSE2 vector processing https://github.com/premake/premake-core/wiki/vectorextensions
	vectorextensions "SSE2"


	files
	{
		"*.h",
		"*.cpp",
		"*.asm" -- !!! NOTE !!!: make sure you include asm files when pushing to github (configure .gitignore) otherwise workflow wont find them
	}


	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
		}
		links 
	 	{
	 		"ws2_32"
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
