#pragma once
#ifndef ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H
#define ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H

#include <Core/Core.hpp>
#include <Core/Version.hpp>
#include <Logger/Logger.hpp>

#include <memory>

#include <cpr/cpr.h> // cpr (cURL c++ wrapper)
#include <json.hpp> // nlohmann-json
using namespace nlohmann;

NS_ENIGMA_BEGIN
class ENIGMA_API CheckForUpdates final
{
public:
	struct LatestReleaseInfo
	{

		String name;   // "name": "Enigma Release (Windows x64, Linux x64)",
		String tag_name; // version e.g v1.0.0
		String created_at;//"created_at": "2021-02-06T11:41:26Z",
		String published_at;	//"published_at" : "2021-02-06T12:16:37Z",
		String body;	//  "body": "Enigma first stable release for Windows x64 and Linux x64 using:\r\n- Crypto++ v8.4.0\r\n- GLFW v3.3.2\r\n- ImGui v1.79\r\n- spdlog v1.8.0\r\n- and other libraries"
		String tarball_url; // "tarball_url": "https://api.github.com/repos/BaderEddineOuaich/Enigma/tarball/v1.0.0",
		String zipball_url;	// "zipball_url" : "https://api.github.com/repos/BaderEddineOuaich/Enigma/zipball/v1.0.0",

		LatestReleaseInfo()
			:
			tag_name("<unknown>"),
			name("<unknown>"),
			created_at("<unknown>"),
			published_at("<unknown>"),
			body("<unknown>"),
			tarball_url("<unknown>"),
			zipball_url("<unknown>")
		{}
		
		~LatestReleaseInfo()
		{
			tag_name.clear();
			created_at.clear();
			published_at.clear();
			body.clear();
		}

		static std::unique_ptr<LatestReleaseInfo> FromJson(const json& obj)
		{
#define CC1(a, b) a##b
#define CC(a, b) CC1(a, b)
#define ASSIGN_IF(var, field, get_type, json_type) \
		if(!obj[field].is_null() && CC(obj[field].is_, json_type)()) \
			(var) = obj[field].get<get_type>(); 

			std::unique_ptr<LatestReleaseInfo> info = std::make_unique<LatestReleaseInfo>();
			ASSIGN_IF(info->name, "name", String, string);
			ASSIGN_IF(info->tag_name, "tag_name", String, string);
			ASSIGN_IF(info->created_at, "created_at", String, string);
			ASSIGN_IF(info->published_at, "published_at", String, string);
			ASSIGN_IF(info->body, "body", String, string);
			ASSIGN_IF(info->tarball_url, "tarball_url", String, string);
			ASSIGN_IF(info->zipball_url, "zipball_url", String, string);
			return info;

#undef ASSIGN_IF
#undef CC1
#undef CC
		}
		
		String toString() noexcept
		{
			std::ostringstream oss;
			if (!name.empty())
				oss << "name: " << name << " ";
			if (!tag_name.empty())
				oss << "tag name: " << tag_name << " ";
			if (!created_at.empty())
				oss << "created at: " << created_at << " ";
			if (!published_at.empty())
				oss << "published at: " << published_at << " ";
			if (!body.empty())
				oss << "body: " << body << " ";
			if (!tarball_url.empty())
				oss << "tarball_url: " << tarball_url << " ";
			if (!zipball_url.empty())
				oss << "zipball_url: " << zipball_url << " ";
			return oss.str();
		}

		
	};

public:
	static std::unique_ptr<LatestReleaseInfo> GetLatestReleaseInfo()
	{
		try
		{
			cpr::Url url = Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE;
			cpr::Response response = cpr::Get(url);
			if (response.status_code == cpr::status::HTTP_OK)
			{
				json data = json::parse(response.text);
				return LatestReleaseInfo::FromJson(data);
			}
			else
				ENIGMA_WARN("Failed to get latest release info with status code {0}", response.status_code);
		}
		catch (const std::exception& e)
		{
			ENIGMA_WARN("Failed to get latest release info with exception {0}", e.what());
		}
		return nullptr;
	}

};
NS_ENIGMA_END


#endif // !ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H
