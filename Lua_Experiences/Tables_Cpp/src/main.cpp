#include "table.hpp"

#include <print>

auto main() -> int {

  if (!test()) {
    std::println("Test failed!");
  }

  return 0;
}
