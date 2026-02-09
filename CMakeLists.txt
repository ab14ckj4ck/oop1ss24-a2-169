cmake_minimum_required(VERSION 3.16)
project(A2_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Alle cpp-Dateien automatisch finden
file(GLOB_RECURSE PROJECT_SOURCES
        "*.cpp"
)

file(GLOB_RECURSE PROJECT_HEADERS
        "*.hpp"
)

# Für CLion: Erstelle ein "dummy"-Executable,
# damit Code-Analyse funktioniert.
add_executable(a2_dummy
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)

# Optional: Warnings einschalten (nur für CLion, nicht make)
target_compile_options(a2_dummy PRIVATE
        -Wall -Wextra -Wpedantic
)
