#include "subsub1.hpp"
#include <iostream>
#include <vector>
#include "command.hpp"

namespace subsub1 {
std::unique_ptr<cmd::Command> create() {
  auto subsub1 = std::make_unique<cmd::Command>("subsub1", "description of subsub1");
  subsub1->action = [](const std::vector<std::string>& args, cmd::ExecMode mode) {
    std::vector<std::string> subsub1_args = args;
    if (mode == cmd::ExecMode::kInteractive) {
      std::cout << "interactive mode in subsub1\n";
      std::vector<std::string> state_options = {"option1", "option2", "option3"};
      auto state = cmd::select_string_by_number_simple(state_options, "Select an option: ");
      subsub1_args = {state.value_or("default")};
    }

    std::cout << "subsub1 executed with args:";
    for (const auto& a : subsub1_args) {
      std::cout << " [" << a << "]";
    }
    std::cout << "\n";
  };
  return subsub1;
}
}  // namespace subsub1
