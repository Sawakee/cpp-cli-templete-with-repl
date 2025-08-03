cmake_minimum_required(VERSION 3.16)

project(mycli VERSION 0.1 LANGUAGES CXX)

# C++17 固定、拡張なし、必須指定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "*.cpp" "**/*.cpp" "**/**/*.cpp")

# 実行ファイル作成
add_executable(mycli ${SOURCES})

# インクルードパス（ヘッダがプロジェクト直下にあればここで追加）
target_include_directories(mycli PRIVATE "${CMAKE_SOURCE_DIR}")

# target_compile_options(mycli PRIVATE -Wall -Wextra -Wpedantic -Werror)
