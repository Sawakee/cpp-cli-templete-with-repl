#pragma once
#include <memory>
#include <string>
#include <vector>
#include "command.hpp"

// コマンドライン引数をトークン化
std::vector<std::string> to_tokens(int argc, char* argv[]);

// 行を空白で分割（簡易、引用符未対応）
std::vector<std::string> split_line(const std::string& line);

// subcommandsの中からprefix一致するコマンドを返す
std::vector<std::shared_ptr<Command>> prefix_matches(const std::shared_ptr<Command>& current,
                                                     const std::string& token);

// subcommandsを番号付きで一覧表示しベクタで返す
std::vector<std::shared_ptr<Command>> list_and_suggest(const std::shared_ptr<Command>& current);
