#pragma once
#include <QMainWindow>
#include <memory>

namespace fosd {
namespace protocol { class ProtocolControl; }
namespace Calculation { class CalculationControl; }
}

class FOSD : public QMainWindow {
    Q_OBJECT

public:
    FOSD(QWidget* parent = nullptr);
    ~FOSD();

private:
    // ProtocolControl owns communication and its shared worker thread.
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
    std::unique_ptr<fosd::Calculation::CalculationControl> m_calculation;
};
