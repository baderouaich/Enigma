-- # Thanks to premake5 https://github.com/premake/premake-core/blob/master/contrib/curl/premake5.lua AND https://github.com/multitheftauto/mtasa-blue/tree/master/vendor/curl
project "curl"
	kind "StaticLib"
    language "C"
    cdialect "C11"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	warnings "Off"

    includedirs 
    {
     	"include/", 
     	"lib/",
	    "%{IncludeDir.zlib}",
    }

    files
	{
		"**.h",
		"**.c"
	}

	excludes 
	{
		"lib/amigaos.c",
		"lib/amigaos.h",
		"lib/config-amigaos.h",
		"lib/config-dos.h",
		"lib/config-mac.h",
		"lib/config-os400.h",
		"lib/config-riscos.h",
		"lib/config-symbian.h",
		"lib/config-tpf.h",
		"lib/config-win32ce.h",
		"lib/config-vxworks.h",
		"lib/setup-os400.h",
		"lib/setup-vms.h"
	}

	defines    
    {
        "BUILDING_LIBCURL",
        "CURL_STATICLIB", 
        "HTTP_ONLY", 
        "USE_ZLIB", 
        "HAVE_LIBZ", 
        "HAVE_ZLIB_H", 
        "HAVE_CONFIG_H"
    }

    links
    {
    	"zlib"
    }
	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_WINSOCK_DEPRECATED_NO_WARNINGS",
			"USE_SCHANNEL", 
			"USE_WINDOWS_SSPI",
			"USE_WIN32_IDN", 
			"WANT_IDN_PROTOTYPES"
		}
		links 
	 	{
	 		"Crypt32",
	 		"Normaliz",
	 		--"Ws2_32",
	 		--"Wldap32",
	    }

	    
	filter "system:linux or bsd or solaris"
	    pic "On" -- required fo clang-10
		defines 
		{ 
			"CURL_HIDDEN_SYMBOLS",
			"USE_MBEDTLS"
		}
		includedirs
		{
	    	"%{IncludeDir.mbedtls}",
		}
		
	    links
	    {
	    	"mbedtls"
	    }

		-- find the location of the ca bundle
		local ca = nil
		for _, f in ipairs {
			"/etc/ssl/certs/ca-certificates.crt",
			"/etc/pki/tls/certs/ca-bundle.crt",
			"/usr/share/ssl/certs/ca-bundle.crt",
			"/usr/local/share/certs/ca-root.crt",
			"/usr/local/share/certs/ca-root-nss.crt",
			"/etc/certs/ca-certificates.crt",
			"/etc/ssl/cert.pem" } do
			if os.isfile(f) then
				ca = f
				break
			end
		end
		if ca then
			defines { 'CURL_CA_BUNDLE="' .. ca .. '"' }
		end


	filter "system:macosx"
		defines 
		{ 
			"USE_DARWINSSL" 
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