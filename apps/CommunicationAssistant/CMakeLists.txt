add_executable(CommunicationAssistant WIN32
    main.cpp
    ../../ui/CommunicationAssistant/resources.qrc)
target_link_libraries(CommunicationAssistant PRIVATE
    fosd_assistant_presenter Qt5::Qml Qt5::Quick Qt5::QuickControls2)

if(FOSD_ASSISTANT_BUILD_TESTS)
    add_executable(test_assistant_qml
        ../../ui/CommunicationAssistant/tests/QmlTests.cpp
        ../../ui/CommunicationAssistant/resources.qrc)
    target_link_libraries(test_assistant_qml PRIVATE
        fosd_assistant_presenter Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Test)
    add_test(NAME test_assistant_qml COMMAND test_assistant_qml)
    set_tests_properties(test_assistant_qml PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_QUICK_BACKEND=software"
        TIMEOUT 30)
    if(WIN32)
        set_property(TEST test_assistant_qml APPEND PROPERTY
            ENVIRONMENT "QT_QPA_FONTDIR=$ENV{WINDIR}/Fonts")
    endif()
endif()
