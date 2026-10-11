#include "FOSD.h"
#include "communication/ConnectControl.h"
#include "protocol/ProtocolControl.h"

FOSD::FOSD(QWidget* parent)
    : QMainWindow(parent),
      m_communication(new fosd::communication::ConnectControl),
      m_protocol(new fosd::protocol::ProtocolControl(m_communication.get()))
{
}

FOSD::~FOSD()
{
}
