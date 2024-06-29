#include "TestsAll.hpp"

int main(int argc, const char* argv[]) {
  Catch::Session session{};
  return session.run(argc, argv);
}