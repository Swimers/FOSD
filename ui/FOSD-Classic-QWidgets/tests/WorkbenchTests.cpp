#include "../apps/mainwindow/views/WorkbenchView.h"
#include "../common/ViewFactory.h"
#include "../apps/mainwindow/presenters/WorkbenchPresenter.h"
#include "../apps/account/presenters/AccountPresenter.h"
#include "../apps/connection/views/connectwindow.h"
#include "../apps/account/views/login.h"
#include "../controls/renderhostwidget.h"
#include "../controls/qcustomplot.h"
#include "../controls/switchcontrol.h"
#include <QAction>
#include <QDockWidget>
#include <QFile>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QSignalSpy>
#include <QStackedWidget>
#include <QTableWidget>
#include <QThread>
#include <QtTest>
#include <atomic>

using namespace fosd::ui::classic;
class RunningThread final : public QThread {
public:
    ~RunningThread() override { quit(); wait(); }
};
class FakePanel final : public IView {
public:
    ViewEvents *events() const override { return nullptr; }
    QVariantMap input, output;
    QString status;
    int cleared = 0;
    QVariantMap values() const override { return input; }
    void applyFields(const QVariantMap &fields) override { output = fields; }
    void clearSensitiveFields() override { ++cleared; }
    void clearInputs() override { input.clear(); }
    void showStatus(const QString &message) override { status = message; }
};
class Backend final : public IBackend {
    Q_OBJECT
public:
    explicit Backend(std::atomic<QThread *> *thread) : observed(thread) {}
    bool supports(PanelId panel) const override { return featureForPanel(panel) == FeatureId::Connection || featureForPanel(panel) == FeatureId::Account; }
public slots:
    void execute(const UiRequest &request) override {
        observed->store(QThread::currentThread());
        emit handled(request);
        UiResponse response; response.id = request.id; response.panel = request.panel;
        response.success = true; response.message = "backend-confirmed";
        response.fields["edit_LocalPort"] = "7777";
        emit completed(response);
    }
signals:
    void handled(const fosd::ui::classic::UiRequest &request);
private:
    std::atomic<QThread *> *observed;
};
class WorkbenchTests final : public QObject {
    Q_OBJECT
private slots:
    void allForms_data();
    void allForms();
    void presenterResponseMatching();
    void navigationAndIsolation();
    void queuedBackendAndDisconnect();
    void tablesAndRenderHost();
    void typedSignalsAndStandaloneView();
    void backendScopesAndReplacement();
};
void WorkbenchTests::allForms_data() {
    QTest::addColumn<int>("panel");
    for (int index = int(PanelId::Main); index <= int(PanelId::Account); ++index)
        QTest::newRow(qPrintable(QString::number(index))) << index;
}
void WorkbenchTests::allForms() {
    QFETCH(int, panel);
    std::unique_ptr<QWidget> form(createPanelView(PanelId(panel)));
    QVERIFY(form); QVERIFY(!form->objectName().isEmpty());
    auto *contract = dynamic_cast<IView *>(form.get());
    QVERIFY(contract); QVERIFY(contract->events());
    QVERIFY(!QPixmap(":/icon/icon/3D_Plot.png").isNull());
    QVERIFY(QFile::exists(":/disp2d/disp2d/image2.jpg"));
    QVERIFY(QFile::exists(":/fosd-ui/styles/dark.qss"));
    if (PanelId(panel) == PanelId::Connection) QVERIFY(form->findChild<QCustomPlot *>("cplot_Spectrum"));
    if (PanelId(panel) == PanelId::Main) QVERIFY(form->findChild<SwitchControl *>("switch_Style"));
    if (PanelId(panel) == PanelId::Visualization) QVERIFY(form->findChild<RenderHostWidget *>("vtk_openGL"));
    if (PanelId(panel) == PanelId::Registration)
        QCOMPARE(form->findChild<QLineEdit *>("edit_NewPassword")->echoMode(), QLineEdit::Password);
    if (PanelId(panel) == PanelId::Recovery) {
        QCOMPARE(form->findChild<QLineEdit *>("edit_NewPassword1")->echoMode(), QLineEdit::Password);
        QCOMPARE(form->findChild<QLineEdit *>("edit_NewPassword1_2")->echoMode(), QLineEdit::Normal);
    }
}
void WorkbenchTests::typedSignalsAndStandaloneView() {
    ConnectionView view;
    QSignalSpy controls(&view, &ConnectionView::pbtn_OpenSerialRequested);
    QSignalSpy intent(view.events(), &ViewEvents::actionTriggered);
    view.findChild<QLineEdit *>("edit_LocalPort")->setText("4567");
    view.findChild<QPushButton *>("pbtn_OpenSerial")->click();
    QCOMPARE(controls.count(), 1); QCOMPARE(intent.count(), 1);
    QCOMPARE(controls[0][0].toMap().value("edit_LocalPort").toString(), QStringLiteral("4567"));
    QCOMPARE(qvariant_cast<PanelId>(intent[0][0]), PanelId::Connection);
    // A form can be used independently of the workbench with the same contract.
    std::atomic<QThread *> observed{nullptr}; Backend backend(&observed);
    PanelPresenter presenter(PanelId::Connection, &view); presenter.setBackend(&backend);
    view.findChild<QPushButton *>("pbtn_OpenNet")->click();
    QTRY_COMPARE(view.findChild<QLineEdit *>("edit_LocalPort")->text(), QStringLiteral("7777"));
}
void WorkbenchTests::backendScopesAndReplacement() {
    WorkbenchView view; WorkbenchPresenter presenter(&view); WorkbenchBackend registry;
    presenter.setBackends(&registry);
    auto *connection = registry.backend(FeatureId::Connection);
    QVERIFY(connection->supports(PanelId::Connection));
    QVERIFY(!connection->supports(PanelId::Login));
    QSignalSpy rejected(connection, &IBackend::completed);
    UiRequest foreign; foreign.id = 90; foreign.panel = PanelId::Login; foreign.action = "btn_Login";
    connection->execute(foreign);
    QCOMPARE(rejected.count(), 1); QVERIFY(!qvariant_cast<UiResponse>(rejected[0][0]).success);
    // Unimplemented adapters fail explicitly and never mutate connection state.
    auto *form = view.panelWidget(PanelId::Connection);
    form->findChild<QPushButton *>("pbtn_OpenSerial")->click();
    QTRY_VERIFY(form->property("presenterStatus").toString().contains(QString::fromUtf8("尚未实现")));
    std::atomic<QThread *> observed{nullptr}; Backend oldBackend(&observed), newBackend(&observed);
    QSignalSpy oldRequests(&oldBackend, &Backend::handled), newRequests(&newBackend, &Backend::handled);
    registry.setBackend(FeatureId::Connection, &oldBackend);
    form->findChild<QPushButton *>("pbtn_OpenNet")->click();
    registry.setBackend(FeatureId::Connection, &newBackend);
    form->findChild<QLineEdit *>("edit_LocalPort")->setText("2222");
    QTRY_COMPARE(oldRequests.count(), 1);
    QTest::qWait(20);
    QCOMPARE(form->findChild<QLineEdit *>("edit_LocalPort")->text(), QStringLiteral("2222"));
    form->findChild<QPushButton *>("pbtn_OpenNet")->click();
    QTRY_COMPARE(newRequests.count(), 1);
    QTRY_COMPARE(form->findChild<QLineEdit *>("edit_LocalPort")->text(), QStringLiteral("7777"));
}
void WorkbenchTests::presenterResponseMatching() {
    FakePanel view; view.input["edit_Username"] = "fixture";
    AccountPresenter presenter(PanelId::Login, &view);
    std::atomic<QThread *> observed{nullptr}; Backend backend(&observed); presenter.setBackend(&backend);
    view.cleared = 0;
    qRegisterMetaType<UiRequest>("fosd::ui::classic::UiRequest");
    QSignalSpy requests(&presenter, &PanelPresenter::requestReady);
    presenter.execute("btn_Login"); QCOMPARE(requests.count(), 1);
    const auto request = qvariant_cast<UiRequest>(requests[0][0]);
    QCOMPARE(request.fields, view.input);
    UiResponse response; response.id = request.id; response.panel = PanelId::Connection;
    response.success = true; response.fields["unexpected"] = true;
    presenter.complete(response); QVERIFY(view.output.isEmpty());
    response.panel = PanelId::Login; ++response.id;
    presenter.complete(response); QVERIFY(view.output.isEmpty());
    response.id = request.id; response.success = false; response.message = "rejected";
    presenter.complete(response); QVERIFY(view.output.isEmpty()); QCOMPARE(view.status, QStringLiteral("rejected"));
    QCOMPARE(view.cleared, 1);
    response.success = true; presenter.complete(response); QVERIFY(view.output.isEmpty()); // Duplicate response ignored.
    presenter.execute("btn_Login");
    response.id = qvariant_cast<UiRequest>(requests[1][0]).id;
    presenter.complete(response); QCOMPARE(view.output, response.fields);
    presenter.execute("btn_Login"); presenter.cancelPending();
    response.id = qvariant_cast<UiRequest>(requests[2][0]).id;
    view.output.clear(); presenter.complete(response); QVERIFY(view.output.isEmpty());
}
void WorkbenchTests::navigationAndIsolation() {
    WorkbenchView view;
    WorkbenchPresenter presenter(&view);
    QSignalSpy requests(&presenter, &WorkbenchPresenter::backendRequest);
    auto *pages = view.findChild<QStackedWidget *>("stackedWidget");
    QCOMPARE(pages->count(), 3);
    view.findChild<QPushButton *>("pbtn_3DPlot")->click(); QCOMPARE(pages->currentIndex(), 2);
    auto *modelDock = qobject_cast<QDockWidget *>(view.panelWidget(PanelId::ModelPanel)->parentWidget());
    QVERIFY(modelDock); modelDock->hide();
    view.panelWidget(PanelId::Visualization)->findChild<QAction *>("actionModelDock")->trigger();
    QVERIFY(!modelDock->isHidden());
    view.findChild<QPushButton *>("pbtn_JieTiaoYi")->click(); QCOMPARE(pages->currentIndex(), 0);
    auto *connection = view.panelWidget(PanelId::Connection);
    auto *login = view.panelWidget(PanelId::Login);
    login->findChild<QLineEdit *>("edit_Password")->setText("fixture-secret");
    QVERIFY(!view.panelView(PanelId::Main)->values().contains("edit_Password"));
    QVERIFY(!view.panelView(PanelId::Connection)->values().contains("edit_Password"));
    connection->findChild<QPushButton *>("pbtn_OpenSerial")->click();
    QCOMPARE(requests.count(), 0); // No backend: don't fabricate connection success.
    QVERIFY(view.property("presenterStatus").toString().contains(QString::fromUtf8("尚未接入")));
    login->findChild<QPushButton *>("btn_Register")->click();
    QVERIFY(!view.panelWidget(PanelId::Registration)->isHidden());
    login->findChild<QPushButton *>("btn_Clear")->click();
    QVERIFY(login->findChild<QLineEdit *>("edit_Password")->text().isEmpty());
    view.applyTheme(true); QVERIFY(!view.styleSheet().isEmpty());
    view.applyTheme(false); QVERIFY(!view.styleSheet().contains("/usr/share/"));
    view.show(); view.selectPage(2); QTest::qWait(60);
    QVERIFY(view.grab().save("workbench-migration.png"));
}
void WorkbenchTests::queuedBackendAndDisconnect() {
    WorkbenchView view;
    WorkbenchPresenter presenter(&view);
    std::atomic<QThread *> observed{nullptr};
    RunningThread thread;
    auto *backend = new Backend(&observed);
    backend->moveToThread(&thread);
    connect(&thread, &QThread::finished, backend, &QObject::deleteLater);
    WorkbenchBackend backends; backends.setBackend(FeatureId::Connection, backend);
    presenter.setBackends(&backends);
    QSignalSpy handled(backend, &Backend::handled);
    QSignalSpy requests(&presenter, &WorkbenchPresenter::backendRequest);
    thread.start();
    auto *form = view.panelWidget(PanelId::Connection);
    form->findChild<QLineEdit *>("edit_LocalPort")->setText("1234");
    form->findChild<QPushButton *>("pbtn_OpenNet")->click();
    QTRY_COMPARE(handled.count(), 1);
    QTRY_COMPARE(form->findChild<QLineEdit *>("edit_LocalPort")->text(), QStringLiteral("7777"));
    QCOMPARE(requests.count(), 1); // Applying presentation fields never sends another command.
    const auto request = qvariant_cast<UiRequest>(handled[0][0]);
    QCOMPARE(request.panel, PanelId::Connection); QCOMPARE(request.action, QStringLiteral("pbtn_OpenNet"));
    QCOMPARE(request.fields.value("edit_LocalPort").toString(), QStringLiteral("1234"));
    QCOMPARE(observed.load(), static_cast<QThread *>(&thread));
    backends.setBackend(FeatureId::Connection, nullptr);
    form->findChild<QPushButton *>("pbtn_OpenNet")->click(); QCOMPARE(requests.count(), 1);
    thread.quit(); QVERIFY(thread.wait(3000));
}
void WorkbenchTests::tablesAndRenderHost() {
    WorkbenchView view;
    QVariantMap table;
    table["headers"] = QStringList{"ID", "X", "Y"};
    QVariantList rows; rows.append(QVariant(QVariantList{"42", 1.25, -2.5})); table["rows"] = rows;
    QVariantMap fields; fields["tableWidget"] = table;
    view.panelView(PanelId::HardwareData)->applyFields(fields);
    auto *widget = view.panelWidget(PanelId::HardwareData)->findChild<QTableWidget *>("tableWidget");
    QCOMPARE(widget->rowCount(), 1); QCOMPARE(widget->item(0, 1)->text(), QStringLiteral("1.25"));
    QVariantMap graph; graph["x"] = QVariantList{1.0, 2.0}; graph["y"] = QVariantList{3.0, 4.0};
    fields.clear(); fields["cplot_Spectrum"] = graph;
    view.panelView(PanelId::Connection)->applyFields(fields);
    auto *plot = view.panelWidget(PanelId::Connection)->findChild<QCustomPlot *>("cplot_Spectrum");
    QCOMPARE(plot->graph(0)->dataCount(), 2);
    QVERIFY(view.renderHost()); QVERIFY(!view.renderHost()->surface());
    auto *surface = new QWidget;
    view.renderHost()->setSurface(surface); QCOMPARE(surface->parentWidget(), view.renderHost());
    view.renderHost()->setSurface(nullptr); QVERIFY(!surface->parentWidget()); delete surface;
    surface = new QWidget; view.renderHost()->setSurface(surface); delete surface;
    QVERIFY(!view.renderHost()->surface());
}
QTEST_MAIN(WorkbenchTests)
#include "WorkbenchTests.moc"
