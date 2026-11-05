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
 QVERIFY(c->findChild<QComboBox *>("cbox_TcpRole")); QVERIFY(c->findChild<QPushButton *>("pbtn_CalculationStop"));
 for(auto name:{"pbtn_CmdStart","pbtn_ModelMerge","pbtn_SelectColor","edit_Xaxis","cbox_FellowAble"}) QVERIFY(!w.findChild<QWidget *>(name));
 }};
QTEST_MAIN(WorkbenchTests)
#include "WorkbenchTests.moc"
