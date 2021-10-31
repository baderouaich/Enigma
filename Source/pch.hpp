/*
*	ENIGMA Precompiled Header
*/
#pragma once
#ifndef ENIGMA_PCH_H
#define ENIGMA_PCH_H


/// ENIGMA Core (Must be before platform includes to detect current platform)
//#include <Core/Core.hpp>
#include <Enigma.hpp>
///


/// Platform specific includes ///
/// Platform Windows include
#if defined(ENIGMA_PLATFORM_WINDOWS)
	#include <Windows.h>
#endif
///

///// ENIGMA Headers
//#include "Enigma.hpp"
/////


/// OpenGL: GLAD
#pragma warning(push, 0)
#include <glad/glad.h>
#pragma warning(pop)
///

/// Graphic library framwork: GLFW
#pragma warning(push, 0)
#include <GLFW/glfw3.h>
#pragma warning(pop)
///

/// UI: ImGui
#pragma warning(push, 0)
#include <imgui.h>
#pragma warning(pop)
///

/// Logger: SpdLog
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/fmt.h> /// FMT: already embedded in SpdLog
#pragma warning(pop)
///


/// pfd: portable file dialog
#pragma warning(push, 0)
#include <portable-file-dialogs.h>
#pragma warning(pop)
///

/// tablulate: cli table maker
#pragma warning(push, 0)
#include <tabulate.hpp>
#pragma warning(pop)
///


/// cxxopts: CMD parser library
#pragma warning(push, 0)
#include <cxxopts.hpp>
#pragma warning(pop)
///

/// cpr: cURL c++ wrapper
#pragma warning(push, 0)
#include <cpr/cpr.h>
#pragma warning(pop)
///

/// nlohmann-json: json parser library
#pragma warning(push, 0)
#include <json.hpp> // nlohmann-json
#pragma warning(pop)
///


/// the C++ Standard Libraries
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <stack>
#include <thread>
#include <memory>
#include <queue>
#include <deque>
#include <utility>
#include <list>
#include <future>
#include <functional>
#include <iomanip>
#include <numeric>
#include <regex>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>
#include <cstdint>
#include <cstring>
///



#endif // !ENIGMA_PCH_H

