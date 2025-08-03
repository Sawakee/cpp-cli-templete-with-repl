#pragma once
#include <memory>
#include "command.hpp"

void repl(std::shared_ptr<Command> root);
