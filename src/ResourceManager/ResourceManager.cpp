#include <pch.hpp>
#include "ResourceManager.hpp"
#include <Utility/DialogUtils.hpp>
#include <Application/Application.hpp>
#include <Logger/Logger.hpp>
#include <System/ENV/ENV.hpp>

NS_ENIGMA_BEGIN

void ResourceManager::initialize() {
  loadFonts();
}

void ResourceManager::shutdown() {
}

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


ImFont*& ResourceManager::getFont(std::string_view name) {
  ENIGMA_ASSERT(m_fonts.contains(name), "Font was not loaded");
  return m_fonts.at(name);
}

void ResourceManager::loadFonts() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  ENIGMA_TRACE("Loading Fonts...");

  const auto& io = ImGui::GetIO();

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

  ImFontConfig fontConfig = ImFontConfig();
  //fontConfig.FontDataOwnedByAtlas = true;

//  m_fonts["Audiowide-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 60.0f, &fontConfig);
//  m_fonts["Audiowide-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 45.0f, &fontConfig);
//  m_fonts["Audiowide-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 30.0f, &fontConfig);
//  m_fonts["Audiowide-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 20.0f, &fontConfig);

  m_fonts["Ubuntu-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 60.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 45.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 30.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 20.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-18"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 18.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-16"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 16.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-14"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 14.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-12"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 12.0f, &fontConfig);

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
