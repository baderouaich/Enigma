#pragma once
#include "Scene.hpp"

NS_ENIGMA_BEGIN

class MainMenuScene : public Enigma::Scene {
  public: /* Constructors / Destructor */
    MainMenuScene() noexcept;
    virtual ~MainMenuScene() = default;

  private: /* Overrides */
    void OnCreate() override;
    [[maybe_unused]] void OnUpdate(const float&) override;
    void OnDraw() override;
    void OnImGuiDraw() override;
    [[maybe_unused]] void OnEvent(Enigma::Event&) override;
    void OnDestroy() override;

  private: /* Callbacks */
    // Buttons
    void OnEncryptFileButtonPressed();
    void OnDecryptFileButtonPressed();
    void OnEncryptTextButtonPressed();
    void OnDecryptTextButtonPressed();
    void OnMyEncryptionsButtonPressed();
    void OnToolsButtonPressed();
    // Menu
    void OnReportIssueMenuButtonPressed();
    void OnCheckForUpdatesMenuButtonPressed();
    void OnAboutMenuButtonPressed();

    void OnExportEncryptionsButtonPressed();
    void OnImportEncryptionsButtonPressed();

  private:
    //std::unique_ptr<ImGuiWidgets::Image> m_logo{};
};

NS_ENIGMA_END
