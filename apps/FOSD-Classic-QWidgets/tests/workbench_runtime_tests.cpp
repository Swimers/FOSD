#include "backend/WorkbenchRuntime.h"
#include "workbench_test_packets.h"
#include "config/ParameterStore.h"
#include <QTemporaryDir>
#include <QSignalSpy>
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtTest>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
using namespace fosd; using namespace fosd::ui::classic;
class WorkbenchRuntimeTests:public QObject {
    Q_OBJECT


private slots:
 void tcpClientAndServerJcs() {
  for(int role=0;role<2;++role) {
   QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("tcp.ini").toUtf8());
   application::WorkbenchRuntime r; QTcpServer peer; QTcpSocket client;
   QVERIFY(peer.listen(QHostAddress::LocalHost,0)); quint16 port=peer.serverPort();
   if(role==1) peer.close();
   UiRequest req; req.panel=PanelId::Connection; req.action="pbtn_OpenNet";
   req.fields={{"cbox_CommuteChoose",QVariantMap{{"index",1}}},{"cbox_TcpRole",QVariantMap{{"index",role}}},{"cbox_LocakIP",QVariantMap{{"text","127.0.0.1"}}},{"edit_LocalPort",QString::number(port)},{"edit_TargetIP","127.0.0.1"},{"edit_TargetPort",QString::number(port)}};
   UiResponse out; QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
   QTcpSocket *socket=nullptr;
   if(role==0) { QTRY_VERIFY(peer.hasPendingConnections()); socket=peer.nextPendingConnection(); }
   else { QTest::qWait(100); client.connectToHost(QHostAddress::LocalHost,port); QTRY_COMPARE(client.state(),QAbstractSocket::ConnectedState); socket=&client; }
   QSignalSpy frames(&r.protocol(),&protocol::ProtocolControl::frameParsed);
   QByteArray bytes(24,'\0'); bytes.replace(0,4,QByteArray::fromHex("fafbfafb")); bytes[19]=1; bytes[20]=1; bytes+=QByteArray::fromHex("00c0c144");
   socket->write(bytes.left(13)); socket->flush(); QTest::qWait(30); QCOMPARE(frames.count(),0);
   socket->write(bytes.mid(13)+bytes); socket->flush(); QTRY_COMPARE(frames.count(),2);
   auto frame=std::dynamic_pointer_cast<const core::WavelengthFrame>(r.protocol().dataModel()->latest()->data);
   QVERIFY(frame); QCOMPARE(frame->records[0].channel,quint8(16)); QCOMPARE(frame->records[0].wavelength,1550.f);
   r.protocol().communication().close(communication::Channel::Tcp);
   if(role==0) delete socket;
   qunsetenv("FOSD_CONFIG_FILE");
  }
 }
 void meanBaselineAndControl() {
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("参数.ini").toUtf8());
  application::WorkbenchRuntime r; QSignalSpy frames(&r.protocol(), &protocol::ProtocolControl::frameParsed);
  QSignalSpy configs(&r.calculation(), &Calculation::CalculationControl::configurationAccepted);
  auto feed=[&](int n) { r.protocol().parseFrame(communication::Channel::Udp,workbench_test::packet(n,n*.001)); };
  feed(1); QTRY_COMPARE(frames.count(),1);
  UiRequest req; req.panel=PanelId::Connection; req.action="pbtn_WaveLengthReset"; UiResponse out;
  QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
  for(int i=0;i<20;++i) {feed(i+2); QTRY_COMPARE(frames.count(),i+2);}
  QTRY_COMPARE(configs.count(),1);
  Calculation::CalculationConfiguration c; QString error;
  QVERIFY2(config::ParameterStore().read(&c,true,&error),qPrintable(error));
  QCOMPARE(c.strainCalibration.originalWavelength.size(),size_t(12));
  double mean=0; for(int i=2;i<=21;++i) mean+=double(float(1550.0+i*.001))/20;
  QVERIFY(qAbs(c.strainCalibration.originalWavelength[0]-mean)<1e-8);
  const auto before=c.strainCalibration.originalWavelength;
  req.action="pbtn_SelectCH"; req.fields={{"ckbox_CH16",true}};
  QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
  QVERIFY(config::ParameterStore().read(&c,true,&error)); QVERIFY(c.strainCalibration.originalWavelength==before);
  QSignalSpy results(&r.calculation(), &Calculation::CalculationControl::resultReady);
  core::ModelData geometry; geometry[0]={100,0,0}; r.calculation().setModel(geometry);
  feed(30); QTRY_VERIFY(results.count()>0);
  req.action="pbtn_CalculationStop"; QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
  QTest::qWait(50); int count=results.count(); feed(31); QTest::qWait(100); QCOMPARE(results.count(),count);
  req.action="pbtn_CalculationStart"; QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
  feed(32); QTRY_VERIFY(results.count()>count);
  req.action="pbtn_SaveStatic"; req.fields={{"filePath",dir.filePath("波长.csv")}};
  QVERIFY(r.handleRequest(req,&out)); QVERIFY(out.success);
  QTRY_VERIFY(QFileInfo(dir.filePath("波长.csv")).size()>0);
  qunsetenv("FOSD_CONFIG_FILE");
 }
};
QTEST_GUILESS_MAIN(WorkbenchRuntimeTests)
#include "workbench_runtime_tests.moc"
