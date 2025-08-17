cmake_minimum_required(VERSION 3.16)

project(mycli VERSION 0.1 LANGUAGES CXX)

# C++17 固定、拡張なし、必須指定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# すべてのソースファイルを明示的に指定
set(SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/sub1/sub1.cpp
    ${CMAKE_SOURCE_DIR}/sub1/subsub1/subsub1.cpp
    ${CMAKE_SOURCE_DIR}/sub1/subsub2/subsub2.cpp
    ${CMAKE_SOURCE_DIR}/sub2/sub2.cpp
)

# 実行ファイル作成
add_executable(mycli ${SOURCES})

# インクルードパス（ヘッダがプロジェクト直下や親ディレクトリにあればここで追加）
target_include_directories(mycli PRIVATE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/..")

# target_compile_options(mycli PRIVATE -Wall -Wextra -Wpedantic -Werror)
