#pragma once
#include "FOSDWidgets/views/WorkbenchView.h"
#include <memory>

namespace fosd {
namespace protocol { class ProtocolControl; }
namespace Calculation { class CalculationControl; }
namespace widgets { class WorkbenchPresenter; class IWorkbenchBackend; }
}

class FOSD : public fosd::widgets::WorkbenchView {
    Q_OBJECT

public:
    FOSD(QWidget* parent = nullptr);
    ~FOSD();
    void setUiBackend(fosd::widgets::IWorkbenchBackend *backend);

private:
    // ProtocolControl owns communication and its shared worker thread.
    std::unique_ptr<fosd::protocol::ProtocolControl> m_protocol;
    std::unique_ptr<fosd::Calculation::CalculationControl> m_calculation;
    std::unique_ptr<fosd::widgets::WorkbenchPresenter> m_presenter;
};
