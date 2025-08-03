#include "./subA.hpp"
#include <memory>
#include "./subsubA/subsubA.hpp"
#include "./subsubB/subsubB.hpp"
#include "command.hpp"

std::shared_ptr<Command> make_subA_command() {
  auto cmd = std::make_shared<Command>();
  cmd->name = "subA";
  cmd->description = "subAコマンド";

  cmd->add_subcommand(make_subsubA_command());
  cmd->add_subcommand(make_subsubB_command());

  return cmd;
}
