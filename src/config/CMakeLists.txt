add_library(fosd_config STATIC
    IConfigStore.h
    IniConfigStore.h
    IniConfigStore.cpp
)
target_include_directories(fosd_config PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(fosd_config PUBLIC Qt5::Core)
target_compile_features(fosd_config PUBLIC cxx_std_11)

# Typed module parameters use transport enums/addresses, but never link back to
# business implementations. The generic fosd_config target remains Qt Core only.
find_package(Qt5 5.12 REQUIRED COMPONENTS Network SerialPort)
add_library(fosd_parameters STATIC
    CoreParameters.h
    ParameterCodec.h
    ParameterCodec.cpp
    ParameterDefaults.cpp
    ParameterValidation.cpp
    ParameterStore.h
    ParameterStore.cpp
    parameter_defaults.qrc
)
target_link_libraries(fosd_parameters PUBLIC fosd_config Qt5::Network Qt5::SerialPort)
set(_fosd_parameter_bin "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if(CMAKE_CONFIGURATION_TYPES)
    string(APPEND _fosd_parameter_bin "/$<CONFIG>")
endif()
add_custom_target(fosd_parameter_templates
    COMMAND ${CMAKE_COMMAND} -E make_directory "${_fosd_parameter_bin}/config"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/core-defaults.ini"
        "${_fosd_parameter_bin}/config/core-defaults.ini"
)
add_dependencies(fosd_parameters fosd_parameter_templates)
add_executable(generate_parameter_defaults EXCLUDE_FROM_ALL DefaultsGenerator.cpp)
target_link_libraries(generate_parameter_defaults PRIVATE fosd_parameters)
