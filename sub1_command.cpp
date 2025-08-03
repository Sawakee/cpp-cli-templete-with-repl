#include <iostream>
#include "command.hpp"

auto make_sub1_command() -> std::unique_ptr<cmd::Command> {
  auto sub1 = std::make_unique<cmd::Command>("sub1", "sub1 の説明");
  sub1->action = [](const std::vector<std::string>& args, cmd::ExecMode mode) {
    if (args.empty()) {
      std::cout << "sub1 のデフォルト動作\n";
    } else {
      std::cout << "sub1 に渡された引数:";
      for (const auto& a : args) std::cout << " [" << a << "]";
      std::cout << "\n";
    }
  };
  return sub1;
}
