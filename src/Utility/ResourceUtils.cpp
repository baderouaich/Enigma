#include "ResourceUtils.hpp"
#include <Core/Core.hpp>
#include <System/ENV/ENV.hpp>

NS_ENIGMA_BEGIN

fs::path ResourceUtils::getInstallBaseDir() {
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

fs::path ResourceUtils::GetResourcesDir() {
  static const fs::path localResDir = fs::path(LOCAL_RES_DIR);
  static const fs::path installedResDir = getInstallBaseDir() / "res";
  // If Enigma was installed user wide (make enigma_install) target
  if (fs::is_directory(installedResDir))
    return installedResDir;
  else
    // Enigma was never installed user wide, use the local project res/ dir
    return localResDir;
}
NS_ENIGMA_END
