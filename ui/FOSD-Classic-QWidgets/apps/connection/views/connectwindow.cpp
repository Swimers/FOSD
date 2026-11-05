#include "connectwindow.h"
#include "../../../common/FormBinding.h"
#include "../../../resources/UiResources.h"
#include "ui_connectwindow.h"
#include <QLineEdit>
#include <QPushButton>
namespace fosd {
namespace ui {
namespace classic {
ConnectionView::ConnectionView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_ui->ptedit_UdpRec->setMaximumBlockCount(200);
    m_ui->ptedit_UdpRec->setUndoRedoEnabled(false);
    m_ui->ptedit_UdpRec->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_ui->label_14->setText(tr("UDP 接收日志（有效包显示摘要，每60秒自动清理）"));
    m_ui->cbox_UdpDisplayMode->setToolTip(tr("有效包显示接收时间、协议、序号和记录数；此选项仅切换未识别包的预览格式"));
    const QStringList defaults{QStringLiteral("udp"), QStringLiteral("reset"),
                               QStringLiteral("fbg"), QStringLiteral("start")};
    for (int i = 0; i < 10; ++i) {
        auto *edit = new QLineEdit(this);
        edit->setObjectName(QStringLiteral("edit_SerialCommand%1").arg(i));
        edit->setPlaceholderText(tr("编辑快捷指令"));
        edit->setToolTip(tr("字符串指令；发送时自动追加回车换行，编辑后自动保存"));
        if (i < defaults.size())
            edit->setText(defaults[i]);
        auto *button = new QPushButton(QString::number(i), this);
        button->setObjectName(QStringLiteral("pbtn_SerialCommand%1").arg(i));
        button->setToolTip(tr("发送第 %1 组指令（自动追加 CRLF）").arg(i));
        button->setFixedWidth(42);
        m_ui->gridLayout_PresetCmd->addWidget(edit, i % 5, (i / 5) * 2);
        m_ui->gridLayout_PresetCmd->addWidget(button, i % 5, (i / 5) * 2 + 1);
    }
    m_ui->gridLayout_PresetCmd->setColumnStretch(0, 1);
    m_ui->gridLayout_PresetCmd->setColumnStretch(2, 1);
    m_ui->ptedit_SerialSend->setPlaceholderText(tr("输入串口指令，发送时自动追加 CRLF（回车换行）"));
    m_binding = new FormBinding(PanelId::Connection, this, this);
    for (auto *edit : findChildren<QLineEdit *>()) {
        if (edit->objectName().startsWith("edit_SerialCommand"))
            connect(edit, &QLineEdit::editingFinished, this, [this, edit]() {
                emit m_binding->actionTriggered(PanelId::Connection, edit->objectName());
            });
    }
    connect(m_binding, &ViewEvents::actionTriggered, this, &ConnectionView::relayControl);
}

ConnectionView::~ConnectionView() = default;
ViewEvents *ConnectionView::events() const {
    return m_binding;
}

QVariantMap ConnectionView::values() const {
    return m_binding->values();
}

void ConnectionView::applyFields(const QVariantMap &fields) {
    m_binding->applyFields(fields);
}

void ConnectionView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ConnectionView::clearInputs() {
    m_binding->clearInputs();
}

void ConnectionView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void ConnectionView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Connection)
        return;
    if (action == "cbox_CommuteChoose") {
        emit cbox_CommuteChooseRequested(values());
        return;
    }
    if (action == "cbox_LocakIP") {
        emit cbox_LocakIPRequested(values());
        return;
    }
    if (action == "cbox_SerialBaudChoose") {
        emit cbox_SerialBaudChooseRequested(values());
        return;
    }
    if (action == "cbox_SerialNameChoose") {
        emit cbox_SerialNameChooseRequested(values());
        return;
    }
    if (action == "ckbox_CH1") {
        emit ckbox_CH1Requested(values());
        return;
    }
    if (action == "ckbox_CH10") {
        emit ckbox_CH10Requested(values());
        return;
    }
    if (action == "ckbox_CH11") {
        emit ckbox_CH11Requested(values());
        return;
    }
    if (action == "ckbox_CH12") {
        emit ckbox_CH12Requested(values());
        return;
    }
    if (action == "ckbox_CH13") {
        emit ckbox_CH13Requested(values());
        return;
    }
    if (action == "ckbox_CH14") {
        emit ckbox_CH14Requested(values());
        return;
    }
    if (action == "ckbox_CH15") {
        emit ckbox_CH15Requested(values());
        return;
    }
    if (action == "ckbox_CH16") {
        emit ckbox_CH16Requested(values());
        return;
    }
    if (action == "ckbox_CH2") {
        emit ckbox_CH2Requested(values());
        return;
    }
    if (action == "ckbox_CH3") {
        emit ckbox_CH3Requested(values());
        return;
    }
    if (action == "ckbox_CH4") {
        emit ckbox_CH4Requested(values());
        return;
    }
    if (action == "ckbox_CH5") {
        emit ckbox_CH5Requested(values());
        return;
    }
    if (action == "ckbox_CH6") {
        emit ckbox_CH6Requested(values());
        return;
    }
    if (action == "ckbox_CH7") {
        emit ckbox_CH7Requested(values());
        return;
    }
    if (action == "ckbox_CH8") {
        emit ckbox_CH8Requested(values());
        return;
    }
    if (action == "ckbox_CH9") {
        emit ckbox_CH9Requested(values());
        return;
    }
    if (action == "pbtn_Diagnosis") {
        emit pbtn_DiagnosisRequested(values());
        return;
    }
    if (action == "pbtn_OpenNet") {
        emit pbtn_OpenNetRequested(values());
        return;
    }
    if (action == "pbtn_OpenSerial") {
        emit pbtn_OpenSerialRequested(values());
        return;
    }
    if (action == "pbtn_ParaRest") {
        emit pbtn_ParaRestRequested(values());
        return;
    }
    if (action == "pbtn_ParaSet") {
        emit pbtn_ParaSetRequested(values());
        return;
    }
    if (action == "pbtn_RefreshSerial") {
        emit pbtn_RefreshSerialRequested(values());
        return;
    }
    if (action == "pbtn_SaveDynamic") {
        emit pbtn_SaveDynamicRequested(values());
        return;
    }
    if (action == "pbtn_SaveSpectrum") {
        emit pbtn_SaveSpectrumRequested(values());
        return;
    }
    if (action == "pbtn_SaveStatic") {
        emit pbtn_SaveStaticRequested(values());
        return;
    }
    if (action == "pbtn_SelectAllCH") {
        emit pbtn_SelectAllCHRequested(values());
        return;
    }
    if (action == "pbtn_SelectCH") {
        emit pbtn_SelectCHRequested(values());
        return;
    }
    if (action == "pbtn_SerialClear") {
        emit pbtn_SerialClearRequested(values());
        return;
    }
    if (action == "pbtn_SerialSend") {
        emit pbtn_SerialSendRequested(values());
        return;
    }
    if (action == "pbtn_TcpClear") {
        emit pbtn_TcpClearRequested(values());
        return;
    }
    if (action == "pbtn_TcpSend") {
        emit pbtn_TcpSendRequested(values());
        return;
    }
    if (action == "pbtn_UdpClear") {
        emit pbtn_UdpClearRequested(values());
        return;
    }
    if (action == "pbtn_UdpSend") {
        emit pbtn_UdpSendRequested(values());
        return;
    }
    if (action == "pbtn_WaveLengthReset") {
        emit pbtn_WaveLengthResetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
