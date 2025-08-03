#include <memory>
#include "./command.hpp"
#include "./repl.hpp"
#include "./subA/subA.hpp"
#include "./subB/subB.hpp"
#include "./utils.hpp"

// to_tokens, split_line, repl などは省略または適宜実装をincludeしてください。

int main(int argc, char* argv[]) {
  auto root = std::make_shared<Command>();
  root->name = "mycli";
  root->description = "階層的サブコマンドを持つCLI";

  root->add_subcommand(make_subA_command());
  root->add_subcommand(make_subB_command());

  auto tokens = to_tokens(argc, argv);
  if (tokens.empty()) {
    repl(root);
    return 0;
  }
  return root->execute(tokens, 0, "");
}
