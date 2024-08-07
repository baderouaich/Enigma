#include <pch.hpp>
#include "SystemAndHardwareInfoTool.hpp"
#include <Application/Application.hpp>
#include <Utility/SizeUtils.hpp>


NS_ENIGMA_BEGIN

void SystemAndHardwareInfoTool::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void SystemAndHardwareInfoTool::OnDraw(Scene *) {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  //const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();

  //const auto button_size = ImVec2(win_w / 2.6f, 40.0f);

  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static ImFont *const& font_ubuntu_regular_60 = ResourceManager::getFont("Ubuntu-Regular-60");
  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_16 = ResourceManager::getFont("Ubuntu-Regular-16");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");

  ImGui::PushFont(font_ubuntu_regular_20);
  {
    if (ImGui::CollapsingHeader("System and Hardware Information", nullptr)) {
      ImGui::PushFont(font_ubuntu_regular_18);
      spacing(3);

      /// CPU Information BEGIN /// Static
      if (ImGui::TreeNode("CPU")) {
        // Static cpu info, unlike ram, needs to be initialized once.
        static const iware::cpu::quantities_t quantities = iware::cpu::quantities();
        static const std::string architecture = this->StringifyArchitectureEnum(iware::cpu::architecture());
        static const std::uint64_t frequency = iware::cpu::frequency();
        static const std::string endianness = iware::cpu::endianness() == iware::cpu::endianness_t::big ? "Big" : "Little";
        static const std::string model_name = iware::cpu::model_name();
        static const std::string vendor = iware::cpu::vendor();
        static const std::string vendor_id = iware::cpu::vendor_id();
        static std::vector<std::string> supported_instructions_set{}; // SSE, SSE2, AVX...
        static std::array<iware::cpu::cache_t, 3> caches{};

        // Get Instructions Set And Caches
        // Will execute just once
        static std::once_flag once_flag{};
        std::call_once(once_flag, [this] {
          // Instructions set
          for (const auto is: iware::cpu::supported_instruction_sets()) {
            supported_instructions_set.push_back(this->StringifyInstructionSetEnum(is));
          }
          // Caches
          for (std::int32_t i = 0; i < caches.size(); ++i) {
            caches[i] = iware::cpu::cache(i + 1);
          }
        });
        /*
				* method 2 of execute once
				* [[maybe_unused]] static const auto GetInstructionsSetAndCachesOnce = [this]() -> bool
				* {
				* 	// Instructions set
				* 	for (const auto is : iware::cpu::supported_instruction_sets())
				* 	{
				* 		supported_instructions_set.push_back(this->StringifyInstructionSetEnum(is));
				* 	}
				* 	// Caches
				* 	for (std::int32_t i = 0; i < caches.size(); ++i)
				* 	{
				* 		caches[i] = iware::cpu::cache(i + 1);
				* 	}
				* 
				* 	return true;
				* }();
				*/

        // Architecture, Frequency, Model name, Vendor Name, Vendor ID
        ImGui::BulletText("%s: %s", ("Architecture"), architecture.c_str());
        ImGui::BulletText("%s: %zu", ("Frequency"), frequency);
        ImGui::BulletText("%s: %s", ("Endianness"), endianness.c_str());
        ImGui::BulletText("%s: %s", ("Model name"), model_name.c_str());
        ImGui::BulletText("%s: %s", ("Vendor"), vendor.c_str());
        ImGui::BulletText("%s: %s", ("Vendor ID"), vendor_id.c_str());

        //spacing(2);

        // Quantities
        if (ImGui::TreeNode("Quantities")) {
          ImGui::BulletText("%s: %u", ("Logical CPUs"), quantities.logical);
          ImGui::BulletText("%s: %u", ("Physical CPUs"), quantities.physical);
          ImGui::BulletText("%s: %u", ("CPU packages"), quantities.packages);

          ImGui::TreePop();
        }

        //spacing(2);

        // Caches
        if (ImGui::TreeNode("Caches")) {
          for (std::size_t i = 0; i < caches.size(); ++i) {
            if (ImGui::TreeNode(std::to_string(i + 1).c_str())) {
              ImGui::BulletText("%s: %s", ("Size"), SizeUtils::FriendlySize(caches[i].size).c_str());
              ImGui::BulletText("%s: %s", ("Line Size"), SizeUtils::FriendlySize(caches[i].line_size).c_str());
              ImGui::BulletText("%s: %d", ("Associativity"), static_cast<std::int32_t>(caches[i].associativity));
              ImGui::BulletText("%s: %s", ("Type"), this->StringifyCacheTypeEnum(caches[i].type));

              ImGui::TreePop();
            }
          }

          ImGui::TreePop();
        }


        //spacing(2);

        // Supported Instruction Set
        if (ImGui::TreeNode("Supported Instruction Set")) {
          for (const auto& is: supported_instructions_set) {
            ImGui::BulletText("%s", is.data());
          }
          ImGui::TreePop();
        }


        ImGui::TreePop();
      }
      /// CPU Information END ///


      /// RAM Information BEGIN /// Volatile
      if (ImGui::TreeNode("RAM")) {
        // Volatile ram info (will only execute if TreeNode is open, unlike cpu info which are static and preferrably initialized at first)
        const auto ram_info = iware::system::memory();

        // Physical
        if (ImGui::TreeNode("Physical")) {
          ImGui::BulletText("%s: %s", ("Available"), SizeUtils::FriendlySize(ram_info.physical_available).c_str());
          ImGui::BulletText("%s: %s", ("Total"), SizeUtils::FriendlySize(ram_info.physical_total).c_str());

          ImGui::TreePop();
        }

        // Virtual
        if (ImGui::TreeNode("Virtual")) {
          ImGui::BulletText("%s: %s", ("Available"), SizeUtils::FriendlySize(ram_info.virtual_available).c_str());
          ImGui::BulletText("%s: %s", ("Total"), SizeUtils::FriendlySize(ram_info.virtual_total).c_str());

          ImGui::TreePop();
        }


        ImGui::TreePop();
      }
      /// RAM Information END ///


      /// GPU Information BEGIN /// Static
      if (ImGui::TreeNode(("GPU"))) {
        static const auto gpu_devices_info = iware::gpu::device_properties();
        if (gpu_devices_info.empty()) {
          ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ("Could not detect GPUs information"));
#define GPU_INFO_MINIMAL 1
#if GPU_INFO_MINIMAL
          // if infoware doesn't provide OpenGL implementation yet, make a minimal approach until then.
          ImGui::TextColored(Constants::Colors::ORANGE, ("GPUs Information not available yet,\n"
                                                         "here are some minimal info about your GPU\n"
                                                         "until this feature is fully implemented"));
          static const GLubyte *vendor = glGetString(GL_VENDOR);
          static const GLubyte *renderer = glGetString(GL_RENDERER);
          static const GLubyte *version = glGetString(GL_VERSION);
          ImGui::BulletText("%s: %s", ("Vendor"), vendor);
          ImGui::BulletText("%s: %s", ("Renderer"), renderer);
          ImGui::BulletText("%s: %s", ("Version"), version);
#endif
        } else {
          for (std::size_t i = 0; i < gpu_devices_info.size(); ++i) {
            const auto& gpu_device = gpu_devices_info[i];

            if (ImGui::TreeNode(fmt::format("Device #{}", i + 1).c_str())) // ("Device #" + std::to_string(i + 1)).c_str()))
            {
              // Vendor, Name, RAM Size, Cache Size, Max Frequency
              ImGui::BulletText("%s: %s", ("Vendor"), this->StringifyGPUVendorEnum(gpu_device.vendor));
              ImGui::BulletText("%s: %s", ("Name"), gpu_device.name.c_str());
              ImGui::BulletText("%s: %s", ("RAM Size"), SizeUtils::FriendlySize(gpu_device.memory_size).c_str());
              ImGui::BulletText("%s: %s", ("Cache Size"), SizeUtils::FriendlySize(gpu_device.cache_size).c_str());
              ImGui::BulletText("%s: %zu", ("Max Frequency"), gpu_device.max_frequency);

              ImGui::TreePop();
            }
          }
        }
        ImGui::TreePop();
      }
      /// GPU Information END ///


      /// OS Information BEGIN /// Static
      if (ImGui::TreeNode("Operating System")) {
        // Static OS info, unlike ram, needs to be initialized once.
        static const auto os_info = iware::system::OS_info();

        // Name, Full Name, Version
        ImGui::BulletText("%s: %s", ("Name"), os_info.name.c_str());
        ImGui::BulletText("%s: %s", ("Full name"), os_info.full_name.c_str());
        ImGui::BulletText("%s: %u.%u.%u Build %u", ("Version"), os_info.major, os_info.minor, os_info.patch, os_info.build_number);

        ImGui::TreePop();
      }
      /// OS Information END ///


      /// Kernel Information BEGIN /// Static
      if (ImGui::TreeNode("Kernel")) {
        // Static kernel info, unlike ram, needs to be initialized once.
        static const auto kernel_info = iware::system::kernel_info();

        // Name, Full Name, Version
        ImGui::BulletText("%s: %s", ("Variant"), this->StringifyKernelVariantEnum(kernel_info.variant));
        ImGui::BulletText("%s: %u.%u.%u Build %u", ("Version"), kernel_info.major, kernel_info.minor, kernel_info.patch, kernel_info.build_number);

        ImGui::TreePop();
      }
      /// Kernel Information END ///


      /// Connected HIDs Information BEGIN /// Volatile
      if (ImGui::TreeNode("Connected HIDs")) {
        // Mice, Keyboards, Other
        ImGui::BulletText("%s: %zu", ("Mice"), iware::system::mouse_amount());
        ImGui::BulletText("%s: %zu", ("Keyboards"), iware::system::keyboard_amount());
        ImGui::BulletText("%s: %zu", ("Other"), iware::system::other_HID_amount());

        ImGui::TreePop();
      }
      /// Connected HIDs END ///


      /// Displays Information BEGIN /// Volatile
      if (ImGui::TreeNode("Displays")) {
        const auto displays = iware::system::displays();
        if (displays.empty()) {
          ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ("No Displays Detected"));
        } else {
          for (const auto& display: displays) {
            // Resolution, DPI, Colour depth, Refresh rate
            ImGui::BulletText("%s: %ux%u", ("Resolution"), display.width, display.height);
            ImGui::BulletText("%s: %u", ("DPI"), display.dpi);
            ImGui::BulletText("%s: %u BPP (bits per pixel)", ("Color depth"), display.bpp);
            ImGui::BulletText("%s: %.2lfHz", ("Refresh rate"), display.refresh_rate);
          }
        }

        ImGui::TreePop();
      }
      /// Displays Information END ///


      /// Display Configurations BEGIN /// Volatile
      if (ImGui::TreeNode("Display Configurations")) {
        const auto configurations = iware::system::available_display_configurations();
        if (configurations.empty()) {
          ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ("No Display Configurations Detected"));
        } else {
          for (std::size_t i = 0; i < configurations.size(); i++) {
            const auto& display_configs = configurations[i];

            const std::string treenode_title = fmt::format("Display #{}", i + 1);

            if (ImGui::TreeNode(treenode_title.c_str())) {
              for (std::size_t j = 0; j < display_configs.size(); ++j) {
                const auto& config = display_configs[j];
                if (ImGui::TreeNode(std::to_string(j).c_str(), "%ux%u", config.width, config.height)) // Resolution
                {
                  ImGui::BulletText("%s:", ("Refresh rates"));
                  for (const double& rate: config.refresh_rates) {
                    ImGui::Text("\t\t%.2lfHz", rate);
                  }
                  ImGui::TreePop();
                }
              }

              ImGui::TreePop();
            }
          }
        }

        ImGui::TreePop();
      }

      /// Display Configurations END ///

      ImGui::PopFont();
    }
  }
  ImGui::PopFont();
}

void SystemAndHardwareInfoTool::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}
NS_ENIGMA_END
