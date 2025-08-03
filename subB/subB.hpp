#pragma once

#include <memory>
#include "command.hpp"

// subsubA, subsubBのファクトリ関数を宣言
std::shared_ptr<Command> make_subB_command();
