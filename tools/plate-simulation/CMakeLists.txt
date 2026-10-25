add_library(fosd_plate_simulation STATIC
    PlateSimulation.h PlateSimulation.cpp PlateIFE.h PlateIFE.cpp
    PlateUdpSender.h PlateUdpSender.cpp)
target_include_directories(fosd_plate_simulation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_compile_definitions(fosd_plate_simulation PRIVATE
    FOSD_PLATE_MODEL_FILE="${CMAKE_CURRENT_SOURCE_DIR}/models/plate.stl")
target_link_libraries(fosd_plate_simulation PUBLIC fosd_calculation Qt5::Network)

add_executable(plate-udp-sender sender_main.cpp)
target_link_libraries(plate-udp-sender PRIVATE fosd_plate_simulation)

if(FOSD_BUILD_TESTS OR FOSD_VTK_NATIVE_BUILD_TESTS)
    add_executable(test_plate_simulation tests.cpp)
    target_link_libraries(test_plate_simulation PRIVATE fosd_plate_simulation fosd_protocol Qt5::Test)
    add_dependencies(test_plate_simulation plate-udp-sender)
    add_test(NAME test_plate_simulation COMMAND test_plate_simulation)
endif()
