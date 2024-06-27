#pragma once
#ifndef ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H
#define ENIGMA_NETWORKING_CHECK_FOR_UPDATES_H

#include <Core/Core.hpp>
#include <Core/Version.hpp>
#include <Logger/Logger.hpp>

#include <memory>

#pragma warning(push, 0)
/// cpr: cURL c++ wrapper
#include <cpr/cpr.h>
/// nlohmann-json: json parser library
#include <nlohmann/json.hpp>
#pragma warning(pop)


NS_ENIGMA_BEGIN
class ENIGMA_API CheckForUpdates final
{
	ENIGMA_STATIC_CLASS(CheckForUpdates);
public:
	/** Comparable Version struct, an instance can be made from a version string e.g "1.0.0" */
	struct Version
	{
		ui16 major;
		ui16 minor;
		ui16 patch;

		Version() : major(0), minor(0), patch(0) {}
		Version(const String& tag_name)
		{
			Parse(tag_name);
		}

		void Parse(const String& tag_name)
		{
			const std::vector<String> parts = StringUtils::Split(tag_name, '.');
			ENIGMA_ASSERT_OR_THROW(parts.size() == 3, fmt::format("Version tag_name is malformed, expected major.minor.patch (%d.%d.%d) but received {}", tag_name));

			// Convert string parts to ui16 major minor patch
			const auto stoui16 = [](const String& str) -> ui16
			{
				ui16 i{};
				std::stringstream oss(str);
				oss >> i;
				return i;
			};

			major = stoui16(parts[0]);
			minor = stoui16(parts[1]);
			patch = stoui16(parts[2]);
		}


    constexpr bool operator>(const Version& other) const noexcept {
      if (major > other.major) return true;
      if (major < other.major) return false;
      // At this point, major == other.major
      if (minor > other.minor) return true;
      if (minor < other.minor) return false;
      // At this point, minor == other.minor
      return patch > other.patch;
    }

    constexpr bool operator<(const Version& other) const noexcept {
      if (major < other.major) return true;
      if (major > other.major) return false;
      // At this point, major == other.major
      if (minor < other.minor) return true;
      if (minor > other.minor) return false;
      // At this point, minor == other.minor
      return patch < other.patch;
    }

    constexpr bool operator==(const Version& other) const noexcept {
      return major == other.major && minor == other.minor && patch == other.patch;
    }

    friend std::ostream& operator<<(std::ostream& os, const Version& v) noexcept {
      return os << v.major <<'.' << v.minor << '.' << v.patch;
    }
	};

	/** Latest Enigma release information data struct */
	struct LatestReleaseInfo
	{
		String name;   // "name": "Enigma Release (Windows x64, Linux x64)",
		String tag_name; // version e.g 1.0.0
		String created_at;//"created_at": "2021-02-06T11:41:26Z",
		String published_at;	//"published_at" : "2021-02-06T12:16:37Z",
		String body;	//  "body": "Enigma first stable release for Windows x64 and Linux x64 using:\r\n- Crypto++ v8.4.0\r\n- GLFW v3.3.2\r\n- ImGui v1.79\r\n- spdlog v1.8.0\r\n- and other libraries"
		String tarball_url; // "tarball_url": "https://api.github.com/repos/BaderEddineOuaich/Enigma/tarball/v1.0.0",
		String zipball_url;	// "zipball_url" : "https://api.github.com/repos/BaderEddineOuaich/Enigma/zipball/v1.0.0",
		Version version; // parsed comparable version

		LatestReleaseInfo() noexcept
			:
			tag_name("<unknown>"),
			name("<unknown>"),
			created_at("<unknown>"),
			published_at("<unknown>"),
			body("<unknown>"),
			tarball_url("<unknown>"),
			zipball_url("<unknown>"),
			version()
		{}
		
		~LatestReleaseInfo() noexcept = default;

		static std::unique_ptr<LatestReleaseInfo> FromJson(const nlohmann::json& obj)
		{
#define CC1(a, b) a##b
#define CC(a, b) CC1(a, b)
#define ASSIGN_IF(var, field, json_type) \
		if(!obj[field].is_null() && CC(obj[field].is_, json_type)()) \
			(var) = obj[field].get<decltype(var)>(); 

			std::unique_ptr<LatestReleaseInfo> info = std::make_unique<LatestReleaseInfo>();
			ASSIGN_IF(info->name, "name", string);
			ASSIGN_IF(info->tag_name, "tag_name", string);
			ASSIGN_IF(info->created_at, "created_at", string);
			ASSIGN_IF(info->published_at, "published_at", string);
			ASSIGN_IF(info->body, "body", string);
			ASSIGN_IF(info->tarball_url, "tarball_url", string);
			ASSIGN_IF(info->zipball_url, "zipball_url", string);

			info->version.Parse(info->tag_name); // parse version by tagname
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
	/** Returns the Latest Enigma release information using GitHub API */
	static std::unique_ptr<LatestReleaseInfo> GetLatestReleaseInfo()
	{
		try
		{
			cpr::Url url = Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE;
			cpr::Response response = cpr::Get(url);
			if (response.status_code == cpr::status::HTTP_OK)
			{
				nlohmann::json data = nlohmann::json::parse(response.text);
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
