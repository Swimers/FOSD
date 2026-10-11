add_library(fosd_protocol STATIC

)

# Consumers include these headers as "communication/ConnectControl.h", so the
# public include root is `src/`, not this directory.
target_include_directories(fosd_protocol
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

# Qt is part of the module's public interface: TransportTypes.h exposes
# QSerialPort/QHostAddress and the signals use Qt types.
target_link_libraries(fosd_protocol
    PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::SerialPort
    PRIVATE
        fosd_communication
)
