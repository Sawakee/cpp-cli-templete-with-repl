#ifndef EXAMPLE_SUB2_SUB2_HPP
#define EXAMPLE_SUB2_SUB2_HPP

#include <memory>
#include "command.hpp"

namespace sub2 {
std::unique_ptr<cmd::Command> create();
}

#endif  // EXAMPLE_SUB2_SUB2_HPP
