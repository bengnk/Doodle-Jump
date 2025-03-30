cmake_minimum_required(VERSION 3.10)
project(Doodle-Jump)

set(CMAKE_CXX_STANDARD 17)

# Setze den Pfad zu SFMLConfig.cmake
set(SFML_DIR "C:/Users/Ben Gnielka/Doodle-Jump/SFML-2.5.1/lib/cmake/SFML")

# Finde das SFML-Paket (SFML 2.5.1)
find_package(SFML 2.5 REQUIRED graphics audio network)

# Erstelle das Executable und füge alle Quelldateien hinzu
add_executable(Doodle-Jump main.cpp
        Game.cpp
        Player.cpp
        Platform.cpp
        Enemy.cpp
        Boost.cpp)

# Verlinke die SFML-Bibliotheken mit deinem Projekt
target_link_libraries(Doodle-Jump PRIVATE sfml-graphics sfml-audio sfml-network)
