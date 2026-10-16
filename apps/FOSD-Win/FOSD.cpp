#include "FOSD.h"
#include "protocol/ProtocolControl.h"
#include "calculation/CalculationControl.h"

FOSD::FOSD(QWidget* parent)
    : QMainWindow(parent),
      m_protocol(new fosd::protocol::ProtocolControl),
      m_calculation(new fosd::Calculation::CalculationControl(m_protocol->dataModel()))
{
}

FOSD::~FOSD()
{
}
