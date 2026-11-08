#pragma once
#include "FOSD-Classic-QWidgets/application/WorkbenchView.h"
#include <memory>
namespace fosd {
namespace protocol {
class ProtocolControl;
}

namespace Calculation {
class CalculationControl;
}

namespace ui {
namespace classic {
class WorkbenchPresenter;
} // namespace classic
} // namespace ui
} // namespace fosd
class FOSD : public fosd::ui::classic::WorkbenchView {
    Q_OBJECT
public:
    explicit FOSD(QWidget *parent = nullptr);
    ~FOSD() override;

private:
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
    std::unique_ptr<fosd::Calculation::CalculationControl> m_calculation;
    std::unique_ptr<fosd::ui::classic::WorkbenchPresenter> m_presenter;
};
