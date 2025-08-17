#ifndef EXAMPLE_SUB1_SUB1_HPP
#define EXAMPLE_SUB1_SUB1_HPP

#include <memory>
#include "command.hpp"

namespace sub1 {
std::unique_ptr<cmd::Command> create();
}

#endif  // EXAMPLE_SUB1_SUB1_HPP
