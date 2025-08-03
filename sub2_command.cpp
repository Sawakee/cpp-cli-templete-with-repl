#include <iostream>
#include "command.hpp"

auto make_sub2_command() -> std::unique_ptr<cmd::Command> {
  auto sub2 = std::make_unique<cmd::Command>("sub2", "sub2 の説明");

  auto deeper = std::make_unique<cmd::Command>("deeper", "さらに深いサブコマンド");
  deeper->action = [](const std::vector<std::string>& args, cmd::ExecMode mode) {
    std::cout << "deeper が実行された。引数数=" << args.size() << "\n";
    if (!args.empty()) {
      std::cout << "受け取った引数:";
      for (const auto& a : args) std::cout << " [" << a << "]";
      std::cout << "\n";
    }
  };

  sub2->addSubcommand(std::move(deeper));
  return sub2;
}
