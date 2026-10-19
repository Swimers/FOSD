# Unit tests. Each test file becomes one console executable and is registered
# with CTest under its file stem.
#
# Run with:
#   cmake --build build --config Debug
#   ctest --test-dir build -C Debug --output-on-failure

function(fosd_add_test name)
    add_executable(${name} ${ARGN})

    target_link_libraries(${name}
        PRIVATE
            Qt5::Test
    )

    # QTEST_MAIN provides main(); the executable must stay a console app so the
    # test output is visible and CTest can capture it.
    set_target_properties(${name} PROPERTIES WIN32_EXECUTABLE OFF)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

fosd_add_test(test_communication
    communication_tests.cpp
)

target_link_libraries(test_communication PRIVATE fosd_communication)

fosd_add_test(test_packet_decoders packet_decoder_tests.cpp)
target_link_libraries(test_packet_decoders PRIVATE fosd_communication)

fosd_add_test(test_protocol protocol_tests.cpp)
target_link_libraries(test_protocol PRIVATE fosd_protocol)

# Exercise JCS parsing directly, without changing the production parser factory.
fosd_add_test(test_jcs_wavelength_parser jcs_wavelength_parser_tests.cpp)
target_link_libraries(test_jcs_wavelength_parser PRIVATE fosd_protocol)

fosd_add_test(test_jcs_channel_parser jcs_channel_parser_tests.cpp)
target_link_libraries(test_jcs_channel_parser PRIVATE fosd_protocol)

fosd_add_test(test_calculation calculation_tests.cpp)
target_link_libraries(test_calculation PRIVATE fosd_calculation fosd_protocol)

fosd_add_test(test_storage storage_tests.cpp)
target_link_libraries(test_storage PRIVATE fosd_calculation fosd_protocol)

# Real loopback UDP/TCP -> decoder -> MP parser -> calibration/HYY -> storage.
fosd_add_test(test_end_to_end end_to_end_tests.cpp)
target_link_libraries(test_end_to_end PRIVATE fosd_calculation fosd_protocol)
