#pragma once
#ifndef ENIGMA_H
#define ENIGMA_H

//Core
#include <Core/Core.hpp>
#include <Application/Application.hpp>
//

//Window
#include <Window/Window.hpp>
//


//System Dialogs, Notification
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <System/Notification/Notification.hpp>
//

//Events
#include <Events/Event.hpp>
#include <Events/EventDispatcher.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/JoystickEvent.hpp>
//

//Input
#include <Input/Input.hpp>
#include <Input/Keyboard/KeyCodes.hpp>
#include <Input/Mouse/MouseCodes.hpp>
//

//Logging 
#include <Logger/Logger.hpp>
//


//GUI 
#include <imgui.h>
#include <UI/ImGui/ImGuiRenderer.hpp>
//

//Scene
#include <Scenes/Scene.hpp>
//

//Utility 
#include <Utility/Random.hpp>
#include <Utility/Base64.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/StringUtils.hpp>
#include <Utility/OpenGLUtils.hpp>
//


//Analytocs: Hardware usage RAM, CPU, GPU, Profiler
#include <Analytics/Hardware/RAM/RAMInfo.hpp>
//


#endif // !ENIGMA_H
