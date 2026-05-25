cmake_minimum_required(VERSION 3.16)
project(physics_calc VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt auto-features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Help CMake find Homebrew-installed Qt6 on macOS.
if(APPLE AND NOT DEFINED CMAKE_PREFIX_PATH)
    foreach(prefix
            "/opt/homebrew/opt/qt"
            "/opt/homebrew/opt/qt@6"
            "/usr/local/opt/qt"
            "/usr/local/opt/qt@6")
        if(EXISTS "${prefix}")
            list(APPEND CMAKE_PREFIX_PATH "${prefix}")
        endif()
    endforeach()
endif()

find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets)

set(CORE_SRC
    src/core/Dimensions.cpp
    src/core/Units.cpp
    src/core/Lexer.cpp
    src/core/Parser.cpp
    src/core/Calculator.cpp
)
set(CORE_HDR
    src/core/Dimensions.h
    src/core/Quantity.h
    src/core/Units.h
    src/core/Lexer.h
    src/core/Parser.h
    src/core/Calculator.h
    src/core/Exceptions.h
)

set(UI_SRC src/ui/MainWindow.cpp)
set(UI_HDR src/ui/MainWindow.h)

qt_add_executable(physics_calc
    src/main.cpp
    ${CORE_SRC} ${CORE_HDR}
    ${UI_SRC}   ${UI_HDR}
)

target_include_directories(physics_calc PRIVATE src)

target_link_libraries(physics_calc PRIVATE Qt6::Widgets)

set_target_properties(physics_calc PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
