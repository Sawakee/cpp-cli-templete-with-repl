#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <algorithm>
#include <cctype>

// -------------------------------
// Command 基盤
// -------------------------------
struct Command {
    std::string name;
    std::string description;
    std::function<int(const std::vector<std::string>& args)> handler;
    std::map<std::string, std::shared_ptr<Command>> subcommands;

    void add_subcommand(std::shared_ptr<Command> sub) {
        subcommands[sub->name] = std::move(sub);
    }

    void print_help(const std::string& parent_path = "") const {
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

    int execute(const std::vector<std::string>& tokens, size_t index = 0, const std::string& path = "") {
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
};

// argv をトークン化
std::vector<std::string> to_tokens(int argc, char* argv[]) {
    std::vector<std::string> t;
    for (int i = 1; i < argc; ++i) t.emplace_back(argv[i]);
    return t;
}

// 行を空白で分割（簡易: 引用など未対応）
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

// 現在位置のパス文字列化
std::string build_path(const std::shared_ptr<Command>& cmd, const std::vector<std::shared_ptr<Command>>& ancestry) {
    std::string path;
    for (const auto& c : ancestry) {
        if (!path.empty()) path += " ";
        path += c->name;
    }
    if (!path.empty()) path += " ";
    path += cmd->name;
    return path;
}

// 子コマンドを番号付き一覧表示してベクタで返す
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

// プレフィックス一致で候補収集
std::vector<std::shared_ptr<Command>> prefix_matches(const std::shared_ptr<Command>& current, const std::string& token) {
    std::vector<std::shared_ptr<Command>> matches;
    for (const auto& [name, cmd] : current->subcommands) {
        if (name.rfind(token, 0) == 0) {
            matches.push_back(cmd);
        }
    }
    return matches;
}

// REPL 本体（番号選択・曖昧・上下移動含む）
void repl(std::shared_ptr<Command> root) {
    std::shared_ptr<Command> current = root;
    std::vector<std::shared_ptr<Command>> ancestry; // 親方向
    std::string line;

    std::cout << "インタラクティブモード。終了するには exit または quit を入力。\n";
//    root->print_help("");
    auto children = list_and_suggest(root);

    while (true) {
        std::string prompt;
        if (current != root) {
            prompt = build_path(current, ancestry) + "> ";
        } else {
            prompt = root->name + "> ";
        }
        std::cout << prompt << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\n";
            break;
        }
        auto tokens = split_line(line);
        if (tokens.empty()) continue;
        const std::string& cmd = tokens[0];

        if (cmd == "exit" || cmd == "quit") break;
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

        bool handled = false;

        // 数字選択
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

        // 完全一致サブコマンド
        auto it = current->subcommands.find(cmd);
        if (it != current->subcommands.end()) {
            ancestry.push_back(current);
            current = it->second;
            std::vector<std::string> rest(tokens.begin() + 1, tokens.end());
            current->execute(rest, 0, "");
            children = list_and_suggest(current);
            continue;
        }

        // プレフィックス曖昧一致
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

        // 現在のコマンドのハンドラを実行
        if (current->handler) {
            current->execute(tokens, 0, "");
            children = list_and_suggest(current);
            continue;
        }

        std::cerr << "不明なコマンド: " << cmd << "\n";
        children = list_and_suggest(current);
    }
}

// -------------------------------
// サブコマンド定義（例: greet, admin user add role assign）
// -------------------------------

// greet hello
std::shared_ptr<Command> make_hello_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "hello";
    cmd->description = "こんにちはを出力する";
    cmd->handler = [](const std::vector<std::string>& args) -> int {
        std::string name = "world";
        if (!args.empty()) name = args[0];
        std::cout << "こんにちは, " << name << "\n";
        return 0;
    };
    return cmd;
}

// greet bye
std::shared_ptr<Command> make_bye_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "bye";
    cmd->description = "さようならを出力する";
    cmd->handler = [](const std::vector<std::string>&) -> int {
        std::cout << "さようなら\n";
        return 0;
    };
    return cmd;
}

// greet
std::shared_ptr<Command> make_greet_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "greet";
    cmd->description = "挨拶関連のコマンド";
    cmd->add_subcommand(make_hello_command());
    cmd->add_subcommand(make_bye_command());
    return cmd;
}

// admin user add role assign

// assign（最深層）
std::shared_ptr<Command> make_assign_role_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "assign";
    cmd->description = "ユーザーにロールを割り当てる";
    cmd->handler = [](const std::vector<std::string>& args) -> int {
        if (args.size() < 2) {
            std::cerr << "使い方: assign <username> <role>\n";
            return 1;
        }
        const std::string& username = args[0];
        const std::string& role = args[1];
        std::cout << "ユーザー " << username << " にロール " << role << " を割り当てました\n";
        return 0;
    };
    return cmd;
}

// role（add の下）
std::shared_ptr<Command> make_role_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "role";
    cmd->description = "ユーザーのロール操作";
    cmd->add_subcommand(make_assign_role_command());
    return cmd;
}

// add（user の下）
std::shared_ptr<Command> make_add_user_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "add";
    cmd->description = "ユーザーを追加する";
    cmd->handler = [](const std::vector<std::string>& args) -> int {
        if (args.empty()) {
            std::cerr << "ユーザー名を指定してください\n";
            return 1;
        }
        std::string username = args[0];
        std::cout << "ユーザー " << username << " を追加しました\n";
        return 0;
    };
    cmd->add_subcommand(make_role_command());
    return cmd;
}

// user（admin の下）
std::shared_ptr<Command> make_user_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "user";
    cmd->description = "ユーザー操作";
    cmd->add_subcommand(make_add_user_command());
    return cmd;
}

// admin（ルート直下）
std::shared_ptr<Command> make_admin_command() {
    auto cmd = std::make_shared<Command>();
    cmd->name = "admin";
    cmd->description = "管理者コマンド";
    cmd->add_subcommand(make_user_command());
    return cmd;
}

// -------------------------------
// main
// -------------------------------
int main(int argc, char* argv[]) {
    auto root = std::make_shared<Command>();
    root->name = "mycli";
    root->description = "階層的サブコマンドとインタラクティブ REPL を持つ CLI";

    root->add_subcommand(make_greet_command());
    root->add_subcommand(make_admin_command());

    auto tokens = to_tokens(argc, argv);
    if (tokens.empty()) {
        repl(root);
        return 0;
    }
    return root->execute(tokens, 0, "");
}

