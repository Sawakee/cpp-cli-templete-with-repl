#ifndef EXAMPLE_SUB1_SUBSUB2_SUBSUB2_HPP
#define EXAMPLE_SUB1_SUBSUB2_SUBSUB2_HPP

#include <memory>
#include "command.hpp"

namespace subsub2 {
std::unique_ptr<cmd::Command> create();
}

#endif  // EXAMPLE_SUB1_SUBSUB2_SUBSUB2_HPP
