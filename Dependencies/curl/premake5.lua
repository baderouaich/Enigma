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
 
 
	filter "system:linux or bsd or solaris"
		defines
		{
			"CURL_HIDDEN_SYMBOLS"
		}
		links
		{
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