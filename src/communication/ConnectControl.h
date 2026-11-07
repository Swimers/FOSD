#pragma once

#include "IPacketDecoder.h"
#include "ITransportTypes.h"

#include <QObject>
#include <QThread>
#include <QPointer>
#include <functional>
#include <memory>

namespace fosd {
namespace protocol {
class ProtocolControl;
}

namespace communication {
class ITransport;
class Udp;
class Tcp;
class Serialport;

using DecoderFactory = std::function<std::unique_ptr<IPacketDecoder>(Channel)>;

namespace detail {
class ConnectWorker final : public QObject {
    Q_OBJECT
public:
    explicit ConnectWorker(DecoderFactory decoderFactory);

public slots:
    void initialize();
    void openSerial(const fosd::communication::SerialConfig &config);
    void openUdp(const fosd::communication::UdpConfig &config);
    void openTcp(const fosd::communication::TcpConfig &config);
    void close(fosd::communication::Channel channel);
    void send(fosd::communication::Channel channel, const QByteArray &bytes,
              const fosd::communication::Endpoint &peer);
    void shutdown();

signals:
    void commandReceived(fosd::communication::Channel channel, const QByteArray &command,
                         const fosd::communication::Endpoint &peer);
    void rawDataReceived(fosd::communication::Channel channel, const QByteArray &bytes,
                         const fosd::communication::Endpoint &peer);
    void packetReceived(fosd::communication::Channel channel, const QByteArray &packet,
                        const fosd::communication::Endpoint &peer);
    void stateChanged(fosd::communication::Channel channel,
                      fosd::communication::ConnectionState state);
    void errorOccurred(const fosd::communication::CommunicationError &error);
    void decoderErrorOccurred(const fosd::communication::DecoderError &error);

private slots:
    void handleBytesReceived(const QByteArray &bytes, const fosd::communication::Endpoint &peer);
    void handleTransportStateChanged(fosd::communication::ConnectionState state);

private:
    void wire(ITransport *transport);
    std::unique_ptr<IPacketDecoder> initializeDecoder(Channel channel);
    void forward(fosd::communication::Channel channel, const QByteArray &bytes,
                 const fosd::communication::Endpoint &peer);
    IPacketDecoder *decoder(fosd::communication::Channel channel);

    DecoderFactory m_decoderFactory;
    Udp *m_udp = nullptr;
    Tcp *m_tcp = nullptr;
    Serialport *m_serialport = nullptr;

    std::unique_ptr<IPacketDecoder> m_udpDecoder;
    std::unique_ptr<IPacketDecoder> m_tcpDecoder;
    std::unique_ptr<IPacketDecoder> m_serialportDecoder;
};

} // namespace detail

// Create and destroy on the caller/UI thread. The hosted constructor requires
// an unstarted thread which outlives this control; its owner starts/stops it.
// An empty factory selects the built-in UDP/TCP/serial decoders. A supplied
// factory replaces that policy; returning nullptr disables decoding per channel.
class ConnectControl final : public QObject {
    Q_OBJECT
public:
    explicit ConnectControl(QObject *parent = nullptr,
                            DecoderFactory decoderFactory = DecoderFactory());
    explicit ConnectControl(QThread &hostThread, QObject *parent = nullptr,
                            DecoderFactory decoderFactory = DecoderFactory());
    ~ConnectControl() override;

    void openSerial(const SerialConfig &config);
    void openUdp(const UdpConfig &config);
    void openTcp(const TcpConfig &config);
    void close(Channel channel);
    void send(Channel channel, const QByteArray &bytes, const Endpoint &peer = Endpoint());

signals:
    void commandReceived(fosd::communication::Channel channel, const QByteArray &command,
                         const fosd::communication::Endpoint &peer);
    void rawDataReceived(fosd::communication::Channel channel, const QByteArray &bytes,
                         const fosd::communication::Endpoint &peer);
    void packetReceived(fosd::communication::Channel channel, const QByteArray &packet,
                        const fosd::communication::Endpoint &peer);
    void stateChanged(fosd::communication::Channel channel,
                      fosd::communication::ConnectionState state);
    void errorOccurred(const fosd::communication::CommunicationError &error);
    void decoderErrorOccurred(const fosd::communication::DecoderError &error);

    void requestOpenSerial(const fosd::communication::SerialConfig &config);
    void requestOpenUdp(const fosd::communication::UdpConfig &config);
    void requestOpenTcp(const fosd::communication::TcpConfig &config);
    void requestClose(fosd::communication::Channel channel);
    void requestSend(fosd::communication::Channel channel, const QByteArray &bytes,
                     const fosd::communication::Endpoint &peer);

private:
    friend class fosd::protocol::ProtocolControl;

    ConnectControl(QObject *parent, DecoderFactory decoderFactory, QThread *hostThread);
    void shutdown();

    std::unique_ptr<QThread> m_ownedThread;

    QThread *m_thread = nullptr;
    QPointer<detail::ConnectWorker> m_worker;
};

} // namespace communication
} // namespace fosd
