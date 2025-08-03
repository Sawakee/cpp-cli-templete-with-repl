// command.cpp
#include "command.hpp"
#include <algorithm>
#include <iostream>

void Command::add_subcommand(std::shared_ptr<Command> sub) {
  subcommands[sub->name] = std::move(sub);
}

void Command::print_help(const std::string& parent_path) const {
  std::string full = parent_path.empty() ? name : parent_path + " " + name;
  std::cout << "Usage: " << full;
  if (!subcommands.empty()) std::cout << " <subcommand>";
  std::cout << " [options]\n";
  std::cout << description << "\n\n";
  if (!subcommands.empty()) {
    std::cout << "Subcommands:\n";
    for (const auto& [k, v] : subcommands) {
      std::cout << "  " << k << "\t" << v->description << "\n";
    }
  }
}

int Command::execute(const std::vector<std::string>& tokens, size_t index,
                     const std::string& path) {
  if (index < tokens.size()) {
    const std::string& tok = tokens[index];
    if (tok == "--help" || tok == "-h") {
      print_help(path);
      return 0;
    }
    auto it = subcommands.find(tok);
    if (it != subcommands.end()) {
      std::string new_path = path.empty() ? name : path + " " + name;
      return it->second->execute(tokens, index + 1, new_path);
    } else {
      if (handler) {
        std::vector<std::string> rest(tokens.begin() + index, tokens.end());
        return handler(rest);
      } else {
        std::cerr << "不明なサブコマンドまたはオプション: " << tok << "\n";
        print_help(path);
        return 1;
      }
    }
  } else {
    if (handler) {
      return handler({});
    } else {
      print_help(path);
      return 0;
    }
  }
}
