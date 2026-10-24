#include "../presenters/VtkValidationPresenter.h"
#include <QtTest>
using namespace fosd::vtk_test;
class FakeView final : public IVtkValidationView {
public:
    vtkRenderWindow *window = reinterpret_cast<vtkRenderWindow *>(quintptr(0x1234));
    vtkRenderWindow *received = nullptr;
    bool validContext = true, bound = false;
    QString status;
    QString file;
    QString chooseFile(bool) override { return file; }
    vtkRenderWindow *renderWindow() const override { return window; }
    bool contextReady() const override { return validContext; }
    void showBinding(bool value, vtkRenderWindow *pointer) override { bound = value; received = pointer; }
    void showStatus(const QString &message) override { status = message; }
};
class FakeBackend final : public IVtkValidationBackend {
public:
    vtkRenderWindow *window = nullptr;
    bool fail = false, mismatch = false;
    int renders = 0, demos = 0, clears = 0, resets = 0, attaches = 0;
    QString action;
    QVariantMap parameters;
    bool execute(const QString &value, const QVariantMap &fields, QString *) override {
        action = value; parameters = fields; return true;
    }
    bool attach(vtkRenderWindow *pointer, QString *error) override {
        ++attaches;
        if (fail) { *error = "injected attach failure"; return false; }
        window = mismatch ? nullptr : pointer; return true;
    }
    void detach() override { window = nullptr; }
    vtkRenderWindow *boundWindow() const override { return window; }
    bool showDemo(QString *) override { ++demos; return true; }
    bool clearDemo(QString *) override { ++clears; return true; }
    bool resetCamera(QString *) override { ++resets; return true; }
    bool render(QString *) override { ++renders; return true; }
};
class PresenterTests final : public QObject {
    Q_OBJECT
private slots:
    void forwardsIntegrationData() {
        FakeView view; FakeBackend backend; VtkValidationPresenter presenter(&view, &backend);
        presenter.execute("openButton", {{"port", 9123}, {"transport", 0}});
        QCOMPARE(backend.action, QStringLiteral("openButton")); QCOMPARE(backend.parameters["port"].toInt(), 9123);
        presenter.attach();
        presenter.execute("modelButton", {}); // Cancelled file selection sends no command.
        QCOMPARE(backend.action, QStringLiteral("openButton"));
        view.file = "fixture.stl"; presenter.execute("modelButton", {});
        QCOMPARE(backend.parameters["path"].toString(), view.file);
        presenter.showStatus("backend result ready"); QCOMPARE(view.status, QStringLiteral("backend result ready"));
    }
    void pointerAndCommands() {
        FakeView view; FakeBackend backend;
        {
            VtkValidationPresenter presenter(&view, &backend);
            presenter.attach(); QVERIFY(view.bound); QCOMPARE(backend.window, view.window); QCOMPARE(view.received, view.window);
            presenter.showDemo(); presenter.clearDemo(); presenter.resetCamera(); presenter.render();
            QCOMPARE(backend.demos, 1); QCOMPARE(backend.clears, 1); QCOMPARE(backend.resets, 1); QCOMPARE(backend.renders, 1);
            view.validContext = false; presenter.render(); QCOMPARE(backend.renders, 1);
            presenter.detach(); QVERIFY(!view.bound); QVERIFY(!backend.window);
            view.validContext = true; presenter.showDemo(); QCOMPARE(backend.demos, 1);
            presenter.attach(); QVERIFY(view.bound);
        }
        QVERIFY(!backend.window); // Destruction detaches before the View dies.
    }
    void failures() {
        FakeView view; FakeBackend backend; VtkValidationPresenter presenter(&view, &backend);
        view.window = nullptr; presenter.attach(); QCOMPARE(backend.attaches, 0); QVERIFY(!view.bound);
        view.window = reinterpret_cast<vtkRenderWindow *>(quintptr(0x1234));
        backend.fail = true; presenter.attach(); QVERIFY(!view.bound); QCOMPARE(view.status, QStringLiteral("injected attach failure"));
        backend.fail = false; backend.mismatch = true; presenter.attach(); QVERIFY(!view.bound); QVERIFY(!backend.window);
    }
};
QTEST_GUILESS_MAIN(PresenterTests)
#include "PresenterTests.moc"
