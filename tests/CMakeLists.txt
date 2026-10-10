cmake_minimum_required(VERSION 3.16)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(FOSDProtocolTests LANGUAGES CXX)
    enable_testing()
endif()
set(CMAKE_AUTOMOC ON)
find_package(Qt5 5.12 REQUIRED COMPONENTS Core Gui Widgets Network Test)
add_executable(protocol_tests
    protocol_tests.cpp
    ../src/protocol/wavelengthframe.cpp
    ../src/protocol/analyzepackets.cpp
    ../src/Communication/udptask.cpp
    ../src/Communication/tcptask.cpp
    ../src/utils/global/global.cpp
)
target_compile_features(protocol_tests PRIVATE cxx_std_11)
if(MSVC)
    target_compile_options(protocol_tests PRIVATE /utf-8)
endif()
target_include_directories(protocol_tests PRIVATE
    ../src/protocol ../src/Communication ../src/utils/global ../src/utils/Timers)
target_link_libraries(protocol_tests PRIVATE
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Test)
add_test(NAME protocol_tests COMMAND protocol_tests)
