#ifndef EXAMPLE_SUB1_SUBSUB1_SUBSUB1_HPP
#define EXAMPLE_SUB1_SUBSUB1_SUBSUB1_HPP

#include <memory>
#include "command.hpp"

namespace subsub1 {
std::unique_ptr<cmd::Command> create();
}

#endif  // EXAMPLE_SUB1_SUBSUB1_SUBSUB1_HPP
