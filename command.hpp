#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace cmd {

inline std::optional<std::string> select_string_by_number_simple(
    const std::vector<std::string>& options,
    const std::string& prompt = "Select number: ", std::ostream& os = std::cout,
    std::istream& is = std::cin) {
  if (options.empty()) return std::nullopt;

  while (true) {
    os << "Selection:\n";
    for (size_t i = 0; i < options.size(); ++i) {
      os << "  " << (i + 1) << ". " << options[i] << "\n";
    }

    os << prompt;
    std::string line;
    if (!std::getline(is, line)) return std::nullopt;

    try {
      int choice = std::stoi(line);
      if (choice >= 1 && static_cast<size_t>(choice) <= options.size()) {
        return options[choice - 1];
      } else {
        os << "Invalid Number.\n";
      }
    } catch (...) {
      os << "Enter Number.\n";
    }
  }
}

struct Command {
  std::string name;
  std::string description;
  std::map<std::string, std::unique_ptr<Command>> subcommands;
  std::function<void(const std::vector<std::string>& args)> action;

  Command(std::string n, std::string d)
      : name(std::move(n)), description(std::move(d)), action(nullptr) {}

  Command& addSubcommand(std::unique_ptr<Command> child) {
    auto& ref = *child;
    subcommands[child->name] = std::move(child);
    return ref;
  }

  Command* getSubcommand(const std::string& n) {
    auto it = subcommands.find(n);
    return it != subcommands.end() ? it->second.get() : nullptr;
  }

  void printAvailableSubcommands(std::ostream& os = std::cout) const {
    if (subcommands.empty()) {
      os << "(No subcommand)\n";
      return;
    }
    os << "Available subcommands:\n";
    for (const auto& [k, v] : subcommands) {
      os << "  " << k << "\t" << v->description << "\n";
    }
  }

  void printHelp(std::ostream& os = std::cout) const {
    os << "\n";
    os << "Usage: " << name;
    if (!subcommands.empty()) os << " <subcommand>";
    os << " [args...]\n";
    os << description << "\n";
    printAvailableSubcommands(os);
    os << "\n";
  }

  void execute(std::vector<std::string> args) {
    if (!args.empty() && args[0] == "--help") {
      printHelp();
      return;
    }

    if (action) {
      action(args);
      return;
    }

    if (args.empty()) {
      printHelp();
      return;
    }

    const std::string next = args.front();
    auto it = subcommands.find(next);
    if (it != subcommands.end()) {
      args.erase(args.begin());
      it->second->execute(std::move(args));
      return;
    }
  }

  void interactive_execute() {
    Command* current = this;

    while (true) {
      if (current->action) {
        current->action({});
        std::cout << "Please enter to continue ...";
        std::string dummy;
        std::getline(std::cin, dummy);
        current = this;  // Back to root.
        std::cout << "\n";
        continue;
      }

      if (current->subcommands.empty()) {
        current->printHelp();
        break;
      }

      std::vector<std::string> names;
      for (const auto& [k, _] : current->subcommands) {
        names.push_back(k);
      }
      names.emplace_back("help");
      names.emplace_back("exit");

      std::optional<std::string> selected_name =
          select_string_by_number_simple(names, "Select command: ");

      if (!selected_name) break;

      if (*selected_name == "exit") break;
      if (*selected_name == "help") {
        current->printHelp();
        continue;
      }

      current = current->subcommands[*selected_name].get();
    }
  }

  void run(std::vector<std::string> args) {
    if (args.empty()) {
      interactive_execute();
    } else {
      execute(std::move(args));
    }
  }
};

}  // namespace cmd

#endif  // COMMAND_HPP
