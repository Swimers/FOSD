#pragma once
#include <QMainWindow>
#include <memory>

namespace fosd {
namespace communication { class ConnectControl; }
namespace protocol { class ProtocolControl; }
}

class FOSD : public QMainWindow {
    Q_OBJECT

public:
    FOSD(QWidget* parent = nullptr);
    ~FOSD();

private:
    // Stop protocol consumers before communication producers on destruction.
    std::unique_ptr<fosd::communication::ConnectControl> m_communication;
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
};
