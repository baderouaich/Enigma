#pragma once

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
#include <Enigma.hpp>
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
#include <spdlog/common.h>
#include <spdlog/fmt/fmt.h> /// FMT: already embedded in SpdLog
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
///

/// pfd: portable file dialog
#include <portable-file-dialogs.h>
///

/// tablulate: cli table maker
#include <tabulate/table.hpp>
///

/// cxxopts: CMD parser library
#include <cxxopts.hpp>
///

/// cpr: cURL c++ wrapper
#include <cpr/cpr.h>
///

/// nlohmann-json: json parser library
#include <nlohmann/json.hpp> // nlohmann-json
///


/// the C++ Standard Libraries
#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
///

/// the C Standard Libraries
#include <cassert>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <csignal>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
///
#pragma warning(pop)
