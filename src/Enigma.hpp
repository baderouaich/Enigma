#pragma once
#ifndef ENIGMA_H
#define ENIGMA_H

// Core
#include <Application/Application.hpp>
#include <Core/Core.hpp>
//

// Window
#include <Window/Window.hpp>
//

// Enigma Config Loader
#include <Config/Config.hpp>
//


// System Dialogs, Notification, Clipboard, Environment Variable
#include <System/Clipboard/Clipboard.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <System/ENV/ENV.hpp>
#include <System/Notification/Notification.hpp>
//

// Events
#include <Events/ApplicationEvent.hpp>
#include <Events/Event.hpp>
#include <Events/EventDispatcher.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
//

// Input
#include <Input/Input.hpp>
#include <Input/Keyboard/KeyCodes.hpp>
#include <Input/Mouse/MouseCodes.hpp>
//

// Logging
#include <Logger/Logger.hpp>
//

// GUI
#include <GUI/ImGuiRenderer.hpp>
#include <GUI/ImGuiWidgets.hpp>
//

// Scene
#include <Scenes/Scene.hpp>
//

// Utility
#include <Utility/Base64.hpp>
#include <Utility/CryptoPPUtils.hpp>
#include <Utility/DialogUtils.hpp>
#include <Utility/FileUtils.hpp>
//#include <Utility/GZip.hpp>
#include <Utility/FinalAction.hpp>
#include <Utility/HashUtils.hpp>
#include <Utility/MathUtils.hpp>
#include <Utility/OpenGLUtils.hpp>
#include <Utility/Random.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/StringUtils.hpp>
//

// Analytics: Hardware info RAM, CPU
#include <Analytics/Hardware/CPU/CPUInfo.hpp>
#include <Analytics/Hardware/RAM/RAMInfo.hpp>
//

// Enigma Command line interface
//#include <CLI/CLI.hpp>
//

// Networking
#include <Networking/CheckForUpdates.hpp>
//

// Database
#include <Database/Database.hpp>
//

// Metadata
#include <Meta/Meta.hpp>
//

#endif // !ENIGMA_H
