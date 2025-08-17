#include "subsub2.hpp"
#include <iostream>
#include <vector>
#include "command.hpp"

namespace subsub2 {
std::unique_ptr<cmd::Command> create() {
  auto subsub2 = std::make_unique<cmd::Command>("subsub2", "description of subsub2");
  subsub2->action = [](const std::vector<std::string>& args, cmd::ExecMode mode) {
    std::vector<std::string> subsub2_args = args;
    if (mode == cmd::ExecMode::kInteractive) {
      std::cout << "interactive mode in subsub2\n";
      std::vector<std::string> state_options = {"option1", "option2", "option3"};
      auto state = cmd::select_string_by_number_simple(state_options, "Select an option: ");
      subsub2_args = {state.value_or("default")};
    }

    std::cout << "subsub2 executed with args:";
    for (const auto& a : subsub2_args) {
      std::cout << " [" << a << "]";
    }
    std::cout << "\n";
  };
  return subsub2;
}
}  // namespace subsub2
