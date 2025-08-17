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

  root->run(argc, argv);
  return 0;
}
