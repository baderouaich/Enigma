#include <pch.hpp>
#include "ResourceManager.hpp"
#include <Utility/DialogUtils.hpp>
#include <Application/Application.hpp>
#include <Logger/Logger.hpp>
#include <System/ENV/ENV.hpp>
#include <Ubuntu-Regular.hpp>

NS_ENIGMA_BEGIN

void ResourceManager::initialize() {
  loadFonts();
}

void ResourceManager::shutdown() {
  // DO NOT FREE fonts, ImGui will do so.
}

fs::path ResourceManager::getAppDataDir() {
  fs::path appDataPath;
  const std::string appName = "Enigma";
#if defined(ENIGMA_PLATFORM_WINDOWS)
  // Windows: Use APPDATA or fall back to a local directory
  const char *appData = std::getenv("APPDATA");
  appDataPath = appData ? fs::path(appData) / appName : fs::path("C:/ProgramData") / appName;
#elif defined(ENIGMA_PLATFORM_LINUX)
  // Linux and others: Use HOME or fall back to the current directory
  const char *homeDir = std::getenv("HOME");
  appDataPath = homeDir ? fs::path(homeDir) / ".local/share" / appName : fs::current_path() / appName;
#elif __APPLE__
  // macOS: Library/Application Support or fall back to home directory
  appDataPath = fs::path(std::getenv("HOME")) / "Library/Application Support" / appName;
  if (!fs::exists(appDataPath)) {
    appDataPath = fs::path(std::getenv("HOME")) / appName; // Fall back to home directory
  }
#endif
  // Create the directory if it doesn't exist
  if (!fs::is_directory(appDataPath))
    fs::create_directories(appDataPath);
  return appDataPath;
}

fs::path ResourceManager::getDatabaseFilepath() {
  const fs::path resDir = getResourcesDir();
  const fs::path dbDir = resDir / "database";
  if (!fs::is_directory(dbDir))
    fs::create_directories(dbDir);
  return dbDir / "Enigma.db";
}

fs::path ResourceManager::getLogsDir() {
  const fs::path resDir = getResourcesDir();
  const fs::path logsDir = resDir / "logs";
  if (!fs::is_directory(logsDir))
    fs::create_directories(logsDir);
  return logsDir;
}

// fs::path ResourceManager::getConfigDir() {
//   const fs::path resDir = getResourcesDir();
//   const fs::path configDir = resDir / "config";
//   if (!fs::is_directory(configDir))
//     fs::create_directories(configDir);
//   return configDir;
// }

fs::path ResourceManager::getResourcesDir() {
  static const fs::path resDir = getAppDataDir() / "res";
  if (!fs::is_directory(resDir))
    fs::create_directories(resDir);
  return resDir;
}


#if 0
fs::path ResourceManager::getInstallBaseDir() {
  // /home/$(whoami)/Enigma in Linux
  // C:\Program Files\Enigma in Windows
  fs::path installBaseDir{};

#if defined(ENIGMA_PLATFORM_WINDOWS)
  const std::string programFiles = ENV::Get("ProgramFiles");
  ENIGMA_ASSERT_OR_THROW(!programFiles.empty(), "The ProgramFiles environment variable is not defined.");
  installBaseDir = fs::path(programFiles) / "Enigma";
#elif defined(ENIGMA_PLATFORM_LINUX)
  const std::string homeDir = ENV::Get("HOME");
  ENIGMA_ASSERT_OR_THROW(!homeDir.empty(), "The HOME environment variable is not defined.");
  installBaseDir = fs::path(homeDir) / "Enigma";
#endif

  return installBaseDir;
}

fs::path ResourceManager::getResourcesDir() {
  static const fs::path localResDir = fs::path(LOCAL_RES_DIR);
  static const fs::path installedResDir = getInstallBaseDir() / "res";
  // If Enigma was installed user wide (make enigma_install) target
  if (fs::is_directory(installedResDir))
    return installedResDir;
  else
    // Enigma was never installed user wide, use the local project res/ dir
    return localResDir;
}

#endif

ImFont *& ResourceManager::getFont(std::string_view name) {
  ENIGMA_ASSERT(m_fonts.contains(name), "Font was not loaded");
  return m_fonts.at(name);
}

void ResourceManager::loadFonts() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  ENIGMA_TRACE("Loading Fonts...");

  const auto& io = ImGui::GetIO();


  ImFontConfig fontConfig = ImFontConfig();
  fontConfig.FontDataOwnedByAtlas = false;

  //  m_fonts["Audiowide-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 60.0f, &fontConfig);
  //  m_fonts["Audiowide-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 45.0f, &fontConfig);
  //  m_fonts["Audiowide-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 30.0f, &fontConfig);
  //  m_fonts["Audiowide-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 20.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-60"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 60.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-45"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 45.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-30"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 30.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-20"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 20.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-18"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 18.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-16"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 16.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-14"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 14.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-12"] = io.Fonts->AddFontFromMemoryTTF(Enigma::Fonts::Ubuntu_Regular_ttf, Enigma::Fonts::Ubuntu_Regular_ttf_len, 12.0f, &fontConfig);

#if 0
  //  const fs::path font_audiowide_path = Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH;
  //  const fs::path font_montserrat_path = Constants::Resources::Fonts::MONTSERRAT_FONT_PATH;
  const fs::path font_ubuntu_path = Constants::Resources::Fonts::UBUNTU_FONT_PATH;
  //  ENIGMA_ASSERT_OR_THROW(fs::exists(font_audiowide_path), "Font " + font_audiowide_path.string() + " not found");
  //  ENIGMA_ASSERT_OR_THROW(fs::exists(font_montserrat_path), "Font " + font_montserrat_path.string() + " not found");
  ENIGMA_ASSERT_OR_THROW(fs::exists(font_ubuntu_path), "Font " + font_ubuntu_path.string() + " not found");

#ifdef ENIGMA_DEBUG
  //  ENIGMA_LOG("Font Audiowide-Regular path: {}", font_audiowide_path.string());
  //  ENIGMA_LOG("Font Montserrat-Medium path: {}", font_montserrat_path.string());
  ENIGMA_LOG("Font Ubuntu-Regular path: {}", font_ubuntu_path.string());
#endif

  m_fonts["Ubuntu-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 60.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 45.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 30.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 20.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-18"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 18.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-16"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 16.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-14"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 14.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-12"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 12.0f, &fontConfig);
#endif
  //  m_fonts["Montserrat-Medium-45"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 45.0f, &fontConfig);
  //  m_fonts["Montserrat-Medium-20"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 20.0f, &fontConfig);
  //  m_fonts["Montserrat-Medium-18"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 18.0f, &fontConfig);
  //  m_fonts["Montserrat-Medium-16"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 16.0f, &fontConfig);
  //  m_fonts["Montserrat-Medium-14"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 14.0f, &fontConfig);
  //  m_fonts["Montserrat-Medium-12"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 12.0f, &fontConfig);

  // Build added fonts atlas --> imgui issue #3643
  io.Fonts->Build();

  // Check if fonts are loaded
  for (const auto& [font_name, font]: m_fonts) {
    if (font->IsLoaded()) {
      ENIGMA_TRACE("Font {} Loaded", font->ConfigData->Name);
    } else {
      const std::string err_msg = "Failed to load font " + std::string(font_name);
      // console alert
      ENIGMA_ERROR(err_msg);
      // ui alert
      (void) DialogUtils::Error("Resource Loading Error", err_msg);
      // no further app without dear fonts :c
      Application::getInstance()->EndApplication();
      break;
    }
  }
}

NS_ENIGMA_END
