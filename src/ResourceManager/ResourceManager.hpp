#pragma once
#include <Core/Macros.hpp>
#include <filesystem>
#include <imgui.h>
#include <unordered_map>
#include <string_view>
namespace fs = std::filesystem;

namespace Enigma {

  class Application;

  class ResourceManager final {
      ENIGMA_STATIC_CLASS(ResourceManager);

    public:
      static void initialize();
      static void shutdown();

      static fs::path getAppDataDir();

      static fs::path getDatabaseFilepath();
      static fs::path getLogsDir();
      //static fs::path getConfigDir();

      /// @brief Returns path to res/ folder
      /// Example: /home/$(whoami)/Enigma/res/
      static fs::path getResourcesDir();
#if 0
      /// @brief Returns path to res/ folder
      /// Example: /home/$(whoami)/Enigma/res/
      static fs::path getResourcesDir();

      /// @brief Returns path to installed Enigma/ folder
      /// Example: /home/$(whoami)/Enigma/
      static fs::path getInstallBaseDir();
#endif
      /// @brief Returns ImFont by name
      static ImFont*& getFont(std::string_view name);

    private:
      static void loadFonts();

    private:
      inline static std::unordered_map<std::string_view, ImFont *> m_fonts{};
  };
}