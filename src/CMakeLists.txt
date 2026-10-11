# Business modules.
#
# Every module is built as a static library named `fosd_<module>` and exposes
# its public headers through `target_include_directories(... PUBLIC ...)`, so a
# consumer only needs `target_link_libraries(app PRIVATE fosd_<module>)`.
#
# Core data models, communication and protocol are implemented. Other directories
# (config, logging, storage, visualization) are placeholders
# and will be added here once they have sources and a CMakeLists.txt.

add_subdirectory(communication)
add_subdirectory(core)
add_subdirectory(protocol)
