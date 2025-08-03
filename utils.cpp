#include "./utils.hpp"
#include <cctype>
#include <iostream>
#include "command.hpp"

std::vector<std::string> to_tokens(int argc, char* argv[]) {
  std::vector<std::string> tokens;
  for (int i = 1; i < argc; ++i) {
    tokens.emplace_back(argv[i]);
  }
  return tokens;
}

std::vector<std::string> split_line(const std::string& line) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : line) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!cur.empty()) {
        out.emplace_back(cur);
        cur.clear();
      }
    } else {
      cur.push_back(c);
    }
  }
  if (!cur.empty()) out.emplace_back(cur);
  return out;
}

std::vector<std::shared_ptr<Command>> prefix_matches(const std::shared_ptr<Command>& current,
                                                     const std::string& token) {
  std::vector<std::shared_ptr<Command>> matches;
  for (const auto& [name, cmd] : current->subcommands) {
    if (name.rfind(token, 0) == 0) {  // prefix一致
      matches.push_back(cmd);
    }
  }
  return matches;
}

std::vector<std::shared_ptr<Command>> list_and_suggest(const std::shared_ptr<Command>& current) {
  std::vector<std::shared_ptr<Command>> indexed;
  if (current->subcommands.empty()) return indexed;
  std::cout << "次に使えるサブコマンド:\n";
  int idx = 1;
  for (const auto& [name, cmd] : current->subcommands) {
    std::cout << "  " << idx << ") " << name << "\t" << cmd->description << "\n";
    indexed.push_back(cmd);
    ++idx;
  }
  return indexed;
}
