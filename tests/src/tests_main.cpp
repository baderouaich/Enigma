#include "TestsAll.hpp"
#include <Logger/Logger.hpp>

int main(int argc, const char* argv[]) {
  Enigma::Logger::initialize();
  Catch::Session session{};
  return session.run(argc, argv);
}