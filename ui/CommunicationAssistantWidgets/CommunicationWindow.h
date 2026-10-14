#pragma once

#include <QMainWindow>

class QComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QPlainTextEdit;
class QLabel;
class QStackedWidget;
class QWidget;

namespace fosd {
namespace assistant {
class CommunicationPresenter;

// Passive View: borrows the Presenter, which must outlive this window.
class CommunicationWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit CommunicationWindow(CommunicationPresenter &presenter, QWidget *parent = nullptr);

private slots:
    void selectChannel(int index);
    void renderSelection();
    void renderConnection();
    void renderPorts();
    void renderLog();
    void renderCounters();
    void renderError();
    void renderHexDisplay();
    void toggleConnection();
    void send();

private:
    QWidget *createSerialPage();
    QWidget *createUdpPage();
    QWidget *createTcpPage();
    void createUi();
    void bindPresenter();
    CommunicationPresenter &m_presenter;
    QComboBox *m_channel = nullptr;
    QStackedWidget *m_settings = nullptr;
    QComboBox *m_serialPort = nullptr;
    QComboBox *m_baud = nullptr;
    QComboBox *m_dataBits = nullptr;
    QComboBox *m_parity = nullptr;
    QComboBox *m_stopBits = nullptr;
    QComboBox *m_flow = nullptr;
    QLineEdit *m_udpBind = nullptr;
    QLineEdit *m_udpLocalPort = nullptr;
    QLineEdit *m_udpPeer = nullptr;
    QLineEdit *m_udpPeerPort = nullptr;
    QWidget *m_udpDestination = nullptr;
    QComboBox *m_tcpRole = nullptr;
    QLineEdit *m_tcpHost = nullptr;
    QLineEdit *m_tcpPort = nullptr;
    QLabel *m_status = nullptr;
    QLabel *m_error = nullptr;
    QLabel *m_counters = nullptr;
    QPushButton *m_connection = nullptr;
    QPushButton *m_send = nullptr;
    QPlainTextEdit *m_log = nullptr;
    QPlainTextEdit *m_sendText = nullptr;
    QCheckBox *m_hexDisplay = nullptr;
    QCheckBox *m_autoScroll = nullptr;
    QCheckBox *m_hexSend = nullptr;
    QCheckBox *m_crlf = nullptr;
};
} // namespace assistant
} // namespace fosd
