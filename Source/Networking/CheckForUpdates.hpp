#pragma once
#ifndef ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H
#define ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H

#include <Core/Core.hpp>
#include <Core/Version.hpp>
#include <Logger/Logger.hpp>

#include <cpr/cpr.h>
//Constants::Links::ENIGMA_LATEST_RELEASE_GITHUB_API
constexpr const auto URL = "https://api.github.com/repos/BaderEddineOuaich/Enigma/releases/latest";
//TODO
NS_ENIGMA_BEGIN
class ENIGMA_API CheckForUpdates final
{
	struct ReleaseInfo
	{
		String tag_name; // version e.g v1.0.0
		String created_at;//"created_at": "2021-02-06T11:41:26Z",
		String published_at;	//"published_at" : "2021-02-06T12:16:37Z",

		std::vector<std::pair<String, String>> assets; // {name, url.zip}
		String linux_asset_url; // browser_download_url https://github.com/BaderEddineOuaich/Enigma/releases/download/v1.0.0/Enigma-v1.0.0-Linux-x64.zip
		String windows_asset_url; // browser_download_url https://github.com/BaderEddineOuaich/Enigma/releases/download/v1.0.0/Enigma-v1.0.0-Windows-x64.zip
	};
public:
	static bool IsNewVersionAvailable()
	{
		const auto current_version = ENIGMA_VERSION;
		cpr::Response r = cpr::Get(cpr::Url{ URL }
			//,cpr::Authentication{ "user", "pass" },
			//cpr::Parameters{ {"anon", "true"}, {"key", "value"} }
		);
		if (r.status_code == cpr::status::HTTP_OK)
		{
			ENIGMA_INFO("");
			//std::cout << "r.status_code" << r.status_code << std::endl;                  // 200
			//std::cout << "r.header" << r.header["content-type"] << std::endl;       // application/json; charset=utf-8
			//std::cout << "r.text" << r.text << std::endl;
		}
	
		return false;
	}

	static ReleaseInfo GetLatestReleaseInfo()
	{
		return {};
	}
};
NS_ENIGMA_END


#endif // !ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H
