# Business modules.
#
# Every module is built as a static library named `fosd_<module>` and exposes
# its public headers through `target_include_directories(... PUBLIC ...)`, so a
# consumer only needs `target_link_libraries(app PRIVATE fosd_<module>)`.
#
# Config, core, communication, protocol, storage, calculation and visualization
# have build targets. Logging remains a placeholder.

add_subdirectory(config)

if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP OR FOSD_BUILD_COMMUNICATION_ASSISTANT_QML OR FOSD_BUILD_WIDGETS_ASSISTANT)
    add_subdirectory(communication)
endif()
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP)
    add_subdirectory(core)
endif()
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP)
    add_subdirectory(protocol)
    add_subdirectory(storage)
    add_subdirectory(calculation)
endif()
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP)
    add_subdirectory(visualization)
endif()
