#include <iostream>
#include <memory>
#include "command.hpp"

std::shared_ptr<Command> make_subsubB_command() {
  auto cmd = std::make_shared<Command>();
  cmd->name = "subsubB";
  cmd->description = "subsubBコマンド";
  cmd->handler = [](const std::vector<std::string>&) -> int {
    std::cout << "execute_subsubB_command\n";
    return 0;
  };
  return cmd;
}
