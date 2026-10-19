# Business modules.
#
# Every module is built as a static library named `fosd_<module>` and exposes
# its public headers through `target_include_directories(... PUBLIC ...)`, so a
# consumer only needs `target_link_libraries(app PRIVATE fosd_<module>)`.
#
# Core data models, communication, protocol, storage and calculation have build targets. Other directories
# (config, logging, visualization) are placeholders
# and will be added here once they have sources and a CMakeLists.txt.

add_subdirectory(communication)
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS)
    add_subdirectory(core)
    add_subdirectory(protocol)
    add_subdirectory(storage)
    add_subdirectory(calculation)
endif()
