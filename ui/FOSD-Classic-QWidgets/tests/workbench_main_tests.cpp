#include "FOSD-Classic-QWidgets/application/WorkbenchPresenter.h"
#include "FOSD-Classic-QWidgets/application/ClassicWorkbench.h"
#include "FOSD-Classic-QWidgets/features/visualization/presenters/VisualizationPresenter.h"
#include "FOSD-Classic-QWidgets/features/visualization/presenters/ModelPanelPresenter.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QAction>
#include <QStatusBar>
#include <QVTKOpenGLNativeWidget.h>
#include <QSurfaceFormat>
#include <QtTest>
#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include <QFile>
#include <QImage>
#include <QTemporaryDir>
#include <QUdpSocket>
#include <QPushButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTextDocument>
#include <QTabWidget>
#include <QCheckBox>
#include <QLabel>
#include <QWheelEvent>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextBlock>
#include <QColorDialog>
#include <QStandardItemModel>
#include "FOSD-Classic-QWidgets/controls/qcustomplot/qcustomplot.h"
#include <QSignalSpy>
#include <QLayout>
#include <QScreen>
#include <QWindow>
#include <QDockWidget>
#include <QtEndian>
#include <cstring>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
class TestWindow : public ClassicWorkbench {
public:
 QString selectedFile;
 fosd::ui::classic::WorkbenchPresenter &presenter(){return *findChild<fosd::ui::classic::WorkbenchPresenter *>();}
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
 void startupAspectAndVtkSpace() {
  using namespace fosd::ui::classic;
  QTemporaryDir configDir; QVERIFY(configDir.isValid());
  struct RestoreConfig {
   QByteArray old;
   ~RestoreConfig() { if(old.isNull()) qunsetenv("FOSD_CONFIG_FILE"); else qputenv("FOSD_CONFIG_FILE",old); }
  } restore{qgetenv("FOSD_CONFIG_FILE")};
  qputenv("FOSD_CONFIG_FILE",configDir.filePath("startup.ini").toUtf8());
  TestWindow w; w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w));
  const auto available=w.windowHandle()->screen()->availableGeometry();
  const auto initial=w.size();
  const auto minimum=w.minimumSize().expandedTo(w.minimumSizeHint());
  const int units=qMax((minimum.width()+15)/16,(minimum.height()+8)/9);
  if(units<=qMin(available.width()/16,available.height()/9)) {
   QCOMPARE(9*w.width(),16*w.height());
   QVERIFY(available.contains(w.geometry()));
  }
  w.selectPage(1); QCoreApplication::processEvents();
  auto *page=qobject_cast<QMainWindow *>(w.panelWidget(PanelId::Visualization));
  auto *model=page->findChild<QDockWidget *>("fosdDock0");
  QCOMPARE(page->tabifiedDockWidgets(model).size(),2);
  for(const auto panel:{PanelId::ModelPanel,PanelId::ScenePanel,PanelId::InfoPanel}) {
   w.showPanel(panel); QCoreApplication::processEvents();
   auto *dock=qobject_cast<QDockWidget *>(w.panelWidget(panel)->parentWidget());
   QVERIFY(dock); QCOMPARE(page->dockWidgetArea(dock),Qt::LeftDockWidgetArea);
   QVERIFY(!dock->visibleRegion().isEmpty());
  }
  w.showPanel(PanelId::ModelPanel); QCoreApplication::processEvents();
  QVERIFY(w.renderHost()->width()>page->width()*0.60);
  auto *vtk=w.renderHost()->surface()->findChild<QVTKOpenGLNativeWidget *>();
  QVERIFY(vtk); QTRY_COMPARE(vtk->size(),w.renderHost()->size());
  qInfo()<<"Startup window"<<initial<<"available screen"<<available.size()<<"VTK"<<vtk->size();
  w.grab().save("startup-vtk-layout.png");
  w.resize(initial+QSize(32,0)); QCOMPARE(w.size(),initial+QSize(32,0));
  w.resize(initial); w.hide(); w.show(); QCoreApplication::processEvents();
  QCOMPARE(w.size(),initial);
 }
#ifdef Q_OS_WIN
 void interactiveResizeCoalescesRendering() {
  using namespace fosd::ui::classic;
  WorkbenchView w; w.resize(1500,1000); w.show();
  QVERIFY(QTest::qWaitForWindowExposed(&w));
  auto *connection=w.panelWidget(PanelId::Connection);
  connection->findChild<QTabWidget *>("tabWidget")->setCurrentIndex(0);
  auto *plot=connection->findChild<QCustomPlot *>("cplot_Spectrum");
  QVector<double> x(20000),y(20000);
  for(int i=0;i<x.size();++i) { x[i]=i*.001; y[i]=1550+qSin(x[i]); }
  plot->addGraph()->setData(x,y); plot->rescaleAxes(); plot->replot();
  QCoreApplication::processEvents();
  QSignalSpy renders(plot,&QCustomPlot::afterReplot);
  const auto original=w.size();
  auto burst=[&] {
   for(int i=1;i<=60;++i) w.resize(original+QSize(i,i/2));
   QCoreApplication::processEvents();
  };
  QElapsedTimer clock; clock.start(); burst();
  const auto baselineMs=clock.elapsed(); const int baseline=renders.count();
  QVERIFY(baseline>10);
  w.resize(original); QCoreApplication::processEvents(); renders.clear();
  const auto hwnd=reinterpret_cast<HWND>(w.winId());
  RECT bounds; GetWindowRect(hwnd,&bounds);
  SendMessage(hwnd,WM_SIZING,WMSZ_BOTTOMRIGHT,reinterpret_cast<LPARAM>(&bounds));
  clock.restart(); burst(); const auto throttledMs=clock.elapsed();
  const int during=renders.count();
  QVERIFY2(during<baseline/2,"Each native size change must not synchronously re-render the plot");
  // A timer frame must apply the latest size while the mouse is still held.
  const auto beforeFrame=plot->size();
  w.resize(w.size()+QSize(20,20)); QTest::qWait(80);
  QVERIFY(plot->size()!=beforeFrame);
  SendMessage(hwnd,WM_EXITSIZEMOVE,0,0);
  QVERIFY(w.layout()->isEnabled());
  QCOMPARE(w.layout()->geometry(),w.rect());
  QCOMPARE(plot->viewport(),plot->rect());
  const auto finalPlot=plot->size();
  w.resize(w.size()+QSize(20,20)); QCoreApplication::processEvents();
  QVERIFY(plot->size()!=finalPlot);
  QCOMPARE(plot->graph(0)->dataCount(),20000);
  // Cancellation must restore ordinary layout as well.
  SendMessage(hwnd,WM_SIZING,WMSZ_BOTTOMRIGHT,reinterpret_cast<LPARAM>(&bounds));
  SendMessage(hwnd,WM_CANCELMODE,0,0); QVERIFY(w.layout()->isEnabled());
  qInfo()<<"Resize burst: immediate"<<baseline<<"replots,"<<baselineMs<<"ms; coalesced"<<during<<"replots,"<<throttledMs<<"ms";
 }
 void framelessResizeHitTests() {
  using namespace fosd::ui::classic;
  WorkbenchView w; w.resize(1500,1000); w.show();
  QVERIFY(QTest::qWaitForWindowExposed(&w));
  const auto hwnd=reinterpret_cast<HWND>(w.winId());
  auto hit=[&](int x,int y) {
   RECT r; GetWindowRect(hwnd,&r);
   return SendMessage(hwnd,WM_NCHITTEST,0,MAKELPARAM(r.left+x,r.top+y));
  };
  RECT r; QVERIFY(GetWindowRect(hwnd,&r));
  const int right=r.right-r.left-2, bottom=r.bottom-r.top-2;
  QCOMPARE(hit(1,1),LRESULT(HTTOPLEFT));
  QCOMPARE(hit(right,1),LRESULT(HTTOPRIGHT));
  QCOMPARE(hit(1,bottom),LRESULT(HTBOTTOMLEFT));
  QCOMPARE(hit(right,bottom),LRESULT(HTBOTTOMRIGHT));
  QCOMPARE(hit(1,bottom/2),LRESULT(HTLEFT));
  QCOMPARE(hit(right,bottom/2),LRESULT(HTRIGHT));
  QCOMPARE(hit(right/2,1),LRESULT(HTTOP));
  QCOMPARE(hit(right/2,bottom),LRESULT(HTBOTTOM));
  QCOMPARE(hit(right/2,bottom/2),LRESULT(HTCLIENT));
  const int minWidth=w.minimumWidth();
  w.setFixedWidth(w.width());
  QVERIFY(hit(1,bottom/2)!=HTLEFT);
  w.setMinimumWidth(minWidth); w.setMaximumWidth(QWIDGETSIZE_MAX);
  const auto isResize=[](LRESULT code){ return code>=HTLEFT && code<=HTBOTTOMRIGHT; };
  w.showMaximized(); QCoreApplication::processEvents();
  QVERIFY(!isResize(hit(1,1)));
  w.showFullScreen(); QCoreApplication::processEvents();
  QVERIFY(!isResize(hit(1,1)));
 }
#endif
 void pairedThemes() {
  using namespace fosd::ui::classic;
  WorkbenchView w; w.resize(1500,1000); w.show();
  QVERIFY(QTest::qWaitForWindowExposed(&w));
  auto *connection=w.panelWidget(PanelId::Connection);
  auto *tabs=connection->findChild<QTabWidget *>("tabWidget");
  auto *nav=w.findChild<QPushButton *>("pbtn_JieTiaoYi");
  for (bool dark : {true, false, true}) {
   w.applyTheme(dark); QCoreApplication::processEvents();
   QCOMPARE(w.palette().color(QPalette::Window),QColor(dark ? "#171e2b" : "#f1f4f8"));
   QCOMPARE(connection->palette().color(QPalette::Text),QColor(dark ? "#e6edf7" : "#243247"));
   QVERIFY(nav->isChecked()); QVERIFY(nav->styleSheet().isEmpty());
   QVERIFY(tabs->styleSheet().isEmpty());
   QVERIFY(QFile::exists(dark ? ":/fosd-ui/icons/chevron-dark.svg" : ":/fosd-ui/icons/chevron-light.svg"));
   w.grab().save(dark ? "theme-connection-dark.png" : "theme-connection-light.png");
   w.showPanel(PanelId::ChannelCalibration);
   auto *calibration=w.panelWidget(PanelId::ChannelCalibration);
   QCOMPARE(calibration->palette().color(QPalette::WindowText),w.palette().color(QPalette::WindowText));
   calibration->grab().save(dark ? "theme-calibration-dark.png" : "theme-calibration-light.png");
   w.closePanel(PanelId::ChannelCalibration);
   w.selectPage(1); QCoreApplication::processEvents();
   w.grab().save(dark ? "theme-scene-dark.png" : "theme-scene-light.png");
   w.selectPage(0);
  }
 }
 void previewWithoutBusinessModel() {
  using namespace fosd::ui::classic;
  WorkbenchView view;
  WorkbenchPresenter presenter(&view);
  presenter.initialize();
  view.show(); QVERIFY(QTest::qWaitForWindowExposed(&view));
  auto *connection=view.panelWidget(PanelId::Connection);
  connection->findChild<QPushButton *>("pbtn_ChannelCalibration")->click();
  QTRY_VERIFY(view.panelWidget(PanelId::ChannelCalibration)->isVisible());
  view.closePanel(PanelId::ChannelCalibration);
  QVERIFY(!view.panelWidget(PanelId::ScenePanel)->isEnabled());
  view.findChild<QPushButton *>("pbtn_3DPlot")->click();
  QTRY_VERIFY(view.panelWidget(PanelId::Visualization)->isVisible());
 }
 void sceneAppearanceSelectionAndToolbar() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("scene.ini").toUtf8());
  TestWindow w; w.resize(1600,1000); w.show(); w.selectPage(1);
  QVERIFY(QTest::qWaitForWindowExposed(&w));
  auto *scene=w.panelWidget(PanelId::ScenePanel); auto *page=w.panelWidget(PanelId::Visualization);
  auto *status=page->findChild<QStatusBar *>("statusBar_SelectedPoint"); QVERIFY(status);
  QTRY_VERIFY(status->isVisible()); QVERIFY(status->currentMessage().contains(QString::fromUtf8("未选择")));
  auto *start=page->findChild<QAction *>("actionStart"); auto *pause=page->findChild<QAction *>("actionPause");
  auto *stop=page->findChild<QAction *>("actionStop"); auto *reset=page->findChild<QAction *>("actionReset");
  QTRY_VERIFY(!start->isEnabled()&&!pause->isEnabled()&&!stop->isEnabled()&&!reset->isEnabled());
  auto &view=w.model().visualization();
  auto *modelPanel=w.panelWidget(PanelId::ModelPanel);
  QVERIFY(modelPanel->findChild<QPushButton *>("pbtn_ModelLoad")->isEnabled());
  QVERIFY(!modelPanel->findChild<QPushButton *>("pbtn_ModelClear")->isEnabled());
  QVERIFY(!scene->findChild<QWidget *>("cbox_AxisAble")->isEnabled());
  QVERIFY(!scene->findChild<QWidget *>("cbox_GridAble")->isEnabled());
  QVERIFY(!scene->findChild<QWidget *>("pbtn_ModelColor")->isEnabled());
  QVERIFY(!modelPanel->findChild<QWidget *>("pbtn_ScalarSet")->isEnabled());
  QVERIFY(!w.panelWidget(PanelId::KeyPointEdit)->isEnabled());
  {
   VisualizationPresenter presenter(PanelId::ScenePanel,w.panelView(PanelId::ScenePanel),&w);
   presenter.setModel(&w.model()); QSignalSpy requests(&presenter,&PanelPresenter::actionFinished);
   presenter.execute("pbtn_ModelColor"); // Must not open a dialog.
   presenter.execute("cbox_AxisAble");
   ModelPanelPresenter modelPresenter(PanelId::ModelPanel,w.panelView(PanelId::ModelPanel),&w);
   modelPresenter.setModel(&w.model()); QSignalSpy modelRequests(&modelPresenter,&PanelPresenter::actionFinished);
   modelPresenter.execute("pbtn_ScalarSet");
   QTest::qWait(30); QCOMPARE(requests.count(),0); QCOMPARE(modelRequests.count(),0);
  }
  QSignalSpy failedLoad(&view,&visualization::VisualizationControl::errorOccurred);
  w.selectedFile=dir.filePath("missing.stl");
  modelPanel->findChild<QPushButton *>("pbtn_ModelLoad")->click();
  QTRY_COMPARE(failedLoad.count(),1);
  QVERIFY(!view.originalModel()); QVERIFY(!scene->isEnabled());
  // The restriction is frontend-only; backend environment APIs stay usable.
  view.showAxes(true); QVERIFY(view.axesSettings().visible); view.showAxes(false);
  const auto model=dir.filePath("model.stl"); QFile file(model); QVERIFY(file.open(QIODevice::WriteOnly));
  file.write("solid t\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid t\n"); file.close();
  QSignalSpy accepted(&w.model().calculation(),&Calculation::CalculationControl::modelAccepted);
  QVERIFY(view.loadModel(model)); QTRY_COMPARE(accepted.count(),1);
  QTRY_VERIFY(scene->isEnabled());
  auto *keyCount=w.panelWidget(PanelId::InfoPanel)->findChild<QLabel *>("lab_NumKeyPoints");
  QVERIFY(keyCount); QTRY_COMPARE(keyCount->text(),QString("3"));
  const auto initialKeys=w.model().keyPoints();
  QVERIFY(w.model().setKeyPoints(initialKeys.mid(1))); QTRY_COMPARE(keyCount->text(),QString("2"));
  QVERIFY(w.model().setKeyPoints(initialKeys)); QTRY_COMPARE(keyCount->text(),QString("3"));
  QVERIFY(modelPanel->findChild<QPushButton *>("pbtn_ModelClear")->isEnabled());
  QVERIFY(modelPanel->findChild<QWidget *>("pbtn_ScalarSet")->isEnabled());
  QVERIFY(w.panelWidget(PanelId::KeyPointEdit)->isEnabled());
  QTRY_VERIFY(start->isEnabled()&&!pause->isEnabled()&&!stop->isEnabled()&&reset->isEnabled());
  auto choose=[&](const char *name,const QColor &color,bool accept) {
   QTimer::singleShot(30,[color,accept] {
    auto *dialog=qobject_cast<QColorDialog *>(QApplication::activeModalWidget()); QVERIFY(dialog);
    dialog->setCurrentColor(color); dialog->grab().save("scene-color-dialog.png");
    if(accept)dialog->accept(); else dialog->reject();
   });
   scene->findChild<QPushButton *>(name)->click();
  };
  auto *mode=scene->findChild<QComboBox *>("cbox_BackgroundMode"); QVERIFY(mode);
  QTRY_VERIFY(!scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->isEnabled());
  choose("pbtn_BackgroundBottomColor",QColor("#17324d"),true);
  QTRY_COMPARE(scene->findChild<QPushButton *>("pbtn_BackgroundBottomColor")->text(),QString("#17324d"));
  QVERIFY(!view.backgroundSettings().gradient);
  const auto bottom=view.backgroundSettings().bottomColor;
  choose("pbtn_BackgroundBottomColor",Qt::red,false); QTest::qWait(50);
  QVERIFY(view.backgroundSettings().bottomColor==bottom);
  QTest::keyClick(mode,Qt::Key_End); QTRY_VERIFY(view.backgroundSettings().gradient);
  QTRY_VERIFY(scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->isEnabled());
  choose("pbtn_BackgroundTopColor",QColor("#86aed4"),true);
  QTRY_COMPARE(scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->text(),QString("#86aed4"));
  choose("pbtn_ModelColor",QColor("#d48528"),true);
  QTRY_COMPARE(scene->findChild<QPushButton *>("pbtn_ModelColor")->text(),QString("#d48528"));
  auto saved=view.modelColor(); QVERIFY(!view.setModelColor({{-1,0,0}})); QVERIFY(view.modelColor()==saved);
  visualization::VisualizationControl restored;
  QVERIFY(restored.modelColor()==saved); QVERIFY(restored.backgroundSettings().gradient);
  QVERIFY(restored.backgroundSettings().bottomColor==bottom);
  start->trigger(); QTRY_VERIFY(!start->isEnabled()&&pause->isEnabled()&&stop->isEnabled());
  pause->trigger(); QTRY_VERIFY(start->isEnabled()&&!pause->isEnabled()&&stop->isEnabled());
  start->trigger(); QTRY_VERIFY(!start->isEnabled()&&pause->isEnabled());
  stop->trigger(); QTRY_VERIFY(start->isEnabled()&&!pause->isEnabled()&&!stop->isEnabled());
  view.setExpectedModelRevision(7); view.setDeformationScale(2);
  QVERIFY(view.selectPoint(0).valid); QTRY_VERIFY(status->currentMessage().simplified().contains("ID: 0"));
  core::CalculationResult result; result.revision=1; result.modelRevision=7;
  for(auto id:view.originalModel()->keys())result.displacementMap.insert(id,{1,2,3});
  view.applyResult(std::make_shared<const core::CalculationResult>(result));
  QTRY_VERIFY(status->currentMessage().remove(' ').contains("(102.00,4.00,6.00)"));
  QVERIFY(status->currentMessage().remove(' ').contains("(100.00,0.00,0.00)"));
  const auto aligned=status->currentMessage();
  w.grab().save("scene-appearance-selected-point.png");
  result.revision=2;
  for(auto id:view.originalModel()->keys())result.displacementMap[id]={-600.125,0,0};
  view.applyResult(std::make_shared<const core::CalculationResult>(result));
  QTRY_VERIFY(status->currentMessage().contains("-1100.25"));
  QCOMPARE(status->currentMessage().size(),aligned.size());
  QCOMPARE(status->currentMessage().indexOf(QString::fromUtf8("变形位置")),aligned.indexOf(QString::fromUtf8("变形位置")));
  QCOMPARE(QFontMetrics(status->font()).horizontalAdvance(status->currentMessage()),QFontMetrics(status->font()).horizontalAdvance(aligned));
  reset->trigger(); QTRY_VERIFY(status->currentMessage().remove(' ').contains(QString::fromUtf8("变形位置:(100.00,0.00,0.00)")));
  view.clearSelection(); QTRY_VERIFY(status->currentMessage().contains(QString::fromUtf8("未选择")));
  QCOMPARE(status->currentMessage().size(),aligned.size());
  w.panelWidget(PanelId::ModelPanel)->findChild<QPushButton *>("pbtn_ModelClear")->click();
  QTRY_VERIFY(!start->isEnabled()&&!pause->isEnabled()&&!stop->isEnabled()&&!reset->isEnabled());
  QTRY_VERIFY(!scene->isEnabled());
  QTRY_COMPARE(keyCount->text(),QString("0"));
  QVERIFY(!modelPanel->findChild<QWidget *>("pbtn_ScalarSet")->isEnabled());
  QVERIFY(!w.panelWidget(PanelId::KeyPointEdit)->isEnabled());
  QVERIFY(modelPanel->findChild<QPushButton *>("pbtn_ModelLoad")->isEnabled());
  w.grab().save("scene-no-model-disabled.png");
  QVERIFY(view.loadModel(model)); QTRY_COMPARE(accepted.count(),2); QTRY_VERIFY(scene->isEnabled());
  QVERIFY(scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->isEnabled()); // Gradient retained.
  QTest::keyClick(mode,Qt::Key_Home); QTRY_VERIFY(!view.backgroundSettings().gradient);
  QTRY_VERIFY(!scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->isEnabled());
  modelPanel->findChild<QPushButton *>("pbtn_ModelClear")->click(); QTRY_VERIFY(!scene->isEnabled());
  QVERIFY(view.loadModel(model)); QTRY_COMPARE(accepted.count(),3); QTRY_VERIFY(scene->isEnabled());
  QVERIFY(!scene->findChild<QPushButton *>("pbtn_BackgroundTopColor")->isEnabled()); // Solid retained.
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void calibrationEntryFilteringAndAddressRefresh() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("calibration.ini").toUtf8());
  Calculation::CalculationConfiguration initial; initial.strainCalibration.A=1;
  initial.strainCalibration.originalWavelength.assign(12,1550.);
  for(int i=0;i<12;++i){initial.channelCalibration.channelIndices.push_back(i);initial.channelCalibration.calibrationFactors.push_back(1.);}
  QString error; QVERIFY(config::ParameterStore().write(initial,true,&error));
  TestWindow w; w.resize(1360,900); w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w)); QTest::qWait(180);
  QVERIFY(!w.findChild<QPushButton *>("pbtn_2DPlot"));
  auto *panel=w.panelWidget(PanelId::Connection); auto *tabs=panel->findChild<QTabWidget *>("tabWidget");
  auto *filter=panel->findChild<QWidget *>("groupBox_select"); auto *wave=panel->findChild<QWidget *>("tab_guangpu");
  QVERIFY(wave->isAncestorOf(filter));
  tabs->setCurrentWidget(panel->findChild<QWidget *>("tab_udp")); QVERIFY(!filter->isVisible());
  auto *addresses=panel->findChild<QComboBox *>("cbox_LocakIP");
  addresses->addItem("192.0.2.123"); addresses->setCurrentIndex(addresses->findText("127.0.0.1"));
  QSignalSpy completed(w.presenter().panelPresenter(PanelId::Connection),&PanelPresenter::actionFinished);
  panel->findChild<QPushButton *>("pbtn_RefreshLocalAddress")->click();
  QCOMPARE(completed.count(),1);
  QCOMPARE(completed.first()[0].toString(),QString("pbtn_RefreshLocalAddress"));
  QVERIFY(completed.first()[1].toBool());
  QTRY_COMPARE(addresses->findText("192.0.2.123"),-1); QCOMPARE(addresses->currentText(),QString("127.0.0.1"));
  auto *calibration=w.panelWidget(PanelId::ChannelCalibration); QVERIFY(calibration);
  QCOMPARE(calibration->objectName(),QString("channlCalibration"));
  panel->findChild<QPushButton *>("pbtn_ChannelCalibration")->click(); QTRY_VERIFY(calibration->isVisible());
  auto *order=calibration->findChild<QLineEdit *>("edit_Sensor1Order");
  auto *factor=calibration->findChild<QLineEdit *>("edit_Sensor1Para"); QVERIFY(order && factor);
  for(int i=1;i<=12;++i) {
   auto *offset=calibration->findChild<QLineEdit *>(QString("edit_Sensor%1Offset").arg(i)); QVERIFY(offset);
   QTRY_COMPARE(offset->text(),QString("0")); // Legacy configurations have empty offsets.
   offset->setText(QString::number((i-6)*0.25));
  }
  auto *offset=calibration->findChild<QLineEdit *>("edit_Sensor1Offset");
  QTRY_COMPARE(order->text(),QString("1")); order->setText("2"); factor->setText("1.75");
  QSignalSpy accepted(&w.model().calculation(),&Calculation::CalculationControl::configurationAccepted);
  calibration->findChild<QPushButton *>("pbtn_SenerSet")->click(); QTRY_COMPARE(accepted.count(),1);
  Calculation::CalculationConfiguration saved; QVERIFY(config::ParameterStore().read(&saved,true,&error));
  QCOMPARE(saved.channelCalibration.channelIndices[0],1); QCOMPARE(saved.channelCalibration.calibrationFactors[0],1.75);
  QCOMPARE(saved.channelCalibration.calibrationOffsets.size(),size_t(12));
  for(int i=0;i<12;++i)QCOMPARE(saved.channelCalibration.calibrationOffsets[size_t(i)],(i-5)*0.25);
  for(const auto &invalid:{"bad","nan","inf","1e999",""}) {
   offset->setText(invalid); calibration->findChild<QPushButton *>("pbtn_SenerSet")->click(); QTest::qWait(60);
   QCOMPARE(accepted.count(),1);
   QVERIFY(config::ParameterStore().read(&saved,true,&error)); QCOMPARE(saved.channelCalibration.calibrationOffsets[0],-1.25);
  }
  offset->setText("-1.25");
  order->setText("99"); calibration->findChild<QPushButton *>("pbtn_SenerSet")->click(); QTest::qWait(100);
  QCOMPARE(accepted.count(),1); QVERIFY(config::ParameterStore().read(&saved,true,&error)); QCOMPARE(saved.channelCalibration.channelIndices[0],1);
  offset->setText("999");
  calibration->findChild<QPushButton *>("pbtn_SenerCancel")->click(); QTRY_VERIFY(!calibration->isVisible());
  panel->findChild<QPushButton *>("pbtn_ChannelCalibration")->click(); QTRY_COMPARE(order->text(),QString("2"));
  QTRY_COMPARE(offset->text(),QString("-1.25"));
  for(int i=1;i<=12;++i)
   QCOMPARE(calibration->findChild<QLineEdit *>(QString("edit_Sensor%1Offset").arg(i))->text().toDouble(),(i-6)*0.25);
  calibration->grab().save("channel-calibration.png"); calibration->close();
  tabs->setCurrentWidget(wave); QVERIFY(filter->isVisible());
  for(int i=1;i<=16;++i)panel->findChild<QCheckBox *>(QString("ckbox_CH%1").arg(i))->setChecked(i==16);
  panel->findChild<QPushButton *>("pbtn_SelectCH")->click();
  w.model().protocol().parseFrame(communication::Channel::Udp,jcs(0));
  auto *plot=panel->findChild<QCustomPlot *>("cplot_Spectrum"); QTRY_COMPARE(plot->graphCount(),6);
  for(int i=0;i<plot->graphCount();++i)QVERIFY(plot->graph(i)->name().startsWith("CH16_"));
  auto *table=panel->findChild<QTableView *>("tablev_Wavelength"); QVERIFY(table);
  QTRY_COMPARE(table->model()->rowCount(),12); // Curve filtering never filters the raw wavelength table.
  w.grab().save("connection-wave-filter.png");
  w.findChild<QPushButton *>("pbtn_3DPlot")->click(); QTest::qWait(50);
  w.findChild<QPushButton *>("pbtn_JieTiaoYi")->click(); QTRY_VERIFY(panel->isVisible());
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void coloredTrafficLogs() {
  using namespace fosd; using namespace fosd::ui::classic; using namespace communication;
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("traffic.ini").toUtf8());
  SerialConfig serialConfig; serialConfig.portName="COM42"; QString error;
  QVERIFY(config::ParameterStore().write(serialConfig,true,&error));
  TestWindow w; w.applyTheme(false); w.resize(1360,900); w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w)); QTest::qWait(150);
  auto *panel=w.panelWidget(PanelId::Connection); auto &transport=w.model().protocol().communication();
  auto *serial=panel->findChild<QPlainTextEdit *>("ptedit_SerialRec");
  auto *udp=panel->findChild<QPlainTextEdit *>("ptedit_UdpRec");
  auto *tcp=panel->findChild<QPlainTextEdit *>("ptedit_TcpRec");
  emit transport.rawDataReceived(Channel::Serial,"serial reply\r\n",{});
  const QByteArray serialOut("serial out\r\n"); const Endpoint none;
  QVERIFY(QMetaObject::invokeMethod(w.presenter().panelPresenter(PanelId::Connection),"rawSent",Qt::DirectConnection,
      Q_ARG(fosd::communication::Channel,Channel::Serial),Q_ARG(QByteArray,serialOut),Q_ARG(fosd::communication::Endpoint,none)));
  QTRY_VERIFY(serial->toPlainText().contains("serial out\\r\\n"));
  QVERIFY(serial->toPlainText().contains("COM42 @115200"));
  QUdpSocket remote; QVERIFY(remote.bind(QHostAddress(QHostAddress::LocalHost),quint16(0)));
  QUdpSocket reserve; QVERIFY(reserve.bind(QHostAddress(QHostAddress::LocalHost),quint16(0)));
  const quint16 localPort=reserve.localPort(); reserve.close();
  UdpConfig udpConfig; udpConfig.bindAddress=QHostAddress::LocalHost; udpConfig.localPort=localPort;
  udpConfig.defaultPeer.address=QHostAddress::LocalHost; udpConfig.defaultPeer.port=remote.localPort();
  QSignalSpy state(&transport,&ConnectControl::stateChanged); transport.openUdp(udpConfig); QTRY_VERIFY(!state.isEmpty());
  QTest::keyClick(panel->findChild<QComboBox *>("cbox_UdpDisplayMode"),Qt::Key_End);
  const QByteArray udpReply("udp reply\r\n"); QCOMPARE(remote.writeDatagram(udpReply,QHostAddress::LocalHost,localPort),qint64(udpReply.size()));
  QTRY_VERIFY(udp->toPlainText().contains("udp reply\\r\\n"));
  transport.send(Channel::Udp,"udp out\r\n",udpConfig.defaultPeer);
  QTRY_VERIFY(remote.hasPendingDatagrams()); QByteArray datagram(int(remote.pendingDatagramSize()),'\0'); remote.readDatagram(datagram.data(),datagram.size());
  QCOMPARE(datagram,QByteArray("udp out\r\n")); QTRY_VERIFY(udp->toPlainText().contains("udp out\\r\\n"));
  QVERIFY(udp->toPlainText().contains(QString::number(localPort))); QVERIFY(udp->toPlainText().contains(QString::number(remote.localPort())));
  QTcpServer server; QVERIFY(server.listen(QHostAddress::LocalHost,0));
  TcpConfig tcpConfig; tcpConfig.host="127.0.0.1"; tcpConfig.port=server.serverPort(); transport.openTcp(tcpConfig);
  QTRY_VERIFY(server.hasPendingConnections()); QScopedPointer<QTcpSocket> peer(server.nextPendingConnection());
  QTest::keyClick(panel->findChild<QComboBox *>("cbox_TcpDisplayMode"),Qt::Key_End);
  peer->write("tcp reply\r\n"); peer->flush(); QTRY_VERIFY(tcp->toPlainText().contains("tcp reply\\r\\n"));
  transport.send(Channel::Tcp,"tcp out\r\n"); QTRY_VERIFY(peer->bytesAvailable()>0);
  QCOMPARE(peer->readAll(),QByteArray("tcp out\r\n")); QTRY_VERIFY(tcp->toPlainText().contains("tcp out\\r\\n"));
  QVERIFY(tcp->toPlainText().contains(QString::number(server.serverPort())));
  auto checkColors=[](QPlainTextEdit *log,bool dark) {
   bool rx=false,tx=false;
   for(auto block=log->document()->begin();block.isValid();block=block.next()) {
    const int kind=block.userState(); if(kind!=1 && kind!=2)continue;
    const auto fragment=block.begin().fragment();
    const QColor expected(kind==2 ? (dark?"#73b7ff":"#1d4ed8") : (dark?"#63df91":"#15803d"));
    if(fragment.charFormat().foreground().color()!=expected)return false;
    rx|=kind==1;tx|=kind==2;
   }
   return rx&&tx;
  };
  for(auto *log:{serial,udp,tcp}) {
   QVERIFY(checkColors(log,false)); QVERIFY(log->document()->blockCount()>=2);
   QCOMPARE(log->toPlainText().split('\n').size(), log->document()->blockCount()*2);
   for(auto block=log->document()->begin();block.isValid();block=block.next()) {
    const auto lines=block.text().split(QChar(0x2028));
    QCOMPARE(lines.size(),2); QVERIFY(lines[0].startsWith('['));
    QVERIFY(!lines[0].contains(QString::fromUtf8("预览")));
    QVERIFY(lines[1].contains(QString::fromUtf8("预览UTF-8：")));
   }
   QVERIFY(QRegularExpression("\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\]").match(log->toPlainText()).hasMatch());
  }
  auto *tabs=panel->findChild<QTabWidget *>("tabWidget"); tabs->setCurrentWidget(panel->findChild<QWidget *>("tab_tcp"));
  w.applyTheme(true); for(auto *log:{serial,udp,tcp})QVERIFY(checkColors(log,true));
  w.grab().save("connection-traffic-dark.png"); w.applyTheme(false); w.grab().save("connection-traffic-light.png");
  auto *cleanup=w.presenter().findChild<QTimer *>("receiveLogCleanupTimer"); QVERIFY(cleanup); cleanup->start(30);
  QTRY_VERIFY(serial->toPlainText().isEmpty()&&udp->toPlainText().isEmpty()&&tcp->toPlainText().isEmpty()); cleanup->stop();
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void connectionLayoutSavingAndSpectrum() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir; qputenv("FOSD_CONFIG_FILE",dir.filePath("interface.ini").toUtf8());
  TestWindow w; w.resize(1360,900); w.show(); QVERIFY(QTest::qWaitForWindowExposed(&w)); QTest::qWait(150);
  auto *panel=w.panelWidget(PanelId::Connection); auto *view=w.panelView(PanelId::Connection);
  for(auto name:{"groupBox_3","pbtn_Diagnosis","cbox_TcpRole","pbtn_SaveStatic","lab_OperationStatus"})
   QVERIFY(!panel->findChild<QWidget *>(name));
  auto *mode=panel->findChild<QComboBox *>("cbox_CommuteChoose"); QVERIFY(mode); QCOMPARE(mode->count(),3);
  QTest::keyClick(mode,Qt::Key_End);
  QTRY_VERIFY(!panel->findChild<QLineEdit *>("edit_LocalPort")->isEnabled());
  QVERIFY(panel->findChild<QLineEdit *>("edit_TargetPort")->isEnabled());
  QTest::keyClick(mode,Qt::Key_Up);
  QTRY_VERIFY(panel->findChild<QLineEdit *>("edit_LocalPort")->isEnabled());
  QVERIFY(!panel->findChild<QLineEdit *>("edit_TargetPort")->isEnabled());
  QTest::keyClick(mode,Qt::Key_Home);
  QTRY_VERIFY(panel->findChild<QLineEdit *>("edit_TargetPort")->isEnabled());
  QCOMPARE(panel->findChild<QPushButton *>("pbtn_RefreshSerial")->text(),QString::fromUtf8("刷新"));
  auto *save=panel->findChild<QPushButton *>("pbtn_SaveDynamic"); QVERIFY(save->isCheckable());
  save->click(); QVERIFY(!save->isChecked()); // File selection was cancelled.
  w.selectedFile=dir.filePath("record.csv");
  QSignalSpy started(&w.model().calculation().storage(),&storage::StorageControl::started);
  QSignalSpy stopped(&w.model().calculation().storage(),&storage::StorageControl::stopped);
  save->click(); QTRY_COMPARE(started.count(),1);
  QTRY_VERIFY(save->isChecked() && save->isEnabled());
  QCOMPARE(save->text(),QString::fromUtf8("停止保存"));
  w.grab().save("connection-saving.png");
  w.model().protocol().parseFrame(communication::Channel::Udp,jcs(0));
  QTest::qWait(100); save->click(); QTRY_COMPARE(stopped.count(),1);
  QTRY_VERIFY(!save->isChecked() && save->isEnabled());
  QCOMPARE(save->text(),QString::fromUtf8("开始保存")); QVERIFY(QFileInfo(w.selectedFile).size()>0);
  w.selectedFile=dir.filePath("invalid.extension"); save->click();
  QTRY_VERIFY(!save->isChecked() && save->isEnabled());
  // A valid extension with an unwritable file target exercises asynchronous failure.
  w.selectedFile=dir.filePath("directory.csv"); QVERIFY(QDir().mkdir(w.selectedFile));
  save->click(); QTRY_VERIFY(!save->isChecked() && save->isEnabled());
  auto *tabs=panel->findChild<QTabWidget *>("tabWidget"); tabs->setCurrentWidget(panel->findChild<QWidget *>("tab_guangpu"));
  auto *plot=panel->findChild<QCustomPlot *>("cplot_Spectrum"); QVERIFY(plot);
  auto *follow=panel->findChild<QCheckBox *>("ckbox_SpectrumAutoRange");
  auto update=[&](const QVariantList &samples) {view->applyFields({{"cplot_Spectrum",QVariantMap{{"series",QVariantList{
    QVariantMap{{"name","CH1_1"},{"samples",samples}}}}}}});};
  const QVariantList samples{QVariantList{0.,1550.},QVariantList{1.,1550.5},QVariantList{2.,1551.}};
  update(samples); plot->replot(); QTest::qWait(50);
  QVERIFY(plot->interactions().testFlag(QCP::iRangeDrag)); QVERIFY(plot->interactions().testFlag(QCP::iRangeZoom));
  QVERIFY(plot->interactions().testFlag(QCP::iSelectPlottables));
  const QPoint point(qRound(plot->xAxis->coordToPixel(1)),qRound(plot->yAxis->coordToPixel(1550.5)));
  QTest::mouseClick(plot,Qt::LeftButton,Qt::NoModifier,point);
  QTRY_VERIFY(panel->findChild<QLabel *>("lab_SpectrumPointValue")->text().contains("1550.500000"));
  QVERIFY(!follow->isChecked());
  plot->xAxis->setRange(.5,1.5); const auto range=plot->xAxis->range();
  auto more=samples; more.append(QVariant(QVariantList{3.,1552.})); update(more);
  QCOMPARE(plot->xAxis->range().lower,range.lower); QCOMPARE(plot->xAxis->range().upper,range.upper);
  // Unknown presentation fields must preserve the active multi-series curve and viewport.
  view->applyFields({{"cplot_Spectrum",QVariantMap{{"unsupported",true}}}});
  QCOMPARE(plot->graphCount(),1); QCOMPARE(plot->graph(0)->dataCount(),4);
  QCOMPARE(plot->graph(0)->name(),QString("CH1_1"));
  QCOMPARE(plot->xAxis->range().lower,range.lower); QCOMPARE(plot->xAxis->range().upper,range.upper);
  QVERIFY(panel->findChild<QLabel *>("lab_SpectrumPointValue")->text().contains("1550.500000"));
  const QPoint dragStart=plot->axisRect()->rect().center();
  QTest::mousePress(plot,Qt::LeftButton,Qt::NoModifier,dragStart);
  QMouseEvent drag(QEvent::MouseMove,QPointF(dragStart+QPoint(40,20)),Qt::NoButton,Qt::LeftButton,Qt::NoModifier);
  QApplication::sendEvent(plot,&drag);
  QTest::mouseRelease(plot,Qt::LeftButton,Qt::NoModifier,dragStart+QPoint(40,20));
  QVERIFY(!qFuzzyCompare(plot->xAxis->range().lower,range.lower));
  const double widthBefore=plot->xAxis->range().size();
  QWheelEvent wheel(QPointF(dragStart),QPointF(plot->mapToGlobal(dragStart)),QPoint(),QPoint(0,120),Qt::NoButton,Qt::NoModifier,Qt::NoScrollPhase,false);
  QApplication::sendEvent(plot,&wheel);
  QVERIFY(plot->xAxis->range().size()<widthBefore);
  w.applyTheme(true); QCOMPARE(plot->axisRect()->backgroundBrush().color(),QColor("#182231"));
  QCOMPARE(plot->xAxis->tickLabelColor(),QColor("#b1bfd2"));
  plot->replot(); w.grab().save("connection-spectrum-dark.png");
  w.applyTheme(false); QCOMPARE(plot->axisRect()->backgroundBrush().color(),QColor("#ffffff"));
  panel->findChild<QPushButton *>("pbtn_SpectrumReset")->click();
  QVERIFY(follow->isChecked()); QVERIFY(plot->xAxis->range().upper>=3.);
  plot->replot(); w.grab().save("connection-spectrum-light.png");
  w.selectedFile=dir.filePath("spectrum-export.png");
  panel->findChild<QPushButton *>("pbtn_SaveSpectrum")->click();
  QVERIFY(QFileInfo(w.selectedFile).size()>0);
  QVERIFY(!QImage(w.selectedFile).isNull());
  QCOMPARE(plot->graph(0)->dataCount(),4);
  tabs->setCurrentWidget(panel->findChild<QWidget *>("tab_usart")); w.grab().save("connection-serial-layout.png");
  qunsetenv("FOSD_CONFIG_FILE");
 }
 void serialQuickCommandsAndCrlf() {
  using namespace fosd; using namespace fosd::ui::classic;
  QTemporaryDir dir;
  qputenv("FOSD_CONFIG_FILE", dir.filePath("commands.ini").toUtf8());
  TestWindow w;
  QTest::qWait(100); // Apply initial configuration before editing commands.
  auto *panel = w.panelWidget(PanelId::Connection);
  auto &transport = w.model().protocol().communication();
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
  auto &transport = w.model().protocol().communication();
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
  const QString plain = QString::fromUtf8("中文 AB\\r\\n");
  QTRY_VERIFY(serialText->toPlainText().contains(plain));
  QTRY_VERIFY(udpText->toPlainText().contains(hex));
  QTest::keyClick(serialMode, Qt::Key_Home);
  QTRY_VERIFY(serialText->toPlainText().contains(QString::fromLatin1(bytes.mid(1).toHex(' ').toUpper())));
  QTest::keyClick(udpMode, Qt::Key_End);
  QTRY_VERIFY(udpText->toPlainText().contains(QString::fromUtf8("预览UTF-8：中文 AB\\r\\n")));
  const QString udpLog = udpText->toPlainText();
  const QByteArray binary = QByteArray::fromHex("00ff");
  emit transport.rawDataReceived(communication::Channel::Serial, binary, {});
  QTRY_VERIFY(serialText->toPlainText().contains("00 FF"));
  QTest::keyClick(serialMode, Qt::Key_End);
  QTest::qWait(150);
  QTest::keyClick(serialMode, Qt::Key_Home);
  QTRY_VERIFY(serialText->toPlainText().contains("00 FF"));
  // Each channel keeps its own mode and receive history.
  QCOMPARE(udpText->toPlainText(), udpLog);
  panel->findChild<QPushButton *>("pbtn_SerialClear")->click();
  QTRY_VERIFY(serialText->toPlainText().isEmpty());
  QTest::keyClick(serialMode, Qt::Key_End);
  QTest::qWait(150);
  QVERIFY(serialText->toPlainText().isEmpty());
  QCOMPARE(udpText->toPlainText(), udpLog);
  emit transport.rawDataReceived(communication::Channel::Serial, QByteArray(70000, 'A'), {});
  QTRY_VERIFY(serialText->toPlainText().contains("70000 B"));
  QVERIFY(!serialText->toPlainText().contains(QString(65,'A')));
  QVERIFY(serialText->toPlainText().size()<1000);
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
  auto &runtime = w.model();
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
  QVERIFY(contents.contains(QString::fromUtf8("\xE7\xB4\xAF\xE8\xAE\xA1=1")));
  QVERIFY(contents.contains("127.0.0.1:"));
  QVERIFY(contents.contains(QString::fromUtf8("序号=305419896")));
  QVERIFY(contents.contains(QString::fromUtf8("记录=2")));
  QVERIFY(contents.contains("CH1/2 1.5 nm"));
  QVERIFY(contents.contains("22 B"));
  const auto frameLines=contents.split('\n'); QCOMPARE(frameLines.size(),2);
  QVERIFY(frameLines[0].contains("22 B")); QVERIFY(!frameLines[0].contains("mp.wavelength"));
  QVERIFY(frameLines[1].contains("mp.wavelength"));
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
  QVERIFY(QMetaObject::invokeMethod(&w.presenter(), "refreshDisplays"));
  QTRY_COMPARE(log->document()->blockCount(), 1);
  QTRY_VERIFY(log->toPlainText().contains("mp.wavelength"));
  // Exercise the document's actual limit after more than 200 display intervals.
  for (int i = 0; i < 220; ++i) {
   emit runtime.protocol().frameParsed(saved);
   QVERIFY(QMetaObject::invokeMethod(&w.presenter(), "refreshDisplays"));
  }
  QTRY_COMPARE(log->document()->blockCount(), 200);
  QCOMPARE(log->toPlainText().split('\n').size(),400);
  for(auto block=log->document()->begin();block.isValid();block=block.next())
   QCOMPARE(block.text().split(QChar(0x2028)).size(),2);
  QVERIFY(log->toPlainText().size() < 100000);
  emit transport.rawDataReceived(communication::Channel::Serial, "serial text", {});
  QTRY_VERIFY(serial->toPlainText().contains("serial text"));
  auto *cleanup = w.presenter().findChild<QTimer *>("receiveLogCleanupTimer");
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
  QSignalSpy frames(&w.model().protocol(),&protocol::ProtocolControl::frameParsed);
  QSignalSpy configured(&w.model().calculation(),&Calculation::CalculationControl::configurationAccepted);
  cw->findChild<QPushButton *>("pbtn_OpenNet")->click(); QTest::qWait(150);
  QUdpSocket sender;
  auto send=[&](float delta){ QCOMPARE(sender.writeDatagram(jcs(delta),QHostAddress::LocalHost,port),qint64(jcs(delta).size())); };
  send(0); QTRY_COMPARE(frames.count(),1);
  cw->findChild<QPushButton *>("pbtn_WaveLengthReset")->click(); QTest::qWait(50);
  for(int i=0;i<20;++i) {send(0); QTRY_COMPARE(frames.count(),i+2);}
  QTRY_COMPARE(configured.count(),1);
  const auto model=dir.filePath("中文模型.stl"); QFile file(model); QVERIFY(file.open(QIODevice::WriteOnly));
  file.write("solid t\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 10 0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid t\n"); file.close();
  w.selectedFile=model; QSignalSpy models(&w.model().calculation(),&Calculation::CalculationControl::modelAccepted);
  w.panelWidget(PanelId::ModelPanel)->findChild<QPushButton *>("pbtn_ModelLoad")->click(); QTRY_COMPARE(models.count(),1);
  QTRY_VERIFY(w.findChild<QAction *>("actionStart")->isEnabled());
  w.findChild<QAction *>("actionStart")->trigger(); QTRY_VERIFY(w.findChild<QAction *>("actionPause")->isEnabled());
  w.selectPage(1); send(.02f);
  QTRY_VERIFY(w.model().calculation().dataModel()->latest());
  QCOMPARE(w.model().calculation().dataModel()->latest()->algorithmId,QString("HYY.IFE"));
  QTest::qWait(150); auto selected=w.model().visualization().selectPoint(0); QVERIFY(selected.valid);
  w.findChild<QAction *>("actionPause")->trigger(); QTest::qWait(50);
  auto frozen=w.model().visualization().selectedPoint().displayPositionMm;
  auto revision=w.model().calculation().dataModel()->latest()->revision;
  send(.04f); QTRY_VERIFY(w.model().calculation().dataModel()->latest()->revision>revision);
  QTest::qWait(100); QVERIFY(w.model().visualization().selectedPoint().displayPositionMm==frozen);
  w.selectedFile=dir.filePath("波长曲线.png"); cw->findChild<QPushButton *>("pbtn_SaveSpectrum")->click();
  QTRY_VERIFY(QFileInfo(w.selectedFile).size()>0); QVERIFY(!QImage(w.selectedFile).isNull());
  QSignalSpy started(&w.model().calculation().storage(),&storage::StorageControl::started);
  w.selectedFile=dir.filePath("波长.csv"); cw->findChild<QPushButton *>("pbtn_SaveDynamic")->click();
  QTRY_COMPARE(started.count(),1); send(.06f); QTest::qWait(100);
  cw->findChild<QPushButton *>("pbtn_SaveDynamic")->click();
  QTRY_VERIFY(!cw->findChild<QPushButton *>("pbtn_SaveDynamic")->isChecked());
  QTRY_VERIFY(QFileInfo(w.selectedFile).size()>0);
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
 w.selectPage(1);
 QTRY_VERIFY(w.panelWidget(fosd::ui::classic::PanelId::Visualization)->findChild<QStatusBar *>("statusBar_SelectedPoint")->isVisible());
 }};
int main(int argc,char **argv) { QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat()); QApplication a(argc,argv); WorkbenchMainTests t; return QTest::qExec(&t,argc,argv); }
#include "workbench_main_tests.moc"
