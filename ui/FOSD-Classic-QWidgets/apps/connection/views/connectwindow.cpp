#include "connectwindow.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_connectwindow.h"
namespace fosd { namespace ui { namespace classic {
ConnectionView::ConnectionView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::Connection, this, this);
    connect(m_binding, &ViewEvents::actionTriggered, this, &ConnectionView::relayControl);
}
ConnectionView::~ConnectionView() = default;
ViewEvents *ConnectionView::events() const { return m_binding; }
QVariantMap ConnectionView::values() const { return m_binding->values(); }
void ConnectionView::applyFields(const QVariantMap &fields) { m_binding->applyFields(fields); }
void ConnectionView::clearSensitiveFields() { m_binding->clearSensitiveFields(); }
void ConnectionView::clearInputs() { m_binding->clearInputs(); }
void ConnectionView::showStatus(const QString &message) { m_binding->showStatus(message); }
void ConnectionView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Connection) return;
    if (action == "cbox_CampingFre") { emit cbox_CampingFreRequested(values()); return; }
    if (action == "cbox_CommuteChoose") { emit cbox_CommuteChooseRequested(values()); return; }
    if (action == "cbox_LocakIP") { emit cbox_LocakIPRequested(values()); return; }
    if (action == "cbox_ModleSelect") { emit cbox_ModleSelectRequested(values()); return; }
    if (action == "cbox_SaveCycle") { emit cbox_SaveCycleRequested(values()); return; }
    if (action == "cbox_SerialBaudChoose") { emit cbox_SerialBaudChooseRequested(values()); return; }
    if (action == "cbox_SerialNameChoose") { emit cbox_SerialNameChooseRequested(values()); return; }
    if (action == "ckbox_CH1") { emit ckbox_CH1Requested(values()); return; }
    if (action == "ckbox_CH10") { emit ckbox_CH10Requested(values()); return; }
    if (action == "ckbox_CH11") { emit ckbox_CH11Requested(values()); return; }
    if (action == "ckbox_CH12") { emit ckbox_CH12Requested(values()); return; }
    if (action == "ckbox_CH13") { emit ckbox_CH13Requested(values()); return; }
    if (action == "ckbox_CH14") { emit ckbox_CH14Requested(values()); return; }
    if (action == "ckbox_CH15") { emit ckbox_CH15Requested(values()); return; }
    if (action == "ckbox_CH16") { emit ckbox_CH16Requested(values()); return; }
    if (action == "ckbox_CH2") { emit ckbox_CH2Requested(values()); return; }
    if (action == "ckbox_CH3") { emit ckbox_CH3Requested(values()); return; }
    if (action == "ckbox_CH4") { emit ckbox_CH4Requested(values()); return; }
    if (action == "ckbox_CH5") { emit ckbox_CH5Requested(values()); return; }
    if (action == "ckbox_CH6") { emit ckbox_CH6Requested(values()); return; }
    if (action == "ckbox_CH7") { emit ckbox_CH7Requested(values()); return; }
    if (action == "ckbox_CH8") { emit ckbox_CH8Requested(values()); return; }
    if (action == "ckbox_CH9") { emit ckbox_CH9Requested(values()); return; }
    if (action == "pbtn_CmdDebug") { emit pbtn_CmdDebugRequested(values()); return; }
    if (action == "pbtn_CmdFbg") { emit pbtn_CmdFbgRequested(values()); return; }
    if (action == "pbtn_CmdReset") { emit pbtn_CmdResetRequested(values()); return; }
    if (action == "pbtn_CmdSerial") { emit pbtn_CmdSerialRequested(values()); return; }
    if (action == "pbtn_CmdStart") { emit pbtn_CmdStartRequested(values()); return; }
    if (action == "pbtn_CmdUdp") { emit pbtn_CmdUdpRequested(values()); return; }
    if (action == "pbtn_Diagnosis") { emit pbtn_DiagnosisRequested(values()); return; }
    if (action == "pbtn_OpenNet") { emit pbtn_OpenNetRequested(values()); return; }
    if (action == "pbtn_OpenSerial") { emit pbtn_OpenSerialRequested(values()); return; }
    if (action == "pbtn_ParaRest") { emit pbtn_ParaRestRequested(values()); return; }
    if (action == "pbtn_ParaSet") { emit pbtn_ParaSetRequested(values()); return; }
    if (action == "pbtn_RefreshSerial") { emit pbtn_RefreshSerialRequested(values()); return; }
    if (action == "pbtn_SaveDynamic") { emit pbtn_SaveDynamicRequested(values()); return; }
    if (action == "pbtn_SaveSpectrum") { emit pbtn_SaveSpectrumRequested(values()); return; }
    if (action == "pbtn_SaveStatic") { emit pbtn_SaveStaticRequested(values()); return; }
    if (action == "pbtn_SelectAllCH") { emit pbtn_SelectAllCHRequested(values()); return; }
    if (action == "pbtn_SelectCH") { emit pbtn_SelectCHRequested(values()); return; }
    if (action == "pbtn_SerialClear") { emit pbtn_SerialClearRequested(values()); return; }
    if (action == "pbtn_SerialSend") { emit pbtn_SerialSendRequested(values()); return; }
    if (action == "pbtn_TcpClear") { emit pbtn_TcpClearRequested(values()); return; }
    if (action == "pbtn_TcpSend") { emit pbtn_TcpSendRequested(values()); return; }
    if (action == "pbtn_UdpClear") { emit pbtn_UdpClearRequested(values()); return; }
    if (action == "pbtn_UdpSend") { emit pbtn_UdpSendRequested(values()); return; }
    if (action == "pbtn_WaveLengthReset") { emit pbtn_WaveLengthResetRequested(values()); return; }
}
} } }
