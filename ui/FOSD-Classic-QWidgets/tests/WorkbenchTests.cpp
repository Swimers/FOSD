#include "FOSD-Classic-QWidgets/apps/WorkbenchView.h"
#include <QPushButton>
#include <QComboBox>
#include <QtTest>
using namespace fosd::ui::classic;
class WorkbenchTests:public QObject {
    Q_OBJECT

private slots: void retainedAndRemoved() {
 WorkbenchView w;
 for(int i=0;i<=int(PanelId::HardwareData);++i) QVERIFY(w.panelView(PanelId(i)));
 auto *c=w.panelWidget(PanelId::Connection);
 auto *mode=c->findChild<QComboBox *>("cbox_CommuteChoose"); QVERIFY(mode);
 QCOMPARE(mode->count(),3); QCOMPARE(mode->itemText(1),QString("TCP Server")); QCOMPARE(mode->itemText(2),QString("TCP Client"));
 QVERIFY(c->findChild<QPushButton *>("pbtn_CalculationStop"));
 for(auto name:{"cbox_TcpRole","pbtn_Diagnosis","groupBox_3","lab_OperationStatus","pbtn_SaveStatic"}) QVERIFY2(!c->findChild<QWidget *>(name),name);
 for(auto name:{"pbtn_CmdStart","pbtn_ModelMerge","pbtn_SelectColor","edit_Xaxis","cbox_FellowAble"}) QVERIFY2(!w.findChild<QWidget *>(name),name);
 }};
QTEST_MAIN(WorkbenchTests)
#include "WorkbenchTests.moc"
