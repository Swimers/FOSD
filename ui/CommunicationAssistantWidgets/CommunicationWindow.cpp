#include "CommunicationWindow.h"
#include "communication-assistant-QML/CommunicationPresenter.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace fosd {
namespace assistant {
namespace {
QString trText(const char *text) {
    return QString::fromUtf8(text);
}

QLineEdit *field(const char *name, const QString &text, bool port = false) {
    QLineEdit *edit = new QLineEdit(text);
    edit->setObjectName(QString::fromLatin1(name));
    if (port)
        edit->setValidator(new QIntValidator(1, 65535, edit));
    return edit;
}

QComboBox *choices(const char *name, const QStringList &items) {
    QComboBox *combo = new QComboBox;
    combo->setObjectName(QString::fromLatin1(name));
    combo->addItems(items);
    return combo;
}
} // namespace

CommunicationWindow::CommunicationWindow(CommunicationPresenter &presenter, QWidget *parent)
    : QMainWindow(parent), m_presenter(presenter) {
    createUi();
    bindPresenter();
    renderPorts();
    renderSelection();
    renderHexDisplay();
    renderLog();
    renderCounters();
    renderError();
}

QWidget *CommunicationWindow::createSerialPage() {
    QWidget *page = new QWidget;
    QFormLayout *form = new QFormLayout(page);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    m_serialPort = choices("serialPort", {});
    m_serialPort->setEditable(true);
    QPushButton *refresh = new QPushButton(trText("刷新"));
    QHBoxLayout *ports = new QHBoxLayout;
    ports->addWidget(m_serialPort, 1);
    ports->addWidget(refresh);
    connect(refresh, &QPushButton::clicked, &m_presenter,
            &CommunicationPresenter::refreshSerialPorts);
    form->addRow(trText("串口"), ports);
    m_baud = choices("baudRate",
                     {"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"});
    m_baud->setEditable(true);
    m_baud->setCurrentIndex(4);
    form->addRow(trText("波特率"), m_baud);
    m_dataBits = choices("dataBits", {"8", "7", "6", "5"});
    m_parity = choices("parity", {trText("无"), trText("偶"), trText("奇")});
    m_stopBits = choices("stopBits", {"1", "2"});
    m_flow =
        choices("flowControl", {trText("无流控"), trText("硬件 RTS/CTS"), trText("软件 XON/XOFF")});
    form->addRow(trText("数据位"), m_dataBits);
    form->addRow(trText("校验"), m_parity);
    form->addRow(trText("停止位"), m_stopBits);
    form->addRow(trText("流控"), m_flow);
    return page;
}

QWidget *CommunicationWindow::createUdpPage() {
    QWidget *page = new QWidget;
    QFormLayout *form = new QFormLayout(page);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    m_udpBind = field("udpBind", "0.0.0.0");
    m_udpLocalPort = field("udpLocalPort", "9000", true);
    form->addRow(trText("本地绑定 IP"), m_udpBind);
    form->addRow(trText("本地端口"), m_udpLocalPort);
    return page;
}

QWidget *CommunicationWindow::createTcpPage() {
    QWidget *page = new QWidget;
    QFormLayout *form = new QFormLayout(page);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    m_tcpRole = choices("tcpRole", {trText("客户端"), trText("服务端 · 单客户端")});
    m_tcpHost = field("tcpHost", "127.0.0.1");
    m_tcpHost->setToolTip(trText("客户端：远端 IP / 主机名；服务端：本地绑定 IP"));
    m_tcpPort = field("tcpPort", "9001", true);
    form->addRow(trText("TCP 模式"), m_tcpRole);
    form->addRow(trText("主机 / 绑定 IP"), m_tcpHost);
    form->addRow(trText("远端 / 监听端口"), m_tcpPort);
    return page;
}

void CommunicationWindow::createUi() {
    setWindowTitle(trText("FOSD · 三合一通信助手 · QWidgets"));
    resize(1120, 800);
    setMinimumSize(920, 700);
    QWidget *root = new QWidget(this);
    setCentralWidget(root);
    QVBoxLayout *outer = new QVBoxLayout(root);
    outer->setContentsMargins(24, 24, 24, 24);
    outer->setSpacing(16);
    QLabel *title = new QLabel(trText("通信助手"));
    title->setObjectName("title");
    outer->addWidget(title);
    outer->addWidget(new QLabel(trText("SerialPort / UDP / TCP · 原始数据收发 · QWidgets")));
    QHBoxLayout *body = new QHBoxLayout;
    body->setSpacing(20);
    outer->addLayout(body, 1);
    QGroupBox *connection = new QGroupBox(trText("连接配置"));
    connection->setFixedWidth(320);
    body->addWidget(connection);
    QVBoxLayout *left = new QVBoxLayout(connection);
    left->setSpacing(14);
    m_channel = choices(
        "channel", {trText("SerialPort · 串口"), trText("UDP · 数据报"), trText("TCP · 网络流")});
    left->addWidget(m_channel);
    m_settings = new QStackedWidget;
    m_settings->addWidget(createSerialPage());
    m_settings->addWidget(createUdpPage());
    m_settings->addWidget(createTcpPage());
    left->addWidget(m_settings);
    left->addStretch();
    m_status = new QLabel;
    m_status->setObjectName("connectionStatus");
    left->addWidget(m_status);
    m_connection = new QPushButton;
    m_connection->setObjectName("connectionButton");
    left->addWidget(m_connection);
    QLabel *hint =
        new QLabel(trText("切换类型保留其他通道连接。\n接收记录显示全部通道的原始字节。"));
    hint->setWordWrap(true);
    left->addWidget(hint);

    QVBoxLayout *right = new QVBoxLayout;
    body->addLayout(right, 1);
    QGroupBox *receive = new QGroupBox(trText("收发记录"));
    right->addWidget(receive, 1);
    QVBoxLayout *receiveLayout = new QVBoxLayout(receive);
    QHBoxLayout *toolbar = new QHBoxLayout;
    m_hexDisplay = new QCheckBox(trText("HEX 显示"));
    m_hexDisplay->setObjectName("hexDisplay");
    m_autoScroll = new QCheckBox(trText("滚动到底部"));
    m_autoScroll->setChecked(true);
    toolbar->addWidget(m_hexDisplay);
    toolbar->addWidget(m_autoScroll);
    toolbar->addStretch();
    QPushButton *clear = new QPushButton(trText("清空记录"));
    clear->setObjectName("clearLog");
    toolbar->addWidget(clear);
    receiveLayout->addLayout(toolbar);
    m_log = new QPlainTextEdit;
    m_log->setObjectName("activityLog");
    m_log->setReadOnly(true);
    m_log->setFont(QFont("Consolas", 10));
    receiveLayout->addWidget(m_log, 1);
    QHBoxLayout *counts = new QHBoxLayout;
    m_counters = new QLabel;
    counts->addWidget(m_counters);
    counts->addStretch();
    QPushButton *reset = new QPushButton(trText("计数清零"));
    reset->setObjectName("resetCounters");
    counts->addWidget(reset);
    receiveLayout->addLayout(counts);

    QGroupBox *sendBox = new QGroupBox(trText("发送到当前通道"));
    right->addWidget(sendBox);
    QVBoxLayout *sendLayout = new QVBoxLayout(sendBox);
    m_udpDestination = new QWidget;
    QHBoxLayout *destination = new QHBoxLayout(m_udpDestination);
    destination->setContentsMargins(0, 0, 0, 0);
    destination->addWidget(new QLabel(trText("目标")));
    m_udpPeer = field("udpPeer", "127.0.0.1");
    m_udpPeerPort = field("udpPeerPort", "9001", true);
    m_udpPeerPort->setMaximumWidth(100);
    destination->addWidget(m_udpPeer, 1);
    destination->addWidget(m_udpPeerPort);
    sendLayout->addWidget(m_udpDestination);
    m_sendText = new QPlainTextEdit;
    m_sendText->setObjectName("sendText");
    m_sendText->setPlaceholderText(trText("文本按 UTF-8 发送；HEX 示例：00 A5 5A FF"));
    m_sendText->setMinimumHeight(100);
    m_sendText->setMaximumHeight(170);
    sendLayout->addWidget(m_sendText);
    QHBoxLayout *sendOptions = new QHBoxLayout;
    m_hexSend = new QCheckBox(trText("HEX 发送"));
    m_hexSend->setObjectName("hexSend");
    m_crlf = new QCheckBox(trText("追加 CRLF"));
    m_crlf->setObjectName("appendCrLf");
    sendOptions->addWidget(m_hexSend);
    sendOptions->addWidget(m_crlf);
    sendOptions->addStretch();
    m_send = new QPushButton(trText("发送"));
    m_send->setObjectName("sendButton");
    sendOptions->addWidget(m_send);
    sendLayout->addLayout(sendOptions);
    m_error = new QLabel;
    m_error->setObjectName("errorMessage");
    m_error->setWordWrap(true);
    outer->addWidget(m_error);
    setStyleSheet(trText(
        "QMainWindow { background: #f3f5f8; }"
        "QWidget { font-family: 'Microsoft YaHei'; font-size: 13px; color: #334155; }"
        "QLabel#title { font-size: 25px; font-weight: bold; color: #172033; }"
        "QGroupBox { background: white; border: 1px solid #e2e8f0; border-radius: 8px; margin-top: "
        "14px; padding: 14px 10px 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 4px; }"
        "QLineEdit, QComboBox, QPlainTextEdit { background: #f8fafc; border: 1px solid #e2e8f0; "
        "border-radius: 4px; padding: 6px; }"
        "QPushButton { background: #e2e8f0; border: none; border-radius: 5px; padding: 8px 12px; }"
        "QPushButton:hover { background: #cbd5e1; }"
        "QPushButton#connectionButton, QPushButton#sendButton { background: #2563eb; color: white; "
        "}"
        "QPushButton:disabled, QPushButton#sendButton:disabled { background: #e2e8f0; color: "
        "#94a3b8; }"
        "QLabel#errorMessage { color: #dc2626; }"));
    connect(clear, &QPushButton::clicked, &m_presenter, &CommunicationPresenter::clearLog);
    connect(reset, &QPushButton::clicked, &m_presenter, &CommunicationPresenter::resetCounters);
}

void CommunicationWindow::bindPresenter() {
    connect(m_channel, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &CommunicationWindow::selectChannel);
    connect(m_connection, &QPushButton::clicked, this, &CommunicationWindow::toggleConnection);
    connect(m_send, &QPushButton::clicked, this, &CommunicationWindow::send);
    connect(m_hexDisplay, &QCheckBox::toggled, &m_presenter,
            &CommunicationPresenter::setHexDisplay);
    connect(&m_presenter, &CommunicationPresenter::selectionChanged, this,
            &CommunicationWindow::renderSelection);
    connect(&m_presenter, &CommunicationPresenter::connectionChanged, this,
            &CommunicationWindow::renderConnection);
    connect(&m_presenter, &CommunicationPresenter::serialPortsChanged, this,
            &CommunicationWindow::renderPorts);
    connect(&m_presenter, &CommunicationPresenter::activityLogChanged, this,
            &CommunicationWindow::renderLog);
    connect(&m_presenter, &CommunicationPresenter::countersChanged, this,
            &CommunicationWindow::renderCounters);
    connect(&m_presenter, &CommunicationPresenter::lastErrorChanged, this,
            &CommunicationWindow::renderError);
    connect(&m_presenter, &CommunicationPresenter::hexDisplayChanged, this,
            &CommunicationWindow::renderHexDisplay);
}

void CommunicationWindow::selectChannel(int index) {
    m_presenter.setSelectedChannel(index);
}

void CommunicationWindow::renderSelection() {
    const QSignalBlocker blocker(m_channel);
    m_channel->setCurrentIndex(m_presenter.selectedChannel());
    m_settings->setCurrentIndex(m_presenter.selectedChannel());
    m_udpDestination->setVisible(m_presenter.selectedChannel() == 1);
    renderConnection();
}

void CommunicationWindow::renderConnection() {
    m_settings->currentWidget()->setEnabled(!m_presenter.isOpen());
    m_status->setText(m_presenter.statusText());
    m_connection->setText(m_presenter.isOpen() ? trText("关闭当前通道") : trText("打开当前通道"));
    m_send->setEnabled(m_presenter.canSend());
}

void CommunicationWindow::renderPorts() {
    const QString current = m_serialPort->currentText();
    const QSignalBlocker blocker(m_serialPort);
    m_serialPort->clear();
    m_serialPort->addItems(m_presenter.serialPorts());
    if (!current.isEmpty())
        m_serialPort->setEditText(current);
}

void CommunicationWindow::renderLog() {
    QScrollBar *scroll = m_log->verticalScrollBar();
    const int position = scroll->value();
    m_log->setPlainText(m_presenter.activityLog());
    if (m_autoScroll->isChecked())
        scroll->setValue(scroll->maximum());
    else
        scroll->setValue(position);
}

void CommunicationWindow::renderCounters() {
    m_counters->setText(trText("接收 %1 B · 发送请求 %2 B")
                            .arg(m_presenter.receivedBytes())
                            .arg(m_presenter.submittedBytes()));
}

void CommunicationWindow::renderError() {
    m_error->setStyleSheet(m_presenter.lastError().isEmpty() ? QStringLiteral("color: #64748b;")
                                                             : QStringLiteral("color: #dc2626;"));
    m_error->setText(m_presenter.lastError().isEmpty()
                         ? trText("文本使用 UTF-8；TCP / 串口直接显示读取字节，不做协议拆包。")
                         : m_presenter.lastError());
}

void CommunicationWindow::renderHexDisplay() {
    const QSignalBlocker blocker(m_hexDisplay);
    m_hexDisplay->setChecked(m_presenter.hexDisplay());
}

void CommunicationWindow::toggleConnection() {
    if (m_presenter.isOpen()) {
        m_presenter.closeConnection();
        return;
    }
    QVariantMap settings;
    settings["portName"] = m_serialPort->currentText();
    settings["baudRate"] = m_baud->currentText();
    settings["dataBits"] = m_dataBits->currentText().toInt();
    const int parity[] = {0, 2, 3};
    settings["parity"] = parity[m_parity->currentIndex()];
    settings["stopBits"] = m_stopBits->currentText().toInt();
    settings["flowControl"] = m_flow->currentIndex();
    settings["bindAddress"] = m_udpBind->text();
    settings["localPort"] = m_udpLocalPort->text();
    settings["tcpRole"] = m_tcpRole->currentIndex();
    settings["host"] = m_tcpHost->text();
    settings["port"] = m_tcpPort->text();
    m_presenter.openConnection(settings);
}

void CommunicationWindow::send() {
    m_presenter.sendData(m_sendText->toPlainText(), m_hexSend->isChecked(), m_crlf->isChecked(),
                         m_udpPeer->text(), m_udpPeerPort->text());
}
} // namespace assistant
} // namespace fosd
