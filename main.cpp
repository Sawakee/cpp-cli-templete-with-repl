#include "command.hpp"

// 各サブツリーを返す関数を外部で宣言
std::unique_ptr<cmd::Command> make_sub1_command();
std::unique_ptr<cmd::Command> make_sub2_command();

int main(int argc, char* argv[]) {
  using namespace cmd;

  Command root("app", "トップレベルのコマンド");

  // 各翻訳単位の builder から受け取り接続（所有権は root が持つ）
  root.addSubcommand(make_sub1_command());
  root.addSubcommand(make_sub2_command());

  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }

  root.run(std::move(args));
  return 0;
}
