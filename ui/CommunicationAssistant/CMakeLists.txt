add_library(fosd_assistant_presenter STATIC
    CommunicationPresenter.cpp CommunicationPresenter.h)
target_include_directories(fosd_assistant_presenter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_assistant_presenter PUBLIC fosd_communication)

if(FOSD_ASSISTANT_BUILD_TESTS OR FOSD_WIDGETS_ASSISTANT_BUILD_TESTS)
    add_executable(test_assistant_presenter tests/PresenterTests.cpp)
    target_link_libraries(test_assistant_presenter PRIVATE fosd_assistant_presenter Qt5::Test)
    add_test(NAME test_assistant_presenter COMMAND test_assistant_presenter)
endif()
