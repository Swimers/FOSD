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
