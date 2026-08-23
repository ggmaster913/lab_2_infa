cmake_minimum_required(VERSION 3.10)
project(Lab2_Sequences CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# путь к заголовочным файлам
include_directories(include)

# исполняемый файл для тестов
add_executable(tests
    tests/Tests.cpp
)

# исполняемый файл для UI
add_executable(ui
    ui/ConsoleUI.cpp
)

