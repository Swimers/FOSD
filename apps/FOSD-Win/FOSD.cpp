#include "FOSD.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "FOSD-Classic-QWidgets/apps/mainwindow/presenters/WorkbenchPresenter.h"

FOSD::FOSD(QWidget *parent)
    : fosd::ui::classic::WorkbenchView(parent), m_protocol(new fosd::protocol::ProtocolControl),
      m_calculation(new fosd::Calculation::CalculationControl(m_protocol->dataModel())),
      m_backends(new fosd::ui::classic::WorkbenchBackend),
      m_presenter(new fosd::ui::classic::WorkbenchPresenter(this)) {
    m_presenter->setBackends(m_backends.get());
    connect(m_protocol.get(), &fosd::protocol::ProtocolControl::frameParsed,
            &m_calculation->storage(), &fosd::storage::StorageControl::writeWavelengthFrame);
}

FOSD::~FOSD() = default;
void FOSD::setUiBackend(fosd::ui::classic::FeatureId feature,
                        fosd::ui::classic::IBackend *backend) {
    m_backends->setBackend(feature, backend);
}
