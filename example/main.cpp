#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "./sub1/sub1.hpp"
#include "./sub2/sub2.hpp"
#include "command.hpp"

int main(int argc, char* argv[]) {
  auto root = std::make_unique<cmd::Command>("app", "root command");

  root->addSubcommand(sub1::create());
  root->addSubcommand(sub2::create());

  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }

  root->run(std::move(args));
  return 0;
}
