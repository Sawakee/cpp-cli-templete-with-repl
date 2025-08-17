
# cpp-cli-templete-with-repl

This repository provides a simple, scalable, and extensible C++ command-line interface (CLI) framework, centered around a single header-only file: `command.hpp`.

- Just include `command.hpp` to get started—no library installation required.
- You can define recursive subcommands by composing `cmd::Command` objects, enabling scalable CLI architectures.
- Help messages are generated automatically based on your command tree.
- Both interactive and non-interactive (scriptable) execution modes are supported out of the box.

See the `example/` directory for a minimal but extensible CLI implementation using this framework.

## Build Instructions (Explicit File List in example Directory)

```sh
mkdir -p build
cd build
cmake ..
make
```

## How to Run

```sh
./example_cli
```
