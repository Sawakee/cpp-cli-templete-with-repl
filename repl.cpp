#include "repl.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include "./utils.hpp"

// 補助関数の例：現在のコマンドまでのパスを文字列化
static std::string build_path(const std::shared_ptr<Command>& cmd,
                              const std::vector<std::shared_ptr<Command>>& ancestry) {
  std::string path;
  for (const auto& c : ancestry) {
    if (!path.empty()) {
      path += " ";
    }
    path += c->name;
  }
  if (!path.empty()) {
    path += " ";
  }
  path += cmd->name;
  return path;
}

void repl(std::shared_ptr<Command> root) {
  std::shared_ptr<Command> current = root;
  std::vector<std::shared_ptr<Command>> ancestry;
  std::string line;

  std::cout << "インタラクティブモード。終了するには exit または quit を入力。\n";
  auto children = list_and_suggest(root);

  while (true) {
    std::string prompt = (current != root) ? build_path(current, ancestry) : root->name;
    std::cout << prompt << "> " << std::flush;

    if (!std::getline(std::cin, line)) {
      std::cout << "\n";
      break;
    }

    auto tokens = split_line(line);
    if (tokens.empty()) {
      continue;
    }
    const std::string& cmd = tokens[0];

    if (cmd == "exit" || cmd == "quit") {
      break;
    }

    if (cmd == "help") {
      current->print_help("");
      children = list_and_suggest(current);
      continue;
    }

    if (cmd == "back") {
      if (!ancestry.empty()) {
        current = ancestry.back();
        ancestry.pop_back();
      }
      children = list_and_suggest(current);
      continue;
    }

    if (cmd == "root") {
      current = root;
      ancestry.clear();
      children = list_and_suggest(current);
      continue;
    }

    bool is_number = !cmd.empty() && std::all_of(cmd.begin(), cmd.end(), ::isdigit);
    if (is_number) {
      int num = std::stoi(cmd);
      if (1 <= num && num <= static_cast<int>(children.size())) {
        auto target = children[num - 1];
        ancestry.push_back(current);
        current = target;
        std::vector<std::string> rest(tokens.begin() + 1, tokens.end());
        current->execute(rest, 0, "");
        children = list_and_suggest(current);
        continue;
      }
    }

    auto it = current->subcommands.find(cmd);
    if (it != current->subcommands.end()) {
      ancestry.push_back(current);
      current = it->second;
      std::vector<std::string> rest(tokens.begin() + 1, tokens.end());
      current->execute(rest, 0, "");
      children = list_and_suggest(current);
      continue;
    }

    auto matches = prefix_matches(current, cmd);
    if (matches.size() > 1) {
      std::cout << "曖昧です。候補:\n";
      for (const auto& m : matches) {
        std::cout << "  " << m->name << "\t" << m->description << "\n";
      }
      continue;
    }
    if (matches.size() == 1) {
      ancestry.push_back(current);
      current = matches[0];
      std::vector<std::string> rest(tokens.begin() + 1, tokens.end());
      current->execute(rest, 0, "");
      children = list_and_suggest(current);
      continue;
    }

    if (current->handler) {
      current->execute(tokens, 0, "");
      children = list_and_suggest(current);
      continue;
    }

    std::cerr << "不明なコマンド: " << cmd << "\n";
    children = list_and_suggest(current);
  }
}
