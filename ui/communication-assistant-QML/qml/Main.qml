import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: window
    objectName: "assistantWindow"
    visible: true
    width: 1120
    height: 800
    minimumWidth: 900
    minimumHeight: 700
    title: "FOSD · 三合一通信助手 · QML"
    color: "#f3f5f8"
    Material.theme: Material.Light
    Material.accent: "#2563eb"
    font.family: "Microsoft YaHei"
    font.pixelSize: 14

    function openSelectedChannel() {
        presenter.openConnection({
            portName: serialPort.editText,
            baudRate: baudRate.editText,
            dataBits: Number(dataBits.currentText),
            parity: [0, 2, 3][parity.currentIndex],
            stopBits: Number(stopBits.currentText),
            flowControl: flowControl.currentIndex,
            bindAddress: udpBind.text,
            localPort: udpLocalPort.text,
            tcpRole: tcpRole.currentIndex,
            host: tcpHost.text,
            port: tcpPort.text
        })
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16
        RowLayout {
            Layout.fillWidth: true
            ColumnLayout {
                spacing: 4
                Label { text: "通信助手"; font.pixelSize: 25; font.bold: true; color: "#172033" }
                Label { text: "SerialPort / UDP / TCP  ·  原始数据收发"; color: "#64748b" }
            }
            Item { Layout.fillWidth: true }
            Label { text: "学习与测试工具"; color: "#64748b" }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20
            GroupBox {
                id: connectionBox
                title: "连接配置"
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                background: Rectangle {
                    y: connectionBox.topPadding - 8
                    width: parent.width
                    height: parent.height - y
                    radius: 8
                    color: "white"
                    border.color: "#e2e8f0"
                }
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10
                    ComboBox {
                        id: channelChoice
                        Layout.fillWidth: true
                        model: ["SerialPort · 串口", "UDP · 数据报", "TCP · 网络流"]
                        currentIndex: presenter.selectedChannel
                        onActivated: presenter.selectedChannel = currentIndex
                    }
                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        contentWidth: availableWidth
                        ColumnLayout {
                            width: parent.width
                            enabled: !presenter.isOpen
                            ColumnLayout {
                                objectName: "serialSettings"
                                visible: presenter.selectedChannel === 0
                                Layout.fillWidth: true
                                Label { text: "串口" }
                                RowLayout {
                                    Layout.fillWidth: true
                                    ComboBox {
                                        id: serialPort
                                        Layout.fillWidth: true
                                        editable: true
                                        model: presenter.serialPorts
                                    }
                                    Button { text: "刷新"; onClicked: presenter.refreshSerialPorts() }
                                }
                                Label { text: "波特率" }
                                ComboBox {
                                    id: baudRate
                                    Layout.fillWidth: true
                                    editable: true
                                    model: ["9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"]
                                    currentIndex: 4
                                }
                                Label { text: "数据位 / 校验 / 停止位" }
                                RowLayout {
                                    Layout.fillWidth: true
                                    ComboBox { id: dataBits; Layout.fillWidth: true; model: ["8", "7", "6", "5"] }
                                    ComboBox { id: parity; Layout.fillWidth: true; model: ["无", "偶", "奇"] }
                                    ComboBox { id: stopBits; Layout.fillWidth: true; model: ["1", "2"] }
                                }
                                Label { text: "流控" }
                                ComboBox {
                                    id: flowControl
                                    Layout.fillWidth: true
                                    model: ["无流控", "硬件 RTS/CTS", "软件 XON/XOFF"]
                                }
                            }
                            ColumnLayout {
                                objectName: "udpSettings"
                                visible: presenter.selectedChannel === 1
                                Layout.fillWidth: true
                                Label { text: "本地绑定 IP" }
                                TextField { id: udpBind; objectName: "udpBind"; Layout.fillWidth: true; text: "0.0.0.0"; selectByMouse: true }
                                Label { text: "本地端口" }
                                TextField {
                                    id: udpLocalPort
                                    objectName: "udpLocalPort"
                                    Layout.fillWidth: true
                                    text: "9000"
                                    selectByMouse: true
                                    validator: IntValidator { bottom: 1; top: 65535 }
                                }
                            }
                            ColumnLayout {
                                objectName: "tcpSettings"
                                visible: presenter.selectedChannel === 2
                                Layout.fillWidth: true
                                Label { text: "TCP 模式" }
                                ComboBox {
                                    id: tcpRole
                                    Layout.fillWidth: true
                                    model: ["客户端", "服务端 · 单客户端"]
                                }
                                Label { text: tcpRole.currentIndex === 0 ? "远端 IP / 主机名" : "本地绑定 IP" }
                                TextField { id: tcpHost; Layout.fillWidth: true; text: "127.0.0.1"; selectByMouse: true }
                                Label { text: tcpRole.currentIndex === 0 ? "远端端口" : "监听端口" }
                                TextField {
                                    id: tcpPort
                                    Layout.fillWidth: true
                                    text: "9001"
                                    selectByMouse: true
                                    validator: IntValidator { bottom: 1; top: 65535 }
                                }
                            }
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        text: presenter.statusText
                        color: presenter.canSend ? "#16803c" : "#64748b"
                        wrapMode: Text.Wrap
                    }
                    Button {
                        id: connectionButton
                        objectName: "connectionButton"
                        Layout.fillWidth: true
                        text: presenter.isOpen ? "关闭当前通道" : "打开当前通道"
                        highlighted: !presenter.isOpen
                        background: Rectangle {
                            radius: 6
                            color: presenter.isOpen ? "#e2e8f0" : (connectionButton.down ? "#1d4ed8" : "#2563eb")
                        }
                        onClicked: {
                            if (presenter.isOpen) presenter.closeConnection()
                            else window.openSelectedChannel()
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        text: "切换类型保留其他通道连接。\n接收记录显示全部通道的原始字节。"
                        color: "#64748b"
                        wrapMode: Text.Wrap
                        font.pixelSize: 12
                    }
                }
            }
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 12
                GroupBox {
                    id: receiveBox
                    title: "收发记录"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    background: Rectangle {
                        y: receiveBox.topPadding - 8
                        width: parent.width
                        height: parent.height - y
                        radius: 8
                        color: "white"
                        border.color: "#e2e8f0"
                    }
                    ColumnLayout {
                        anchors.fill: parent
                        RowLayout {
                            Layout.fillWidth: true
                            CheckBox {
                                text: "HEX 显示"
                                checked: presenter.hexDisplay
                                onToggled: presenter.hexDisplay = checked
                            }
                            CheckBox { id: autoScroll; text: "滚动到底部"; checked: true }
                            Item { Layout.fillWidth: true }
                            Button { text: "清空记录"; onClicked: presenter.clearLog() }
                        }
                        ScrollView {
                            id: logScroll
                            contentWidth: availableWidth
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            TextArea {
                                id: receiveArea
                                width: logScroll.availableWidth
                                height: Math.max(logScroll.availableHeight, implicitHeight)
                                text: presenter.activityLog
                                readOnly: true
                                selectByMouse: true
                                wrapMode: TextEdit.WrapAnywhere
                                font.family: "Consolas"
                                font.pixelSize: 13
                                color: "#334155"
                                background: Rectangle { radius: 4; color: "#f8fafc" }
                                onTextChanged: {
                                    if (autoScroll.checked) cursorPosition = length
                                }
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            Label {
                                text: "接收 " + presenter.receivedBytes + " B   ·   发送请求 " + presenter.submittedBytes + " B"
                                color: "#64748b"
                                font.pixelSize: 12
                            }
                            Item { Layout.fillWidth: true }
                            Button { text: "计数清零"; onClicked: presenter.resetCounters() }
                        }
                    }
                }
                GroupBox {
                    id: sendBox
                    title: "发送到当前通道"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 235
                    background: Rectangle {
                        y: sendBox.topPadding - 8
                        width: parent.width
                        height: parent.height - y
                        radius: 8
                        color: "white"
                        border.color: "#e2e8f0"
                    }
                    ColumnLayout {
                        anchors.fill: parent
                        RowLayout {
                            visible: presenter.selectedChannel === 1
                            Layout.fillWidth: true
                            Label { text: "目标" }
                            TextField { id: udpPeer; Layout.fillWidth: true; text: "127.0.0.1"; selectByMouse: true }
                            TextField {
                                id: udpPeerPort
                                objectName: "udpPeerPort"
                                Layout.preferredWidth: 100
                                text: "9001"
                                selectByMouse: true
                                validator: IntValidator { bottom: 1; top: 65535 }
                            }
                        }
                        ScrollView {
                            id: sendScroll
                            contentWidth: availableWidth
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            TextArea {
                                id: sendArea
                                objectName: "sendText"
                                width: sendScroll.availableWidth
                                height: Math.max(sendScroll.availableHeight, implicitHeight)
                                placeholderText: hexSend.checked ? "例如：00 A5 5A FF（两位一字节）" : "输入文本，以 UTF-8 发送"
                                selectByMouse: true
                                wrapMode: TextEdit.WrapAnywhere
                                background: Rectangle { radius: 4; color: "#f8fafc" }
                            }
                        }
                        RowLayout {
                            Layout.fillWidth: true
                            CheckBox { id: hexSend; text: "HEX 发送" }
                            CheckBox { id: crlf; text: "追加 CRLF" }
                            Item { Layout.fillWidth: true }
                            Button {
                                id: sendButton
                                objectName: "sendButton"
                                text: "发送"
                                highlighted: true
                                enabled: presenter.canSend
                                background: Rectangle {
                                    radius: 6
                                    color: !sendButton.enabled ? "#e2e8f0" : (sendButton.down ? "#1d4ed8" : "#2563eb")
                                }
                                onClicked: presenter.sendData(sendArea.text, hexSend.checked, crlf.checked,
                                                              udpPeer.text, udpPeerPort.text)
                            }
                        }
                    }
                }
            }
        }
        Label {
            Layout.fillWidth: true
            text: presenter.lastError.length > 0 ? presenter.lastError : "文本模式使用 UTF-8；TCP / 串口每次读取的字节直接显示，不做协议拆包。"
            color: presenter.lastError.length > 0 ? "#dc2626" : "#64748b"
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 12
        }
    }
}
