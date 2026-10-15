#include "FOSD.h"
#include "protocol/ProtocolControl.h"

FOSD::FOSD(QWidget* parent)
    : QMainWindow(parent),
      m_protocol(new fosd::protocol::ProtocolControl)
{
}

FOSD::~FOSD()
{
}
