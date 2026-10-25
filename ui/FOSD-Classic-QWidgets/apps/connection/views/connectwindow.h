#pragma once
#include <QWidget>
#include <memory>
#include "../../../common/IView.h"
namespace Ui { class connectwindow; }
namespace fosd { namespace ui { namespace classic {
class FormBinding;
// UI-only wrapper for connectwindow.ui; emits intent, never calls business Controls.
class ConnectionView : public QWidget, public IView {
    Q_OBJECT
public:
    explicit ConnectionView(QWidget *parent = nullptr);
    ~ConnectionView() override;
    ViewEvents *events() const override;
    QVariantMap values() const override;
    void applyFields(const QVariantMap &fields) override;
    void clearSensitiveFields() override;
    void clearInputs() override;
    void showStatus(const QString &message) override;
signals:
    void cbox_CampingFreRequested(const QVariantMap &fields);
    void cbox_CommuteChooseRequested(const QVariantMap &fields);
    void cbox_LocakIPRequested(const QVariantMap &fields);
    void cbox_ModleSelectRequested(const QVariantMap &fields);
    void cbox_SaveCycleRequested(const QVariantMap &fields);
    void cbox_SerialBaudChooseRequested(const QVariantMap &fields);
    void cbox_SerialNameChooseRequested(const QVariantMap &fields);
    void ckbox_CH1Requested(const QVariantMap &fields);
    void ckbox_CH10Requested(const QVariantMap &fields);
    void ckbox_CH11Requested(const QVariantMap &fields);
    void ckbox_CH12Requested(const QVariantMap &fields);
    void ckbox_CH13Requested(const QVariantMap &fields);
    void ckbox_CH14Requested(const QVariantMap &fields);
    void ckbox_CH15Requested(const QVariantMap &fields);
    void ckbox_CH16Requested(const QVariantMap &fields);
    void ckbox_CH2Requested(const QVariantMap &fields);
    void ckbox_CH3Requested(const QVariantMap &fields);
    void ckbox_CH4Requested(const QVariantMap &fields);
    void ckbox_CH5Requested(const QVariantMap &fields);
    void ckbox_CH6Requested(const QVariantMap &fields);
    void ckbox_CH7Requested(const QVariantMap &fields);
    void ckbox_CH8Requested(const QVariantMap &fields);
    void ckbox_CH9Requested(const QVariantMap &fields);
    void pbtn_CmdDebugRequested(const QVariantMap &fields);
    void pbtn_CmdFbgRequested(const QVariantMap &fields);
    void pbtn_CmdResetRequested(const QVariantMap &fields);
    void pbtn_CmdSerialRequested(const QVariantMap &fields);
    void pbtn_CmdStartRequested(const QVariantMap &fields);
    void pbtn_CmdUdpRequested(const QVariantMap &fields);
    void pbtn_DiagnosisRequested(const QVariantMap &fields);
    void pbtn_OpenNetRequested(const QVariantMap &fields);
    void pbtn_OpenSerialRequested(const QVariantMap &fields);
    void pbtn_ParaRestRequested(const QVariantMap &fields);
    void pbtn_ParaSetRequested(const QVariantMap &fields);
    void pbtn_RefreshSerialRequested(const QVariantMap &fields);
    void pbtn_SaveDynamicRequested(const QVariantMap &fields);
    void pbtn_SaveSpectrumRequested(const QVariantMap &fields);
    void pbtn_SaveStaticRequested(const QVariantMap &fields);
    void pbtn_SelectAllCHRequested(const QVariantMap &fields);
    void pbtn_SelectCHRequested(const QVariantMap &fields);
    void pbtn_SerialClearRequested(const QVariantMap &fields);
    void pbtn_SerialSendRequested(const QVariantMap &fields);
    void pbtn_TcpClearRequested(const QVariantMap &fields);
    void pbtn_TcpSendRequested(const QVariantMap &fields);
    void pbtn_UdpClearRequested(const QVariantMap &fields);
    void pbtn_UdpSendRequested(const QVariantMap &fields);
    void pbtn_WaveLengthResetRequested(const QVariantMap &fields);
private slots:
    void relayControl(fosd::ui::classic::PanelId panel, const QString &action);
private:
    std::unique_ptr<Ui::connectwindow> m_ui;
    FormBinding *m_binding = nullptr;
};
} } }
