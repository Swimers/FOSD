#include "FOSD.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"
#include "FOSD-Classic-QWidgets/application/WorkbenchPresenter.h"

FOSD::FOSD(QWidget *parent)
    : fosd::ui::classic::WorkbenchView(parent), m_protocol(new fosd::protocol::ProtocolControl),
      m_calculation(new fosd::Calculation::CalculationControl(m_protocol->dataModel())),
      m_presenter(new fosd::ui::classic::WorkbenchPresenter(this)) {
    connect(m_protocol.get(), &fosd::protocol::ProtocolControl::frameParsed,
            &m_calculation->storage(), &fosd::storage::StorageControl::writeWavelengthFrame);
}

FOSD::~FOSD() = default;
