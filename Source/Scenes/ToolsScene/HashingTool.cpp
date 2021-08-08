#include <pch.hpp>
#include "HashingTool.hpp"
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/CryptoPPUtils.hpp> 
#include <Utility/DialogUtils.hpp> 

NS_ENIGMA_BEGIN
void HashingTool::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// dm Register Available Hashing Algorithms
	//m_hash_algorithms[HashAlgorithm::MD5] = std::make_unique<CryptoPP::MD5>();
	m_selected_hash = HashAlgorithm::MD5; // MD5 selected by default
}

void HashingTool::OnDraw(Scene* parent)
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();

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
		
		//if (ImGui::CollapsingHeader("Hashing", nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		if (ImGui::CollapsingHeader(ENIGMA_TRANSLATE_CSTR("Hashing"), nullptr))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);

			// Hashing algorithms
			ImGui::BulletText("%s:", ENIGMA_TRANSLATE_CSTR("Algorithms"));
			for (i32 i = static_cast<i32>(HashAlgorithm::BEGIN); i <= static_cast<i32>(HashAlgorithm::END); ++i)
			{
				if (ImGui::RadioButton(*static_cast<HashAlgorithm>(i), m_selected_hash == static_cast<HashAlgorithm>(i)))
				{
					m_selected_hash = static_cast<HashAlgorithm>(i);
				}
				// Add new line after each hash kind row
				switch (static_cast<HashAlgorithm>(i))
				{
				case Enigma::HashingTool::HashAlgorithm::MD5:
				case Enigma::HashingTool::HashAlgorithm::SHA512:
				case Enigma::HashingTool::HashAlgorithm::SHAKE256:
				case Enigma::HashingTool::HashAlgorithm::KECCAK512:
				case Enigma::HashingTool::HashAlgorithm::WHIRLPOOL:
					//ImGui::NewLine(); // not calling ImGui::SameLine(); will go to next line, if called newline extra padding appears
					break;
				default: // else, stay at the same line
					ImGui::SameLine();

				}
			}

			spacing(2);

			// Input text to calculate hash for
			ImGui::BulletText("%s:", ENIGMA_TRANSLATE_CSTR("Input"));
			ImGuiWidgets::InputTextMultiline("##input1", &m_input, ImVec2(win_w * 0.85f, 60.0f));
			ImGui::SameLine();
			if (ImGuiWidgets::Button(ENIGMA_TRANSLATE_CSTR("Calculate"), Vec2f(-1.0f, 33.0f), Constants::Colors::TOOLS_BUTTON_COLOR, Constants::Colors::TOOLS_BUTTON_COLOR_HOVER, Constants::Colors::TOOLS_BUTTON_COLOR_ACTIVE))
			{
				Application::GetInstance()->LaunchWorkerThread(parent, ENIGMA_TRANSLATE_CSTR("Calculating Hash..."), [this]()
					{
						this->OnCalculateHashButtonPressed();
					});
			}

			spacing(2);


			// Output calculated hash
			if (!m_output.empty())
			{
				ImGui::BulletText("%s:", ENIGMA_TRANSLATE_CSTR("Output (Hex Encoded)"));
				ImGuiWidgets::InputTextMultiline("##output1", &m_output, Vec2f(win_w * 0.85f, 60.0f), ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				ImGui::PushID("CopyCalculatedHash");
				if (ImGuiWidgets::Button(ENIGMA_TRANSLATE_CSTR("Copy"), Vec2f(-1, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE))
				{
					this->OnCopyHashButtonPressed();
				}
				ImGui::PopID();
			}


			ImGui::PopFont();
		}
	}
	ImGui::PopFont();
}

void HashingTool::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}
void HashingTool::OnCalculateHashButtonPressed()
{
	if (m_input.empty()) return;
	m_output.clear();
	try
	{
		switch (m_selected_hash)
		{
#define CASE_PERFORM_HASH(var, hash_algo, cryptopp_hash_name) \
			case Enigma::HashingTool::HashAlgorithm::hash_algo: \
			{ \
				if (!var) var = std::make_unique<CryptoPP::cryptopp_hash_name>(); \
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true, new CryptoPP::HashFilter(*var, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))); \
				break; \
			}

			CASE_PERFORM_HASH(m_md2, MD2, MD2);
			CASE_PERFORM_HASH(m_md4, MD4, MD4);
			CASE_PERFORM_HASH(m_md5, MD5, MD5);

			CASE_PERFORM_HASH(m_sha1, SHA1, SHA1);
			CASE_PERFORM_HASH(m_sha224, SHA224, SHA224);
			CASE_PERFORM_HASH(m_sha256, SHA256, SHA256);
			CASE_PERFORM_HASH(m_sha384, SHA384, SHA384);
			CASE_PERFORM_HASH(m_sha512, SHA512, SHA512);

			CASE_PERFORM_HASH(m_shake128, SHAKE128, SHAKE128);
			CASE_PERFORM_HASH(m_shake256, SHAKE256, SHAKE256);

			CASE_PERFORM_HASH(m_keccak224, KECCAK224, Keccak_224);
			CASE_PERFORM_HASH(m_keccak256, KECCAK256, Keccak_256);
			CASE_PERFORM_HASH(m_keccak384, KECCAK384, Keccak_384);
			CASE_PERFORM_HASH(m_keccak512, KECCAK512, Keccak_512);

			CASE_PERFORM_HASH(m_tiger, TIGER, Tiger);
			CASE_PERFORM_HASH(m_whirlpool, WHIRLPOOL, Whirlpool);


#undef CASE_PERFORM_HASH

#if 0
			case Enigma::HashingTool::HashAlgorithm::MD2:
			{
				
				if (!m_md2) m_md2 = std::make_unique<CryptoPP::MD2>();
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_md2, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params

				break;
			}
			case Enigma::HashingTool::HashAlgorithm::MD4:
			{
				if (!m_md4) m_md4 = std::make_unique<CryptoPP::MD4>();
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_md4, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params

				break;
			}
			case Enigma::HashingTool::HashAlgorithm::MD5:
			{
				if(!m_md5) m_md5 = std::make_unique<CryptoPP::MD5>();
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
						new CryptoPP::HashFilter(*m_md5, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
					) ;// Note: CryptoPP will auto delete heap allocated passed params
	
				//m_md5->Update(reinterpret_cast<const byte*>(m_input.data()), m_input.size());
				//m_output.resize(m_md5->DigestSize());
				//m_md5->Final(reinterpret_cast<byte*>(m_output.data()));
				//CryptoPP::StringSource ss(m_output, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)));
			    //CryptoPP::StringSource ss(m_output, true, new CryptoPP::Redirector(encoder));

				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHA1:
			{
				if(!m_sha1) m_sha1.reset(new CryptoPP::SHA1());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_sha1, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHA224:
			{
				if(!m_sha224) m_sha224.reset(new CryptoPP::SHA224());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_sha224, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHA256:
			{
				if(!m_sha256) m_sha256.reset(new CryptoPP::SHA256());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_sha256, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHA384:
			{
				if(!m_sha384) m_sha384.reset(new CryptoPP::SHA384());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_sha384, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHA512:
			{
				if(!m_sha512) m_sha512.reset(new CryptoPP::SHA512());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_sha512, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHAKE128:
			{
				if(!m_shake128) m_shake128.reset(new CryptoPP::SHAKE128());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_shake128, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::SHAKE256:
			{
				if(!m_shake256) m_shake256.reset(new CryptoPP::SHAKE256());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_shake256, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::KECCAK224:
			{
				if (!m_keccak224) m_keccak224.reset(new CryptoPP::Keccak_224());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_keccak224, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::KECCAK256:
			{
				if (m_keccak256) m_keccak256.reset(new CryptoPP::Keccak_256());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_keccak256, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::KECCAK384:
			{
				if (!m_keccak384) m_keccak384.reset(new CryptoPP::Keccak_384());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_keccak384, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
			case Enigma::HashingTool::HashAlgorithm::KECCAK512:
			{
				if (!m_keccak512) m_keccak512.reset(new CryptoPP::Keccak_512());
				[[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true,
					new CryptoPP::HashFilter(*m_keccak512, new  CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output)))
				);// Note: CryptoPP will auto delete heap allocated passed params
				break;
			}
#endif
		default:
			return;
		}
	}
	catch (const CryptoPP::Exception& e)
	{
		const String err_msg = ENIGMA_TRANSLATE("Hashing Failure") + ": " + CryptoPPUtils::GetFullErrorMessage(e);
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
	catch (const std::exception& e)
	{
		const String err_msg = ENIGMA_TRANSLATE("Hashing Failure") + ": " + String(e.what());
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
	catch (...)
	{
		const String err_msg = ENIGMA_TRANSLATE("Hashing Failure UNKNOWN ERROR");
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
}

void HashingTool::OnCopyHashButtonPressed()
{
	if (!m_output.empty())
		Clipboard::Set(m_output);
}

NS_ENIGMA_END
