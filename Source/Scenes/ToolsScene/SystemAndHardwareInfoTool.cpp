#include <pch.hpp>
#include "SystemAndHardwareInfoTool.hpp"
#include <Utility/SizeUtils.hpp>


NS_ENIGMA_BEGIN

void SystemAndHardwareInfoTool::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

}

void SystemAndHardwareInfoTool::OnDraw(Scene*)
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	//const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();

	//const auto button_size = Vec2f(win_w / 2.6f, 40.0f);

	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static auto& fonts = Application::GetInstance()->GetFonts();
	//static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	//static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_14 = fonts.at("Montserrat-Medium-14");
	//static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	ImGui::PushFont(font_audiowide_regular_20);
	{
		if (ImGui::CollapsingHeader(ENIGMA_TRANSLATE_CSTR("System and Hardware Information"), nullptr))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);

			/// CPU Information BEGIN /// Static
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("CPU")))
			{
				// Static cpu info, unlike ram, needs to be initialized once.
				static const iware::cpu::quantities_t quantities = iware::cpu::quantities();
				static const String architecture = this->StringifyArchitectureEnum(iware::cpu::architecture());
				static const ui64 frequency = iware::cpu::frequency();
				static const String endianness = iware::cpu::endianness() == iware::cpu::endianness_t::big ? "Big" : "Little";
				static const String model_name = iware::cpu::model_name();
				static const String vendor = iware::cpu::vendor();
				static const String vendor_id = iware::cpu::vendor_id();
				static std::vector<String> supported_instructions_set{}; // SSE, SSE2, AVX...
				static std::array<iware::cpu::cache_t, 3> caches{};
				// Will execute just once
				[[maybe_unused]] static const auto GetInstructionsSetAndCachesOnce = [this]() -> bool
				{
					// Instructions set
					for (const auto is : iware::cpu::supported_instruction_sets())
					{
						supported_instructions_set.push_back(this->StringifyInstructionSetEnum(is));
					}
					// Caches
					for (i32 i = 0; i < caches.size(); ++i)
					{
						caches[i] = iware::cpu::cache(i + 1);
					}

					return true;
				}();
				 
				// Architecture, Frequency, Model name, Vendor Name, Vendor ID
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Architecture"), architecture.c_str());
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Frequency"), frequency);
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Endianness"), endianness.c_str());
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Model name"), model_name.c_str());
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Vendor"), vendor.c_str());
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Vendor ID"), vendor_id.c_str());

				
				//spacing(2);

				// Quantities
				if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Quantities")))
				{
					ImGui::BulletText("%s: %u", ENIGMA_TRANSLATE_CSTR("Logical CPUs"), quantities.logical);
					ImGui::BulletText("%s: %u", ENIGMA_TRANSLATE_CSTR("Physical CPUs"), quantities.physical);
					ImGui::BulletText("%s: %u", ENIGMA_TRANSLATE_CSTR("CPU packages"), quantities.packages);

					ImGui::TreePop();
				}

				//spacing(2);

				// Caches
				if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Caches")))
				{
					for (size_t i = 0; i < caches.size(); ++i)
					{
						if (ImGui::TreeNode(std::to_string(i + 1).c_str()))
						{
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Size"), SizeUtils::FriendlySize(caches[i].size).c_str());
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Line Size"), SizeUtils::FriendlySize(caches[i].line_size).c_str());
							ImGui::BulletText("%s: %d", ENIGMA_TRANSLATE_CSTR("Associativity"),  static_cast<i32>(caches[i].associativity));
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Type"), this->StringifyCacheTypeEnum(caches[i].type));

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
					

				//spacing(2);

				// Supported Instruction Set
				if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Supported Instruction Set")))
				{
					for (const auto& is : supported_instructions_set)
					{
						ImGui::BulletText("%s", is.data());
					}
					ImGui::TreePop();
				}


				ImGui::TreePop();
			}
			/// CPU Information END ///


			/// RAM Information BEGIN /// Volatile
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("RAM")))
			{
				// Volatile ram info (will only execute if TreeNode is open, unlike cpu info which are static and preferrably initialized at first)
				const auto ram_info = iware::system::memory();
				
				// Physical
				if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Physical")))
				{
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Available"), SizeUtils::FriendlySize(ram_info.physical_available).c_str());
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Total"), SizeUtils::FriendlySize(ram_info.physical_total).c_str());

					ImGui::TreePop();
				}

				// Virtual
				if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Virtual")))
				{
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Available"), SizeUtils::FriendlySize(ram_info.virtual_available).c_str());
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Total"), SizeUtils::FriendlySize(ram_info.virtual_total).c_str());

					ImGui::TreePop();
				}
				

				ImGui::TreePop();
			}
			/// RAM Information END ///



			/// GPU Information BEGIN /// Static
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("GPU")))
			{
				static const auto gpu_devices_info = iware::gpu::device_properties();
				if(gpu_devices_info.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ENIGMA_TRANSLATE_CSTR("Could not detect GPUs information"));
#define GPU_INFO_MINIMAL 1
#if GPU_INFO_MINIMAL 
				    // if infoware doesnt provide OpenGL implementation yet, make a minimal approach until then.
					ImGui::TextColored(Constants::Colors::ORANGE, ENIGMA_TRANSLATE_CSTR("GPUs Information not yet available, here are some minimal info about your gpu until this feature is fully implemented"));
					static const GLubyte* vendor = glGetString(GL_VENDOR);
					static const GLubyte* renderer = glGetString(GL_RENDERER);
					static const GLubyte* version = glGetString(GL_VERSION);
					ImGui::BulletText("%s: % s", ENIGMA_TRANSLATE_CSTR("Vendor"), vendor);
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Renderer"), renderer);
					ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Version"), version);
#endif
				}
				else
				{
					for (size_t i = 0; i < gpu_devices_info.size(); ++i)
					{
						const auto& gpu_device = gpu_devices_info[i];

						if (ImGui::TreeNode(ENIGMA_TRANSLATE_FMT_CSTR("Device #{}", i + 1)))  //("Device #" + std::to_string(i + 1)).c_str()))
						{
							// Vendor, Name, RAM Size, Cache Size, Max Frequency
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Vendor"), this->StringifyGPUVendorEnum(gpu_device.vendor));
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Name"), gpu_device.name.c_str());
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("RAM Size"), SizeUtils::FriendlySize(gpu_device.memory_size).c_str());
							ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Cache Size"), SizeUtils::FriendlySize(gpu_device.cache_size).c_str());
							ImGui::BulletText("%s: %zu", ENIGMA_TRANSLATE_CSTR("Max Frequency"), gpu_device.max_frequency);

							ImGui::TreePop();
						}
					}
				}	
				ImGui::TreePop();
			}
			/// GPU Information END ///



			/// OS Information BEGIN /// Static
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Operating System")))
			{
				// Static os info, unlike ram, needs to be initialized once.
				static const auto os_info = iware::system::OS_info();

				// Name, Full Name, Version
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Name"), os_info.name.c_str());
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Full name"), os_info.full_name.c_str());
				ImGui::BulletText("%s: %u.%u.%u Build %u", ENIGMA_TRANSLATE_CSTR("Version"), os_info.major, os_info.minor, os_info.patch, os_info.build_number);
			
				ImGui::TreePop();
			}
			/// OS Information END ///



			/// Kernel Information BEGIN /// Static
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Kernel")))
			{
				// Static kernel info, unlike ram, needs to be initialized once.
				static const auto kernel_info = iware::system::kernel_info();

				// Name, Full Name, Version	
				ImGui::BulletText("%s: %s", ENIGMA_TRANSLATE_CSTR("Variant"), this->StringifyKernelVariantEnum(kernel_info.variant));
				ImGui::BulletText("%s: %u.%u.%u Build %u", ENIGMA_TRANSLATE_CSTR("Version"), kernel_info.major, kernel_info.minor, kernel_info.patch, kernel_info.build_number);

				ImGui::TreePop();
			}
			/// Kernel Information END ///


			/// Connected HIDs Information BEGIN /// Volatile
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Connected HIDs")))
			{
				// Mice, Keyboards, Other
				ImGui::BulletText("%s: %zu", ENIGMA_TRANSLATE_CSTR("Mice"), iware::system::mouse_amount());
				ImGui::BulletText("%s: %zu", ENIGMA_TRANSLATE_CSTR("Keyboards"), iware::system::keyboard_amount());
				ImGui::BulletText("%s: %zu", ENIGMA_TRANSLATE_CSTR("Other"), iware::system::other_HID_amount());

				ImGui::TreePop();
			}
			/// Connected HIDs END ///


			/// Displays Information BEGIN /// Volatile
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Displays")))
			{
				const auto displays = iware::system::displays();
				if (displays.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ENIGMA_TRANSLATE_CSTR("No Displays Detected"));
				}
				else
				{
					for (const auto& display : displays)
					{
						// Resolution, DPI, Colour depth, Refresh rate
						ImGui::BulletText("%s: %ux%u", ENIGMA_TRANSLATE_CSTR("Resolution"), display.width, display.height);
						ImGui::BulletText("%s: %u", ENIGMA_TRANSLATE_CSTR("DPI"), display.dpi);
						ImGui::BulletText("%s: %u BPP (bits per pixel)", ENIGMA_TRANSLATE_CSTR("Colour depth"), display.bpp);
						ImGui::BulletText("%s: %.2lfHz", ENIGMA_TRANSLATE_CSTR("Refresh rate"), display.refresh_rate);
					}
				}

				ImGui::TreePop();
			}
			/// Displays Information END ///


			/// Display Configurations BEGIN /// Volatile
			if (ImGui::TreeNode(ENIGMA_TRANSLATE_CSTR("Display Configurations")))
			{
				const auto configurations = iware::system::available_display_configurations();
				if (configurations.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, ENIGMA_TRANSLATE_CSTR("No Display Configurations Detected"));
				}
				else
				{
					for (size_t i = 0; i < configurations.size(); i++)
					{
						const auto& display_configs = configurations[i];

						const String treenode_title = ENIGMA_TRANSLATE_FMT("Display #{}", i + 1);

						if (ImGui::TreeNode(treenode_title.c_str()))
						{
							for (size_t j = 0; j < display_configs.size(); ++j)
							{
								const auto& config = display_configs[j];
								if (ImGui::TreeNode(std::to_string(j).c_str(), "%ux%u", config.width, config.height)) // Resolution
								{
									ImGui::BulletText("%s:", ENIGMA_TRANSLATE_FMT("Refresh rates"));
									for (const f64& rate : config.refresh_rates)
									{
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

void SystemAndHardwareInfoTool::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

}
NS_ENIGMA_END
