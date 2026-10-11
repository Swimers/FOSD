add_library(fosd_communication STATIC
    ConnectControl.cpp
    ConnectControl.h
    ITransport.h
    IPacketDecoder.h
    TransportTypes.h
    Tcp.cpp
    Tcp.h
    Udp.cpp
    Udp.h
    Usart.cpp
    Usart.h
)

# Consumers include these headers as "communication/ConnectControl.h", so the
# public include root is `src/`, not this directory.
target_include_directories(fosd_communication
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

# Qt is part of the module's public interface: TransportTypes.h exposes
# QSerialPort/QHostAddress and the signals use Qt types.
target_link_libraries(fosd_communication
    PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::SerialPort
)
