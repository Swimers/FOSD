add_executable(FOSD
    WIN32
    main.cpp
    FOSD.cpp
    FOSD.h
)

target_link_libraries(FOSD
    PRIVATE
        Qt5::Widgets
        fosd_classic_widgets
        fosd_classic_presenters
        fosd_communication
        fosd_protocol
        fosd_calculation
        fosd_visualization
)

# Qt5::Widgets already implies Qt5::Core / Qt5::Gui.
target_compile_features(FOSD PRIVATE cxx_std_11)
