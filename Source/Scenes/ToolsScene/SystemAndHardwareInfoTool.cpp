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
		if (ImGui::CollapsingHeader("System and Hardware Information", nullptr))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);

			/// CPU Information BEGIN /// Static
			if (ImGui::TreeNode("CPU"))
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
				ImGui::BulletText("Architecture: %s", architecture.c_str());
				ImGui::BulletText("Frequency: %lu Hz", frequency);
				ImGui::BulletText("Endianness: %s", endianness.c_str());
				ImGui::BulletText("Model name: %s", model_name.c_str());
				ImGui::BulletText("Vendor: %s", vendor.c_str());
				ImGui::BulletText("Vendor ID: %s", vendor_id.c_str());

				
				//spacing(2);

				// Quantities
				if (ImGui::TreeNode("Quantities"))
				{
					ImGui::BulletText("Logical CPUs: %u",  quantities.logical);
					ImGui::BulletText("Physical CPUs: %u", quantities.physical);
					ImGui::BulletText("CPU packages: %u",  quantities.packages);

					ImGui::TreePop();
				}

				//spacing(2);

				// Caches
				if (ImGui::TreeNode("Caches"))
				{
					for (size_t i = 0; i < caches.size(); ++i)
					{
						if (ImGui::TreeNode(std::to_string(i + 1).c_str()))
						{
							ImGui::BulletText("Size: %s", SizeUtils::FriendlySize(caches[i].size).c_str());
							ImGui::BulletText("Line Size: %s", SizeUtils::FriendlySize(caches[i].line_size).c_str());
							ImGui::BulletText("Associativity: %d", static_cast<i32>(caches[i].associativity));
							ImGui::BulletText("Type: %s", this->StringifyCacheTypeEnum(caches[i].type));

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
					

				//spacing(2);

				// Supported Instruction Set
				if (ImGui::TreeNode("Supported Instruction Set"))
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
			if (ImGui::TreeNode("RAM"))
			{
				// Volatile ram info (will only execute if TreeNode is open, unlike cpu info which are static and preferrably initialized at first)
				const auto ram_info = iware::system::memory();
				
				// Physical
				if (ImGui::TreeNode("Physical"))
				{
					ImGui::BulletText("Available: %s", SizeUtils::FriendlySize(ram_info.physical_available).c_str());
					ImGui::BulletText("Total: %s", SizeUtils::FriendlySize(ram_info.physical_total).c_str());

					ImGui::TreePop();
				}

				// Virtual
				if (ImGui::TreeNode("Virtual"))
				{
					ImGui::BulletText("Available: %s", SizeUtils::FriendlySize(ram_info.virtual_available).c_str());
					ImGui::BulletText("Total: %s", SizeUtils::FriendlySize(ram_info.virtual_total).c_str());

					ImGui::TreePop();
				}
				

				ImGui::TreePop();
			}
			/// RAM Information END ///



			/// GPU Information BEGIN /// Static
			if (ImGui::TreeNode("GPU"))
			{
				static const auto gpu_devices_info = iware::gpu::device_properties();
				if(gpu_devices_info.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, "Could not detect GPUs information");
#define GPU_INFO_MINIMAL 1
#if GPU_INFO_MINIMAL 
				    // if infoware doesnt provide OpenGL implementation yet, make a minimal approach until then.
					ImGui::TextColored(Constants::Colors::ORANGE, "GPUs Information not yet available, here are some minimal info about your gpu until this feature is fully implemented:");
					static const GLubyte* vendor = glGetString(GL_VENDOR);
					static const GLubyte* renderer = glGetString(GL_RENDERER);
					static const GLubyte* version = glGetString(GL_VERSION);
					ImGui::BulletText("Vendor: %s", vendor);
					ImGui::BulletText("Renderer: %s", renderer);
					ImGui::BulletText("Version: %s", version);
#endif
				}
				else
				{
					for (size_t i = 0; i < gpu_devices_info.size(); ++i)
					{
						const auto& gpu_device = gpu_devices_info[i];

						if (ImGui::TreeNode(("Device #" + std::to_string(i + 1)).c_str()))
						{
							// Vendor, Name, RAM Size, Cache Size, Max Frequency
							ImGui::BulletText("Vendor: %s", this->StringifyGPUVendorEnum(gpu_device.vendor));
							ImGui::BulletText("Name: %s", gpu_device.name.c_str());
							ImGui::BulletText("RAM Size: %s", SizeUtils::FriendlySize(gpu_device.memory_size).c_str());
							ImGui::BulletText("Cache Size: %s", SizeUtils::FriendlySize(gpu_device.cache_size).c_str());
							ImGui::BulletText("Max Frequency: %zu", gpu_device.max_frequency);

							ImGui::TreePop();
						}
					}
				}	
				ImGui::TreePop();
			}
			/// GPU Information END ///



			/// OS Information BEGIN /// Static
			if (ImGui::TreeNode("Operating System"))
			{
				// Static os info, unlike ram, needs to be initialized once.
				static const auto os_info = iware::system::OS_info();

				// Name, Full Name, Version
				ImGui::BulletText("Name: %s", os_info.name.c_str());
				ImGui::BulletText("Full name: %s", os_info.full_name.c_str());
				ImGui::BulletText("Version: %u.%u.%u Build %u", os_info.major, os_info.minor, os_info.patch, os_info.build_number);
			
				ImGui::TreePop();
			}
			/// OS Information END ///



			/// Kernel Information BEGIN /// Static
			if (ImGui::TreeNode("Kernel"))
			{
				// Static kernel info, unlike ram, needs to be initialized once.
				static const auto kernel_info = iware::system::kernel_info();

				// Name, Full Name, Version	
				ImGui::BulletText("Variant: %s", this->StringifyKernelVariantEnum(kernel_info.variant));
				ImGui::BulletText("Version: %u.%u.%u Build %u", kernel_info.major, kernel_info.minor, kernel_info.patch, kernel_info.build_number);

				ImGui::TreePop();
			}
			/// Kernel Information END ///


			/// Connected HIDs Information BEGIN /// Volatile
			if (ImGui::TreeNode("Connected HIDs"))
			{
				// Mice, Keyboards, Other
				ImGui::BulletText("Mice: %zu", iware::system::mouse_amount());
				ImGui::BulletText("Keyboards: %zu", iware::system::keyboard_amount());
				ImGui::BulletText("Other: %zu", iware::system::other_HID_amount());

				ImGui::TreePop();
			}
			/// Connected HIDs END ///


			/// Displays Information BEGIN /// Volatile
			if (ImGui::TreeNode("Displays"))
			{
				const auto displays = iware::system::displays();
				if (displays.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, "No Displays Detected");
				}
				else
				{
					for (const auto& display : displays)
					{
						// Resolution, DPI, Colour depth, Refresh rate
						ImGui::BulletText("Resolution: %ux%u", display.width, display.height);
						ImGui::BulletText("DPI: %u", display.dpi);
						ImGui::BulletText("Colour depth: %u BPP (bits per pixel)", display.bpp);
						ImGui::BulletText("Refresh rate: %.2lfHz", display.refresh_rate);
					}
				}

				ImGui::TreePop();
			}
			/// Displays Information END ///


			/// Display Configurations BEGIN /// Volatile
			if (ImGui::TreeNode("Display Configurations"))
			{
				const auto configurations = iware::system::available_display_configurations();
				if (configurations.empty())
				{
					ImGui::TextColored(Constants::Colors::ERROR_TEXT_COLOR, "No Display Configurations Detected");
				}
				else
				{
					for (size_t i = 0; i < configurations.size(); i++)
					{
						const auto& display_configs = configurations[i];

						const String treenode_title = "Display #" + std::to_string(i + 1);
						if (ImGui::TreeNode(treenode_title.c_str()))
						{
							for (size_t j = 0; j < display_configs.size(); ++j)
							{
								const auto& config = display_configs[j];
								if (ImGui::TreeNode(std::to_string(j).c_str(), "%ux%u", config.width, config.height)) // Resolution
								{
									ImGui::BulletText("Refresh rates:");
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
