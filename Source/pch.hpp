/*
*	ENIGMA Precompiled Header
*/
#pragma once
#ifndef ENIGMA_PCH_H
#define ENIGMA_PCH_H
#pragma warning(push, 0)

/// ENIGMA Core (Must be before platform includes to detect current platform)
#include <Core/Core.hpp>
///

/// Platform specific includes ///
/// Platform Windows include
#if defined(ENIGMA_PLATFORM_WINDOWS)
	#include <Windows.h>
#endif
///

///// ENIGMA Headers
#include "Enigma.hpp"
/////

/// OpenGL: GLAD
#include <glad/glad.h>
///

/// Graphic library framework: GLFW
#include <GLFW/glfw3.h>
///

/// UI: ImGui
#include <imgui.h>
///

/// Logger: SpdLog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/fmt.h> /// FMT: already embedded in SpdLog
///

/// pfd: portable file dialog
#include <portable-file-dialogs.h>
///

/// tablulate: cli table maker
#include <tabulate.hpp>
///

/// cxxopts: CMD parser library
#include <cxxopts.hpp>
///

/// cpr: cURL c++ wrapper
#include <cpr/cpr.h>
///

/// nlohmann-json: json parser library
#include <json.hpp> // nlohmann-json
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
#include <csignal>
///

#pragma warning(pop)
#endif // !ENIGMA_PCH_H

