add_executable(test_classic_widgets WorkbenchTests.cpp)
target_link_libraries(test_classic_widgets PRIVATE fosd_classic_widgets Qt5::Test)
add_test(NAME test_classic_widgets COMMAND test_classic_widgets)
set_tests_properties(test_classic_widgets PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
if(WIN32)
    set_property(TEST test_classic_widgets APPEND PROPERTY ENVIRONMENT "QT_QPA_FONTDIR=$ENV{WINDIR}/Fonts")
endif()

if(TARGET fosd_classic_application)
    add_executable(test_workbench_model workbench_model_tests.cpp workbench_test_packets.h)
    target_link_libraries(test_workbench_model PRIVATE fosd_classic_model fosd_classic_presenters Qt5::Test)
    add_test(NAME test_workbench_model COMMAND test_workbench_model)

    add_executable(test_workbench_markers workbench_marker_tests.cpp)
    target_link_libraries(test_workbench_markers PRIVATE fosd_classic_model
        fosd_classic_widgets fosd_classic_presenters Qt5::Test)
    add_test(NAME test_workbench_markers COMMAND test_workbench_markers)

    add_executable(test_workbench_main workbench_main_tests.cpp)
    target_link_libraries(test_workbench_main PRIVATE fosd_classic_application Qt5::Test)
    add_test(NAME test_workbench_main COMMAND test_workbench_main -o workbench-main-test.txt,txt)
endif()
