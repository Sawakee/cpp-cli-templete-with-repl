#include "sub1.hpp"
#include <iostream>
#include "command.hpp"

#include "subsub1/subsub1.hpp"
#include "subsub2/subsub2.hpp"

namespace sub1 {
std::unique_ptr<cmd::Command> create() {
  auto sub1 = std::make_unique<cmd::Command>("sub1", "description of sub1");
  sub1->addSubcommand(subsub1::create());
  sub1->addSubcommand(subsub2::create());
  return sub1;
}
}  // namespace sub1
