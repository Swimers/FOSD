#include "FOSD.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"

FOSD::FOSD(QWidget* parent)
    : QMainWindow(parent),
      m_protocol(new fosd::protocol::ProtocolControl),
      m_calculation(new fosd::Calculation::CalculationControl(m_protocol->dataModel()))
{
    // Save every parsed wavelength frame, independently of latest-frame polling.
    connect(m_protocol.get(), &fosd::protocol::ProtocolControl::frameParsed,
            &m_calculation->storage(), &fosd::storage::StorageControl::writeWavelengthFrame);
}

FOSD::~FOSD()
{
}
