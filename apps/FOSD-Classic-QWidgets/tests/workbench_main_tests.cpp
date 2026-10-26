#include "ClassicWorkbench.h"
#include "backend/WorkbenchRuntime.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include "workbench_test_packets.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QSurfaceFormat>
#include <QTableView>
#include <QTemporaryDir>
#include <QFile>
#include <QUdpSocket>
#include <QVTKOpenGLNativeWidget.h>
#include <QtTest>
#include <vtkAutoInit.h>
#include <vtkRendererCollection.h>
#include <cmath>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
using namespace fosd;
using namespace fosd::ui::classic;
class TestWindow : public ClassicWorkbench {
public:
    QString outputPath;
    QString chooseFile(bool, const QString &, const QString &) override {
        return outputPath;
    }
};
class WorkbenchMainTests : public QObject {
    Q_OBJECT
private slots:
    void completeUiPipeline();
};
static bool ready(const QSignalSpy &states) {
    for (const auto &state : states)
        if (qvariant_cast<communication::Channel>(state[0]) == communication::Channel::Udp &&
            qvariant_cast<communication::ConnectionState>(state[1]) ==
                communication::ConnectionState::Ready)
            return true;
    return false;
}

void WorkbenchMainTests::completeUiPipeline() {
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    TestWindow window;
    window.outputPath = directory.filePath("full-chain.csv");
    window.show();
    window.selectPage(2);
    QVERIFY(QTest::qWaitForWindowExposed(&window));
    auto *surface = window.renderHost()->surface()->findChild<QVTKOpenGLNativeWidget *>();
    QVERIFY(surface);
    QCOMPARE(window.runtime().visualization().renderWindow(), surface->GetRenderWindow());
    QVERIFY(surface->GetRenderWindow()->GetRenderers()->GetNumberOfItems() > 0);
    QSignalSpy configured(&window.runtime().calculation(),
                          &Calculation::CalculationControl::configurationAccepted);
    QSignalSpy accepted(&window.runtime().calculation(),
                        &Calculation::CalculationControl::modelAccepted);
    QSignalSpy result(&window.runtime().calculation(),
                      &Calculation::CalculationControl::resultReady);
    QSignalSpy started(&window.runtime().calculation().storage(),
                       &storage::StorageControl::started);
    QSignalSpy stopped(&window.runtime().calculation().storage(),
                       &storage::StorageControl::stopped);
    QSignalSpy batches(&window.runtime().calculation().storage(),
                       &storage::StorageControl::batchWritten);
    auto *visualization = window.panelWidget(PanelId::Visualization);
    auto *plate = visualization->findChild<QAction *>("actionPlateValidation");
    QVERIFY(plate);
    plate->trigger();
    QTRY_COMPARE(configured.size(), 1);
    QTRY_COMPARE(accepted.size(), 1);
    auto *connection = window.panelWidget(PanelId::Connection);
    QUdpSocket reserve;
    QVERIFY(reserve.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 port = reserve.localPort();
    reserve.close();
    window.selectPage(0);
    connection->findChild<QComboBox *>("cbox_CommuteChoose")->setCurrentIndex(0);
    connection->findChild<QComboBox *>("cbox_LocakIP")->setCurrentText("127.0.0.1");
    connection->findChild<QLineEdit *>("edit_LocalPort")->setText(QString::number(port));
    connection->findChild<QLineEdit *>("edit_TargetIP")->setText("127.0.0.1");
    connection->findChild<QLineEdit *>("edit_TargetPort")->setText("5073");
    QSignalSpy states(&window.runtime().protocol().communication(),
                      &communication::ConnectControl::stateChanged);
    connection->findChild<QPushButton *>("pbtn_OpenNet")->click();
    QTRY_VERIFY(ready(states));
    auto *save = connection->findChild<QPushButton *>("pbtn_SaveDynamic");
    QVERIFY(save->isCheckable());
    save->click();
    QTRY_COMPARE(started.size(), 1);
    QUdpSocket sender;
    const auto bytes = workbench_test::packet(19);
    QCOMPARE(sender.writeDatagram(bytes, QHostAddress::LocalHost, port), qint64(bytes.size()));
    QTRY_VERIFY(result.size() >= 1);
    QTRY_VERIFY(batches.size() >= 3);
    auto *wavelengths = connection->findChild<QTableView *>("tablev_Wavelength");
    QVERIFY(wavelengths);
    QTRY_VERIFY(wavelengths->model() && wavelengths->model()->rowCount() == 12);
    const auto solved = window.runtime().calculation().dataModel()->latest();
    QVERIFY(solved);
    QCOMPARE(solved->algorithmId, QStringLiteral("demo.plate.modal-ife"));
    QCOMPARE(solved->sourceFrame->source.transport, core::TransportKind::Udp);
    window.selectPage(2);
    // Select a point with nonzero Z displacement and verify actual display follows its result.
    core::PointId selected = -1;
    for (auto it = solved->displacementMap.constBegin(); it != solved->displacementMap.constEnd();
         ++it)
        if (std::abs(it.value()[2]) > 10) {
            selected = it.key();
            break;
        }
    QVERIFY(selected >= 0);
    window.runtime().visualization().selectPoint(vtkIdType(selected));
    QTRY_VERIFY(std::abs(window.runtime().visualization().selectedPoint().displacementMm[2]) > 10);
    auto *scene = window.panelWidget(PanelId::ScenePanel);
    auto *axes = scene->findChild<QCheckBox *>("cbox_AxisAble");
    QVERIFY(axes);
    axes->click();
    QTRY_COMPARE(window.runtime().visualization().axesSettings().visible, axes->isChecked());
    auto *model = window.panelWidget(PanelId::ModelPanel);
    model->findChild<QPushButton *>("pbtn_ModelMerge")->click();
    QTRY_VERIFY(
        model->property("presenterStatus").toString().contains(QString::fromUtf8("尚未实现")));
    window.selectPage(0);
    save->click();
    QTRY_COMPARE(stopped.size(), 1);
    QFile file(window.outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    const auto contents = file.readAll();
    QVERIFY(contents.contains("wavelength"));
    QVERIFY(contents.contains("point_displacement"));
    connection->findChild<QPushButton *>("pbtn_OpenNet")->click();
}

int main(int argc, char **argv) {
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication app(argc, argv);
    WorkbenchMainTests tests;
    return QTest::qExec(&tests, argc, argv);
}
#include "workbench_main_tests.moc"
