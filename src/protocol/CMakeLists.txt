add_library(fosd_protocol STATIC
    IFrameParser.h
    MPWavelengthParser.h
    MPWavelengthParser.cpp
    JCSWavelengthParser.h
    JCSWavelengthParser.cpp
    ProtocolControl.h
    ProtocolControl.cpp
)

# Consumers include these headers as "protocol/ProtocolControl.h", so the
# public include root is `src/`, not this directory.
target_include_directories(fosd_protocol
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_link_libraries(fosd_protocol
    PUBLIC
        Qt5::Core
        fosd_core
        fosd_communication
)
