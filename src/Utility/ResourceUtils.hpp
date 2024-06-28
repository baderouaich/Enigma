#pragma once

#include <Core/Types.hpp>

namespace Enigma {
  class ResourceUtils final {
    public:
      /// @brief Returns path to res/ folder
      /// Example: /home/$(whoami)/Enigma/res/
      static fs::path GetResourcesDir();

      /// @brief Returns path to installed Enigma/ folder
      /// Example: /home/$(whoami)/Enigma/
      static fs::path getInstallBaseDir();
  };
}
