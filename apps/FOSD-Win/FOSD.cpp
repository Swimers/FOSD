#include "FOSD.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "FOSDWidgets/presenters/WorkbenchPresenter.h"

FOSD::FOSD(QWidget* parent)
    : fosd::widgets::WorkbenchView(parent),
      m_protocol(new fosd::protocol::ProtocolControl),
      m_calculation(new fosd::Calculation::CalculationControl(m_protocol->dataModel())),
      m_presenter(new fosd::widgets::WorkbenchPresenter(this))
{
    connect(this, &fosd::widgets::WorkbenchView::actionTriggered,
            m_presenter.get(), &fosd::widgets::WorkbenchPresenter::execute);
    // A later application adapter can connect these UI requests to Controls
    // through WorkbenchPresenter::setBackend(). No legacy backend is created.
    // Save every parsed wavelength frame, independently of latest-frame polling.
    connect(m_protocol.get(), &fosd::protocol::ProtocolControl::frameParsed,
            &m_calculation->storage(), &fosd::storage::StorageControl::writeWavelengthFrame);
}

FOSD::~FOSD()
{
}

void FOSD::setUiBackend(fosd::widgets::IWorkbenchBackend *backend) {
    m_presenter->setBackend(backend);
}
