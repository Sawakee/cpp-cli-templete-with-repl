#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace cmd {

/// Buil-in command constants
namespace builtin {
inline constexpr const char* kHelp = "help";
inline constexpr const char* kExit = "exit";
inline constexpr const char* lhelpFlag = "--help";
}  // namespace builtin

/// Interactive string selection utility
/// @param options Available string options for selection
/// @param prompt User prompt message
/// @param os Output stream for displaying options
/// @param is Input stream for reading user input
/// @return Selected string option or std::nullopt if no valid selection is made
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
        return options[static_cast<size_t>(choice - 1)];
      }
      os << "Invalid Number.\n";
    } catch (...) {
      os << "Enter Number.\n";
    }
  }
}

/// COmmand exectuion mode
enum class ExecMode : std::uint8_t { kNonInteractive, kInteractive };

using Action = std::function<void(const std::vector<std::string>&, ExecMode)>;

struct Command {
  std::string name;
  std::string description;
  std::map<std::string, std::unique_ptr<Command>> subcommands;
  Action action;

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
    if (!args.empty() && args[0] == builtin::lhelpFlag) {
      printHelp();
      return;
    }

    if (action) {
      action(args, ExecMode::kNonInteractive);
      return;
    }

    if (args.empty()) {
      printHelp();
      return;
    }

    const std::string next = args.front();
    auto* subcommand = getSubcommand(next);
    if (subcommand != nullptr) {
      args.erase(args.begin());
      subcommand->execute(std::move(args));
      return;
    }

    std::cerr << "Unknown command: " << next << "\n";
    printHelp();
  }

  void interactive_execute() {
    Command* current = this;

    while (true) {
      if (current->action) {
        current->action({}, ExecMode::kInteractive);
        std::cout << "\n";
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
      names.reserve(current->subcommands.size() + 2);
      for (const auto& [k, _] : current->subcommands) {
        names.push_back(k);
      }
      names.emplace_back(builtin::kHelp);
      names.emplace_back(builtin::kExit);

      std::optional<std::string> selected_name =
          select_string_by_number_simple(names, "Select command: ");

      if (!selected_name) break;

      if (*selected_name == builtin::kExit) break;
      if (*selected_name == builtin::kHelp) {
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

  void run(int argc, char* argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
      args.emplace_back(argv[i]);
    }
    run(std::move(args));
  }
};

}  // namespace cmd

#endif  // COMMAND_HPP
