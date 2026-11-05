#include "ClassicWorkbench.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QAction>
#include <QStatusBar>
#include <QVTKOpenGLNativeWidget.h>
#include <QSurfaceFormat>
#include <QtTest>
#include "backend/WorkbenchRuntime.h"
#include <QFile>
#include <QTemporaryDir>
#include <QUdpSocket>
#include <QPushButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTextDocument>
#include <QSignalSpy>
#include <QtEndian>
#include <cstring>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
class TestWindow : public ClassicWorkbench {
public:
 QString selectedFile;
 QString chooseFile(bool,const QString &,const QString &) override { return selectedFile; }
};
static QByteArray jcs(float delta) {
 QByteArray bytes(24,'\0'); bytes.replace(0,4,QByteArray::fromHex("fafbfafb")); bytes[4]=6; bytes[19]=6; bytes[20]=12;
 for(int i=0;i<12;++i) { float value=1550.0f+i*.5f+delta; quint32 bits; std::memcpy(&bits,&value,4); char b[4]; qToLittleEndian(bits,reinterpret_cast<uchar *>(b)); bytes.append(b,4); }
 return bytes;
}
class WorkbenchMainTests:public QObject {
    Q_OBJECT

private slots:
 void serialQuickCommandsAndCrlf() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir;
  qputenv("FOSD_CONFIG_FILE", dir.filePath("commands.ini").toUtf8());
  TestWindow w;
  QTest::qWait(100); // Apply initial configuration before editing commands.
  auto *panel = w.panelWidget(PanelId::Connection);
  auto &transport = w.runtime().protocol().communication();
  // Observe the exact bytes submitted to the transport without physical serial I/O.
  disconnect(&transport, &communication::ConnectControl::requestSend, nullptr, nullptr);
  QSignalSpy sent(&transport, &communication::ConnectControl::requestSend);
  auto *first = panel->findChild<QLineEdit *>("edit_SerialCommand0");
  QVERIFY(first); QCOMPARE(first->text(), QString("udp"));
  panel->findChild<QPushButton *>("pbtn_SerialCommand0")->click();
  QTest::qWait(100); QCOMPARE(sent.count(), 0); // Closed serial must not send.
  emit transport.stateChanged(communication::Channel::Serial, communication::ConnectionState::Connected);
  for (int i = 0; i < 10; ++i) {
   auto *edit = panel->findChild<QLineEdit *>(QString("edit_SerialCommand%1").arg(i));
   auto *button = panel->findChild<QPushButton *>(QString("pbtn_SerialCommand%1").arg(i));
   QVERIFY(edit && button);
   const QString command = QString::fromUtf8("指令%1").arg(i);
   edit->setText(command);
   button->click();
   QTRY_COMPARE(sent.count(), i + 1);
   QCOMPARE(qvariant_cast<communication::Channel>(sent.last()[0]), communication::Channel::Serial);
   QCOMPARE(sent.last()[1].toByteArray(), command.toUtf8() + "\r\n");
  }
  // Editing alone saves a slot, including an intentionally blank value.
  first->setText(QString::fromUtf8("编辑后保存"));
  QVERIFY(QMetaObject::invokeMethod(first, "editingFinished"));
  auto *blank = panel->findChild<QLineEdit *>("edit_SerialCommand4");
  blank->clear(); panel->findChild<QPushButton *>("pbtn_SerialCommand4")->click();
  QTest::qWait(100); QCOMPARE(sent.count(), 10);
  auto *manual = panel->findChild<QPlainTextEdit *>("ptedit_SerialSend");
  for (const auto &command : QStringList{"reset", "reset\n", "reset\r\n", "reset\n\n"}) {
   const int count = sent.count();
   manual->setPlainText(command);
   panel->findChild<QPushButton *>("pbtn_SerialSend")->click();
   QTRY_COMPARE(sent.count(), count + 1);
   QCOMPARE(sent.last()[1].toByteArray(), QByteArray("reset\r\n"));
  }
  {
   TestWindow restored;
   auto *connection = restored.panelWidget(PanelId::Connection);
   QTRY_COMPARE(connection->findChild<QLineEdit *>("edit_SerialCommand0")->text(), QString::fromUtf8("编辑后保存"));
   QCOMPARE(connection->findChild<QLineEdit *>("edit_SerialCommand4")->text(), QString());
   QCOMPARE(connection->findChild<QLineEdit *>("edit_SerialCommand9")->text(), QString::fromUtf8("指令9"));
  }
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void receiveDisplayModes() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir;
  qputenv("FOSD_CONFIG_FILE", dir.filePath("display.ini").toUtf8());
  TestWindow w;
  auto *panel = w.panelWidget(PanelId::Connection);
  auto *serialMode = panel->findChild<QComboBox *>("cbox_SerialDisplayMode");
  auto *udpMode = panel->findChild<QComboBox *>("cbox_UdpDisplayMode");
  auto *serialText = panel->findChild<QPlainTextEdit *>("ptedit_SerialRec");
  auto *udpText = panel->findChild<QPlainTextEdit *>("ptedit_UdpRec");
  QVERIFY(serialMode && udpMode && serialText && udpText);
  QCOMPARE(serialMode->currentIndex(), 1);
  QCOMPARE(udpMode->currentIndex(), 0);
  QVERIFY(serialText->isReadOnly()); QVERIFY(udpText->isReadOnly());
  auto &transport = w.runtime().protocol().communication();
  const QByteArray bytes = QString::fromUtf8("中文 AB\r\n").toUtf8();
  // Serial reads may split a UTF-8 character across callbacks.
  emit transport.rawDataReceived(communication::Channel::Serial, bytes.left(1), {});
  emit transport.rawDataReceived(communication::Channel::Serial, bytes.mid(1), {});
  emit transport.rawDataReceived(communication::Channel::Udp, bytes, {});
  communication::DecoderError invalid;
  invalid.channel = communication::Channel::Udp;
  invalid.message = "not a protocol frame";
  emit transport.decoderErrorOccurred(invalid);
  const QString hex = QString::fromLatin1(bytes.toHex(' ').toUpper());
  // QPlainTextEdit normalizes CRLF to LF.
  const QString plain = QString::fromUtf8(bytes).replace("\r\n", "\n");
  QTRY_COMPARE(serialText->toPlainText(), plain);
  QTRY_VERIFY(udpText->toPlainText().contains(hex));
  QTest::keyClick(serialMode, Qt::Key_Home);
  QTRY_COMPARE(serialText->toPlainText(), hex);
  QTest::keyClick(udpMode, Qt::Key_End);
  QTRY_VERIFY(udpText->toPlainText().contains(QString::fromUtf8("预览UTF-8：中文 AB\\r\\n")));
  const QString udpLog = udpText->toPlainText();
  const QByteArray binary = QByteArray::fromHex("00ff");
  emit transport.rawDataReceived(communication::Channel::Serial, binary, {});
  const auto binaryHex = QString::fromLatin1((bytes + binary).toHex(' ').toUpper());
  QTRY_COMPARE(serialText->toPlainText(), binaryHex);
  QTest::keyClick(serialMode, Qt::Key_End);
  QTest::qWait(150);
  QTest::keyClick(serialMode, Qt::Key_Home);
  QTRY_COMPARE(serialText->toPlainText(), binaryHex);
  // Each channel keeps its own mode and receive history.
  QCOMPARE(udpText->toPlainText(), udpLog);
  panel->findChild<QPushButton *>("pbtn_SerialClear")->click();
  QTRY_VERIFY(serialText->toPlainText().isEmpty());
  QTest::keyClick(serialMode, Qt::Key_End);
  QTest::qWait(150);
  QVERIFY(serialText->toPlainText().isEmpty());
  QCOMPARE(udpText->toPlainText(), udpLog);
  emit transport.rawDataReceived(communication::Channel::Serial, QByteArray(70000, 'A'), {});
  QTRY_COMPARE(serialText->toPlainText(), QString(65536, 'A'));
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void udpPacketSummaryAndCleanup() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir;
  qputenv("FOSD_CONFIG_FILE", dir.filePath("udp-log.ini").toUtf8());
  TestWindow w;
  auto *panel = w.panelWidget(PanelId::Connection);
  auto *log = panel->findChild<QPlainTextEdit *>("ptedit_UdpRec");
  auto *serial = panel->findChild<QPlainTextEdit *>("ptedit_SerialRec");
  QVERIFY(log && serial);
  QCOMPARE(log->maximumBlockCount(), 200);
  QVERIFY(!log->isUndoRedoEnabled());
  QUdpSocket reserve;
  QVERIFY(reserve.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
  const quint16 port = reserve.localPort(); reserve.close();
  auto &runtime = w.runtime();
  auto &transport = runtime.protocol().communication();
  QSignalSpy state(&transport, &communication::ConnectControl::stateChanged);
  communication::UdpConfig config;
  config.bindAddress = QHostAddress::LocalHost; config.localPort = port;
  transport.openUdp(config);
  QTRY_VERIFY(!state.isEmpty());
  QUdpSocket sender;
  const QByteArray packet = QByteArray::fromHex("5aa5010102007856341201020000c03f10ff000010c0");
  QCOMPARE(sender.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
  QTRY_VERIFY(log->toPlainText().contains("mp.wavelength"));
  auto contents = log->toPlainText();
  // Expected UTF-8 bytes are independent of MSVC's source/literal conversion.
  QCOMPARE(contents.section(" | ", 2, 2), QString::fromUtf8("\xE7\xB4\xAF\xE8\xAE\xA1=1"));
  QVERIFY(contents.contains("127.0.0.1:"));
  QVERIFY(contents.contains(QString::fromUtf8("序号=305419896")));
  QVERIFY(contents.contains(QString::fromUtf8("记录=2")));
  QVERIFY(contents.contains("CH1/2 1.5 nm"));
  QVERIFY(contents.contains("22 B"));
  QVERIFY(QRegularExpression("\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\]").match(contents).hasMatch());
  QVERIFY(!contents.contains("5A A5"));
  QVERIFY(serial->toPlainText().isEmpty());
  const auto saved = runtime.protocol().dataModel()->latest();
  QVERIFY(saved);
  // Invalid/large data gets a short preview, never the entire datagram.
  const QByteArray invalid(60000, 'Z');
  QCOMPARE(sender.writeDatagram(invalid, QHostAddress::LocalHost, port), qint64(invalid.size()));
  QTRY_VERIFY(log->toPlainText().contains(QString::fromUtf8("未识别/无效包")));
  QVERIFY(!log->toPlainText().contains(QString(65, 'Z')));
  QCOMPARE(runtime.protocol().dataModel()->latest(), saved);
  panel->findChild<QPushButton *>("pbtn_UdpClear")->click();
  QTRY_VERIFY(log->toPlainText().isEmpty());
  QTest::qWait(150); QVERIFY(log->toPlainText().isEmpty());
  // Many callbacks within one display interval produce only the latest summary.
  for (int i = 0; i < 1000; ++i)
   emit runtime.protocol().frameParsed(saved);
  QVERIFY(QMetaObject::invokeMethod(&runtime, "publishLatest"));
  QTRY_COMPARE(log->document()->blockCount(), 1);
  QTRY_VERIFY(log->toPlainText().contains("mp.wavelength"));
  // Exercise the document's actual limit after more than 200 display intervals.
  for (int i = 0; i < 220; ++i) {
   emit runtime.protocol().frameParsed(saved);
   QVERIFY(QMetaObject::invokeMethod(&runtime, "publishLatest"));
  }
  QTRY_COMPARE(log->document()->blockCount(), 200);
  QVERIFY(log->toPlainText().size() < 100000);
  emit transport.rawDataReceived(communication::Channel::Serial, "serial text", {});
  QTRY_COMPARE(serial->toPlainText(), QString("serial text"));
  auto *cleanup = runtime.findChild<QTimer *>("receiveLogCleanupTimer");
  QVERIFY(cleanup); QCOMPARE(cleanup->interval(), 60000);
  cleanup->start(30); // Verify the same production timer without a 60-second delay.
  QTRY_VERIFY(log->toPlainText().isEmpty() && serial->toPlainText().isEmpty());
  cleanup->stop();
  QCOMPARE(runtime.protocol().dataModel()->latest(), saved);
  // Logs resume after cleanup; the serial and parser state remain independent.
  QCOMPARE(sender.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
  QTRY_VERIFY(log->toPlainText().contains("mp.wavelength"));
  QVERIFY(serial->toPlainText().isEmpty());
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void acquisitionToDisplayAndFiles() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("配置.ini").toUtf8());
  TestWindow w; w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w)); QTest::qWait(100);
  auto *connection=w.panelView(PanelId::Connection); auto *cw=w.panelWidget(PanelId::Connection);
  QUdpSocket reserve; QVERIFY(reserve.bind(QHostAddress(QHostAddress::LocalHost),quint16(0))); quint16 port=reserve.localPort(); reserve.close();
  connection->applyFields({{"cbox_CommuteChoose",QVariantMap{{"index",0}}},{"cbox_LocakIP",QVariantMap{{"text","127.0.0.1"}}},{"edit_LocalPort",QString::number(port)},{"edit_TargetIP","127.0.0.1"},{"edit_TargetPort","9000"}});
  QSignalSpy frames(&w.runtime().protocol(),&protocol::ProtocolControl::frameParsed);
  QSignalSpy configured(&w.runtime().calculation(),&Calculation::CalculationControl::configurationAccepted);
  cw->findChild<QPushButton *>("pbtn_OpenNet")->click(); QTest::qWait(150);
  QUdpSocket sender;
  auto send=[&](float delta){ QCOMPARE(sender.writeDatagram(jcs(delta),QHostAddress::LocalHost,port),qint64(jcs(delta).size())); };
  send(0); QTRY_COMPARE(frames.count(),1);
  cw->findChild<QPushButton *>("pbtn_WaveLengthReset")->click(); QTest::qWait(50);
  for(int i=0;i<20;++i) {send(0); QTRY_COMPARE(frames.count(),i+2);}
  QTRY_COMPARE(configured.count(),1);
  const auto model=dir.filePath("中文模型.stl"); QFile file(model); QVERIFY(file.open(QIODevice::WriteOnly));
  file.write("solid t\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 10 0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid t\n"); file.close();
  w.selectedFile=model; QSignalSpy models(&w.runtime().calculation(),&Calculation::CalculationControl::modelAccepted);
  w.panelWidget(PanelId::ModelPanel)->findChild<QPushButton *>("pbtn_ModelLoad")->click(); QTRY_COMPARE(models.count(),1);
  w.selectPage(2); send(.02f);
  QTRY_VERIFY(w.runtime().calculation().dataModel()->latest());
  QCOMPARE(w.runtime().calculation().dataModel()->latest()->algorithmId,QString("HYY.IFE"));
  QTest::qWait(150); auto selected=w.runtime().visualization().selectPoint(0); QVERIFY(selected.valid);
  w.findChild<QAction *>("actionPause")->trigger(); QTest::qWait(50);
  auto frozen=w.runtime().visualization().selectedPoint().displayPositionMm;
  auto revision=w.runtime().calculation().dataModel()->latest()->revision;
  send(.04f); QTRY_VERIFY(w.runtime().calculation().dataModel()->latest()->revision>revision);
  QTest::qWait(100); QVERIFY(w.runtime().visualization().selectedPoint().displayPositionMm==frozen);
  w.selectedFile=dir.filePath("波长曲线.png"); cw->findChild<QPushButton *>("pbtn_SaveSpectrum")->click();
  QTRY_VERIFY(QFileInfo(w.selectedFile).size()>0); QVERIFY(!QImage(w.selectedFile).isNull());
  w.selectedFile=dir.filePath("波长.csv"); cw->findChild<QPushButton *>("pbtn_SaveStatic")->click(); QTRY_VERIFY(QFileInfo(w.selectedFile).size()>0);
  w.grab().save("migration-classic.png");
  w.selectPage(0); w.grab().save("migration-communication.png");
  cw->findChild<QPushButton *>("pbtn_OpenNet")->click();
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void classicComposition() {
 ClassicWorkbench w; w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w));
 QVERIFY(w.renderHost()->surface()->findChild<QVTKOpenGLNativeWidget *>());
 QVERIFY(!w.findChild<QAction *>("actionPlateValidation"));
 QVERIFY(!w.findChild<QAction *>("actionTrackingPoints"));
 for(auto *bar:w.findChildren<QStatusBar *>()) QVERIFY(!bar->isVisible());
 }};
int main(int argc,char **argv) { QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat()); QApplication a(argc,argv); WorkbenchMainTests t; return QTest::qExec(&t,argc,argv); }
#include "workbench_main_tests.moc"
