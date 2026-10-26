#pragma once
#include "FOSD-Classic-QWidgets/apps/WorkbenchView.h"
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
class WorkbenchBackend;
class IBackend;
} // namespace classic
} // namespace ui
} // namespace fosd
class FOSD : public fosd::ui::classic::WorkbenchView {
    Q_OBJECT
public:
    explicit FOSD(QWidget *parent = nullptr);
    ~FOSD() override;
    void setUiBackend(fosd::ui::classic::FeatureId feature, fosd::ui::classic::IBackend *backend);

private:
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
    std::unique_ptr<fosd::Calculation::CalculationControl> m_calculation;
    std::unique_ptr<fosd::ui::classic::WorkbenchBackend> m_backends;
    std::unique_ptr<fosd::ui::classic::WorkbenchPresenter> m_presenter;
};
