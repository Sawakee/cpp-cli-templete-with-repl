#include <iostream>
#include <memory>
#include "command.hpp"

std::shared_ptr<Command> make_subsubA_command() {
  auto cmd = std::make_shared<Command>();
  cmd->name = "subsubA";
  cmd->description = "subsubAコマンド";
  cmd->handler = [](const std::vector<std::string>&) -> int {
    std::cout << "execute_subsubA_command\n";
    return 0;
  };
  return cmd;
}
