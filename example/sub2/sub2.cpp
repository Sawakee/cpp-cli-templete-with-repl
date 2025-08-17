#include "sub2.hpp"
#include <iostream>
#include <vector>
#include "command.hpp"

namespace sub2 {
std::unique_ptr<cmd::Command> create() {
  auto sub2 = std::make_unique<cmd::Command>("sub2", "description of sub2");
  sub2->action = [](const std::vector<std::string>& args, cmd::ExecMode mode) {
    std::vector<std::string> sub2_args = args;
    if (mode == cmd::ExecMode::kInteractive) {
      std::cout << "interactive mode in sub2\n";
      std::vector<std::string> state_options = {"option1", "option2", "option3"};
      auto state = cmd::select_string_by_number_simple(state_options, "Select an option: ");
      sub2_args = {state.value_or("default")};
    }

    std::cout << "sub2 executed with args:";
    for (const auto& a : sub2_args) {
      std::cout << " [" << a << "]";
    }
    std::cout << "\n";
  };
  return sub2;
}
}  // namespace sub2
