#include "pch.hpp"
#include "MainScene.hpp"

MainScene::MainScene()
	:
	Enigma::Scene()
{

}

void MainScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Initialize RAMInfo
	{
		m_ram_info = MakeUnique<RAMInfo>();
	}

	glClearColor(0, 0, 0, 0);
}

void MainScene::OnUpdate(const f32& dt)
{
	m_ram_info->Update();
}

void MainScene::OnDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void MainScene::OnImGuiDraw()
{

	{
		static bool open = true;
		static ImVec2 size = { 300.0f, 150.0f };
		ImGui::Begin("Delta time Test", &open);
		ImGui::SetWindowSize(size);
		{
			ImGui::Text("Delta time:");
			ImGui::NewLine();
			ImGui::Text("%f", Application::GetInstance().GetDeltaTime());
			ImGui::NewLine();
			if (ImGui::Button("Pause for 1 sec"))
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		ImGui::End();
	}

	{
#define MB / 1024.0f / 1024.0f
		static bool open = true;
		static ImVec2 size = { 300.0f, 150.0f };
		ImGui::Begin("Hardware Test: RAM Info", &open);
		ImGui::SetWindowSize(size);
		{
			ImGui::Text("Available Memory: %.2f MB", m_ram_info->GetAvailableRAM() MB);
			ImGui::Text("Used Memory: %.2f MB", m_ram_info->GetUsedRAM() MB);
			ImGui::Text("Free Memory: %.2f MB", m_ram_info->GetFreeRAM() MB);
		}
		ImGui::End();
	}

	{

		static bool open = true;
		ImGui::Begin("MessageBox Test", &open);
		ImGui::SetWindowSize({ 300.0f, 150.0f });
		{
			if (ImGui::Button("Spawn Dialog"))
			{
				MessageBox msg_box(
					"Title",
					"Description",
					MessageBox::Icon::Warning,
					MessageBox::Choice::Ok_Cancel);
				MessageBox::Action action = msg_box.Show();
				switch (action)
				{
				case MessageBox::Action::Ok:
					LOG("Ok");
					break;
				case MessageBox::Action::Cancel:
					LOG("Cancel");
					break;
				default:
					LOG("UNKNOWN");
				}

			}

		}
		ImGui::End();
	}

	{

		static bool open = true;
		ImGui::Begin("OpenFileDialog Test", &open);
		ImGui::SetWindowSize({ 300.0f, 150.0f });
		{
			if (ImGui::Button("Spawn Dialog"))
			{
				OpenFileDialog dialog(
					"Enigma asks you to select a file or more",
					".",
					true,
					{ "All Files", "*" }
				);
				std::vector<String> selected_files = dialog.Show();
				for (const String& filename : selected_files)
				{
					LOG("Selected File: {0}", filename.c_str());
				}
			}

		}
		ImGui::End();
	}

	{

		static bool open = true;
		ImGui::Begin("SaveFileDialog Test", &open);
		ImGui::SetWindowSize({ 300.0f, 150.0f });
		{
			if (ImGui::Button("Spawn Dialog"))
			{
				SaveFileDialog dialog(
					"Enigma asks you to select a path to save the file to",
					".",
					true,
					{ "All Files", "*" }
				);
				String selected_path = dialog.Show();
				LOG("Selected Path: {0}", selected_path.c_str());

			}

		}
		ImGui::End();
	}

	{

		static bool open = true;
		ImGui::Begin("SelectFolderDialog Test", &open);
		ImGui::SetWindowSize({ 300.0f, 90.0f });
		{
			if (ImGui::Button("Spawn Dialog"))
			{
				SelectFolderDialog dialog(
					"Enigma asks you to select a folder from disk",
					".",
					true);
				String selected_folder = dialog.Show();
				LOG("Selected Folder: {0}", selected_folder.c_str());
			}

		}
		ImGui::End();
	}

	{

		static bool open = true;
		ImGui::Begin("Notification Test", &open);
		ImGui::SetWindowSize({ 300.0f, 90.0f });
		{
			if (ImGui::Button("Spawn Notification"))
			{
				Notification notification(
					"Title",
					"Description",
					Notification::Icon::Info);
				notification.Show();
			}

		}
		ImGui::End();
	}



#if USING_SOUND
	if (sound->IsValid())
	{
		const auto [w, h] = Application::GetInstance().GetWindow()->GetSize();
		static bool open = true;
		static const String title = "Sound testing";
		static const ImVec2 size(250, 200);
		ImGui::Begin(title.c_str(), &open);
		ImGui::SetWindowSize(size);
		ImGui::SetWindowPos(ImVec2(w / 2.0f, h / 2.0f));
		if (sound->IsPaused())
			if (ImGui::Button("Play"))
			{
				sound->Play();
			}
		ImGui::Spacing();
		if (!sound->IsPaused())
			if (ImGui::Button("Pause"))
			{
				sound->Pause();
			}
		ImGui::Spacing();
		if (sound->IsPaused())
			if (ImGui::Button("Resume"))
			{
				sound->Resume();
			}
		ImGui::Spacing();
		if (ImGui::Button("Stop"))
		{
			sound->Stop();
		}
		ImGui::Spacing();
		if (ImGui::SliderFloat("Volume", &volume, 0.0f, 2.0f))
		{
			sound->SetVolume(volume);
		}
		ImGui::Spacing();
		if (ImGui::Button("Stop All"))
		{
			Sound::StopAll();
		}
		ImGui::End();
	}

#endif

#if 0
	ImGui::Begin("Scene Test");
	if (ImGui::Button("NEW SCENE"))
	{
		SecondScene* scene = new SecondScene();
		__super::PushScene(scene);
	}
	if (ImGui::Button("END SCENE"))
	{
		__super::EndScene();
	}
	ImGui::SameLine(); ImGui::Text("Push new scene");
	ImGui::End();
#endif

}

void MainScene::OnEvent(Event& event)
{
	//Listening to specific event
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& e) -> bool
		{
			LOG(e.ToString().c_str());

			return true; // handled, return false if other scenes may use the event
		});

	dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& e) -> bool
		{
			LOG(e.ToString().c_str());

			return true; // handled, return false if other scenes may use the event
		});

	if (Input::IsKeyPressed(Enigma::Key::Escape))
	{
		__super::EndScene();
	}

};

void MainScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}