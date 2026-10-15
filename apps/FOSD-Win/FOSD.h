#pragma once
#include <QMainWindow>
#include <memory>

namespace fosd {
namespace protocol { class ProtocolControl; }
}

class FOSD : public QMainWindow {
    Q_OBJECT

public:
    FOSD(QWidget* parent = nullptr);
    ~FOSD();

private:
    // ProtocolControl owns communication and its shared worker thread.
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
};
