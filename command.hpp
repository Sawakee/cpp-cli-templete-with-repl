#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Command {
  std::string name;
  std::string description;
  std::function<int(const std::vector<std::string>& args)> handler;
  std::map<std::string, std::shared_ptr<Command>> subcommands;

  void add_subcommand(std::shared_ptr<Command> sub);

  void print_help(const std::string& parent_path = "") const;

  auto execute(const std::vector<std::string>& tokens, size_t index = 0,
               const std::string& path = "") -> int;
};
