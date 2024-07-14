#include <pch.hpp>
#include "HashingTool.hpp"
#include <Application/Application.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/CryptoPPUtils.hpp>
#include <Utility/DialogUtils.hpp>

NS_ENIGMA_BEGIN
void HashingTool::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  m_selected_hash = HashAlgorithm::MD5; // MD5 selected by default
}

void HashingTool::OnDraw(Scene *parent) {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();

  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };
  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };

  static auto& fonts = Application::getInstance()->GetFonts();
  //static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
  //static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
  static ImFont *const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
  static ImFont *const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
  static ImFont *const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
  static ImFont *const& font_montserrat_medium_14 = fonts.at("Montserrat-Medium-14");
  //static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

  ImGui::PushFont(font_audiowide_regular_20);
  {

    //if (ImGui::CollapsingHeader("Hashing", nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
    if (ImGui::CollapsingHeader(("Hashing"), nullptr)) {
      ImGui::PushFont(font_montserrat_medium_18);
      spacing(3);

      // Hashing algorithms
      ImGui::BulletText("%s:", ("Algorithms"));
      for (std::int32_t i = static_cast<std::int32_t>(HashAlgorithm::BEGIN); i <= static_cast<std::int32_t>(HashAlgorithm::END); ++i) {
        if (ImGui::RadioButton(*static_cast<HashAlgorithm>(i), m_selected_hash == static_cast<HashAlgorithm>(i))) {
          m_selected_hash = static_cast<HashAlgorithm>(i);
        }
        // Add new line after each hash kind row
        switch (static_cast<HashAlgorithm>(i)) {
          case Enigma::HashingTool::HashAlgorithm::MD5:
          case Enigma::HashingTool::HashAlgorithm::SHA512:
          case Enigma::HashingTool::HashAlgorithm::SHAKE256:
          case Enigma::HashingTool::HashAlgorithm::KECCAK512:
          case Enigma::HashingTool::HashAlgorithm::RIPEMD320:
          case Enigma::HashingTool::HashAlgorithm::END:
            //ImGui::NewLine(); // not calling ImGui::SameLine(); will go to next line, if called newline extra padding appears
            break;
          default: // else, stay at the same line
            ImGui::SameLine();
        }
      }

      spacing(2);


      // Input source (text, file)
      ImGui::BulletText("%s:", ("Input Source"));
      ImGui::NewLine();
      for (byte s = (byte) InputSource::BEGIN; s <= (byte) InputSource::END; ++s) {
        inline_dummy(1.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::RadioButton(inputSourceEnumToStr((InputSource) s), m_input_source == (InputSource) s)) {
          m_input_source = (InputSource) s;
        }
      }

      spacing(2);

      // Input text or file to calculate hash for
      switch (m_input_source) {
        case InputSource::Text: {
          ImGui::BulletText("%s:", "Text");
          ImGuiWidgets::InputTextMultiline("##input1", &m_input, ImVec2(win_w * 0.85f, 256.0f));
          ImGui::SameLine();
          break;
        }
        case InputSource::File: {
          ImGui::BulletText("%s:", "File");
          ImGuiWidgets::InputTextMultiline("##input1", &m_input, ImVec2(win_w * 0.85f, 33.0f));
          ImGui::SameLine();
          static const ImVec2 browse_button_size(-1.0f, 33.0f);
          ImGui::SameLine();
          ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
          ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
          ImGui::PushID("Browse1");
          if (ImGui::Button("Browse", browse_button_size)) {
            Application::getInstance()->LaunchWorkerThread(parent, "Browsing input file...", [this]() -> void {
              if (std::vector<std::string> filenames = OpenFileDialog{"Select a file to compute its hash", ".", false}.Show(); !filenames.empty()) {
                m_input = filenames[0];
              }
            });
          }
          ImGui::PopID();
          ImGui::PopStyleColor(3);
          break;
        }
        default:
          ENIGMA_ASSERT(false, "Unhandled input source");
      }
      if (ImGuiWidgets::Button(("Calculate"), ImVec2(-1.0f, 33.0f), Constants::Colors::TOOLS_BUTTON_COLOR, Constants::Colors::TOOLS_BUTTON_COLOR_HOVER, Constants::Colors::TOOLS_BUTTON_COLOR_ACTIVE)) {
        Application::getInstance()->LaunchWorkerThread(parent, ("Calculating Hash..."), [this]() {
          this->OnCalculateHashButtonPressed();
        });
      }


      spacing(2);


      // Output calculated hash
      if (!m_output.empty()) {
        ImGui::BulletText("%s:", ("Output (Hex Encoded)"));
        ImGuiWidgets::InputTextMultiline("##output1", &m_output, ImVec2(win_w * 0.85f, 60.0f), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        ImGui::PushID("CopyCalculatedHash");
        if (ImGuiWidgets::Button(("Copy"), ImVec2(-1, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE)) {
          this->OnCopyHashButtonPressed();
        }
        ImGui::PopID();
      }


      ImGui::PopFont();
    }
  }
  ImGui::PopFont();
}

void HashingTool::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}
void HashingTool::OnCalculateHashButtonPressed() {
  if (m_input.empty()) return;
  m_output.clear();

  try {
    switch (m_selected_hash) {

#define CASE_PERFORM_HASH(var, hash_algo, cryptopp_hash_name)                                                                                                                              \
  case Enigma::HashingTool::HashAlgorithm::hash_algo: {                                                                                                                                    \
    if (!var) var = std::make_unique<CryptoPP::cryptopp_hash_name>();                                                                                                                      \
    switch (m_input_source) {                                                                                                                                                              \
      case InputSource::Text: {                                                                                                                                                            \
        [[maybe_unused]] const auto ss = CryptoPP::StringSource(m_input, true, new CryptoPP::HashFilter(*var, new CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output), false)));       \
        break;                                                                                                                                                                             \
      }                                                                                                                                                                                    \
      case InputSource::File: {                                                                                                                                                            \
        ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(m_input), "No such file found");                                                                                                        \
        [[maybe_unused]] const auto ss = CryptoPP::FileSource(m_input.c_str(), true, new CryptoPP::HashFilter(*var, new CryptoPP::HexEncoder(new CryptoPP::StringSink(m_output), false))); \
        break;                                                                                                                                                                             \
      }                                                                                                                                                                                    \
      default: {                                                                                                                                                                           \
        ENIGMA_ASSERT(false, "Unhandled InputSource");                                                                                                                                     \
        break;                                                                                                                                                                             \
      };                                                                                                                                                                                   \
    }                                                                                                                                                                                      \
    break;                                                                                                                                                                                 \
  }


      CASE_PERFORM_HASH(m_md2, MD2, Weak::MD2);
      CASE_PERFORM_HASH(m_md4, MD4, Weak::MD4);
      CASE_PERFORM_HASH(m_md5, MD5, Weak::MD5);

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

      CASE_PERFORM_HASH(m_ripemd128, RIPEMD128, RIPEMD128);
      CASE_PERFORM_HASH(m_ripemd160, RIPEMD160, RIPEMD160);
      CASE_PERFORM_HASH(m_ripemd256, RIPEMD256, RIPEMD256);
      CASE_PERFORM_HASH(m_ripemd320, RIPEMD320, RIPEMD320);

      CASE_PERFORM_HASH(m_tiger, TIGER, Tiger);
      CASE_PERFORM_HASH(m_whirlpool, WHIRLPOOL, Whirlpool);
      CASE_PERFORM_HASH(m_sm3, SM3, SM3);

#undef CASE_PERFORM_HASH

      default:
        return;
    }
  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = ("Hashing Failure: ") + CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  } catch (const std::exception& e) {
    const std::string err_msg = ("Hashing Failure: ") + std::string(e.what());
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  } catch (...) {
    const std::string err_msg = ("Hashing Failure UNKNOWN ERROR");
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  }
}

void HashingTool::OnCopyHashButtonPressed() {
  if (!m_output.empty())
    Clipboard::Set(m_output);
}

NS_ENIGMA_END
