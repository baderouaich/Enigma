#pragma once
#ifndef ENIGMA_H
#define ENIGMA_H

//Core
#include <Enigma/Core/Core.hpp>
#include <Enigma/Application/Application.hpp>
//

//Window
#include <Enigma/Window/Window.hpp>
//


//System Dialogs, Notification
#include <Enigma/System/Dialogs/MessageBox.hpp>
#include <Enigma/System/Dialogs/OpenFileDialog.hpp>
#include <Enigma/System/Dialogs/SaveFileDialog.hpp>
#include <Enigma/System/Dialogs/SelectFolderDialog.hpp>
#include <Enigma/System/Notification/Notification.hpp>
//

//Events
#include <Enigma/Events/Event.hpp>
#include <Enigma/Events/EventDispatcher.hpp>
#include <Enigma/Events/ApplicationEvent.hpp>
#include <Enigma/Events/KeyEvent.hpp>
#include <Enigma/Events/MouseEvent.hpp>
#include <Enigma/Events/JoystickEvent.hpp>
//

//Input
#include <Enigma/Input/Input.hpp>
#include <Enigma/Input/Keyboard/KeyCodes.hpp>
#include <Enigma/Input/Mouse/MouseCodes.hpp>
//

//Debug, Logging 
#include <Enigma/Debug/Logger/Logger.hpp>
//

//Scene
#include <Enigma/Scene/Scene.hpp>
//


//GUI 
#include <imgui.h>
#include <Enigma/UI/ImGui/ImGuiRenderer.hpp>
//


//Utility 
#include <Enigma/Utility/Random.hpp>
#include <Enigma/Utility/Base64.hpp>
#include <Enigma/Utility/FileUtils.hpp>
#include <Enigma/Utility/StringUtils.hpp>
//


//Hardware RAM, CPU, GPU
#include <Enigma/Analytics/Hardware/RAM/RAMInfo.hpp>
//

#endif // !ENIGMA_H
