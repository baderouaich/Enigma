#pragma once
#include "Scene.hpp"
#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha/ChaCha.hpp>
using namespace Enigma;

class EncryptTextScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit EncryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~EncryptTextScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnEncryptButtonPressed();
	void OnCancelButtonPressed();


private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	struct Error
	{
		ImVec2 position;
		String msg;
		float timer{0.0f};
	};
	void DrawErrors() 
	{
		const auto& dt = Application::GetInstance().GetDeltaTime();
		if (m_errors.empty()) return;

		float x = 100.0f;
		while (!m_errors.empty())
		{
			auto& err = m_errors.back();
			ImGui::SetCursorPosX(x);
			ImGui::Text("Error: %s", err.msg.c_str());
			err.timer += dt;
			if (err.timer >= 5.0f) // 5secs
			{
				m_errors.pop();
			}
			x += 100.0f;
		}
	}
	std::queue<Error> m_errors;

private:
	Algorithm::Type m_algorithm; // selected algorithm to encrypt text with
	String m_text; // Text to encrypt
	String m_password; // Encryption password
};



