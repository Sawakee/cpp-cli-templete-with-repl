#include "./subB.hpp"
#include <iostream>

std::shared_ptr<Command> make_subB_command() {
  auto cmd = std::make_shared<Command>();
  cmd->name = "subB";
  cmd->description = "subBコマンド";
  cmd->handler = [](const std::vector<std::string>&) -> int {
    std::cout << "execute_subB_command\n";
    return 0;
  };
  return cmd;
}
