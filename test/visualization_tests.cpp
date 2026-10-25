#include "visualization/VisualizationControl.h"
#include "visualization/render/ScalarColorMapper.h"
#include "visualization/render/DeformationRenderer.h"
#include <QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QSignalSpy>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkScalarsToColors.h>
#include <vtkAutoInit.h>
#include <cmath>
#include <limits>
using namespace fosd;
using namespace fosd::visualization;
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
namespace {
vtkSmartPointer<vtkPolyData> mesh() {
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(100, 0, 0);
    points->InsertNextPoint(700, 50, 0);
    points->InsertNextPoint(1300, 0, 0);
    auto data = vtkSmartPointer<vtkPolyData>::New();
    data->SetPoints(points);
    return data;
}

core::CalculationResultPtr result() {
    core::CalculationResult r;
    r.revision = 1;
    r.modelRevision = 7;
    r.displacementMap.insert(0, {-2, 3, 4});
    r.displacementMap.insert(1, {0, -4, 0});
    r.displacementMap.insert(2, {2, 0, -4});
    return std::make_shared<const core::CalculationResult>(r);
}

vtkDoubleArray *colors(vtkPolyData *data) {
    return vtkDoubleArray::SafeDownCast(data->GetPointData()->GetArray("FOSD.ColorScalars"));
}
} // namespace
class VisualizationTests final : public QObject {
    Q_OBJECT
private slots:
    void componentsAndNormalization() {
        ScalarColorMapper coloring;
        ScalarColorSettings settings;
        settings.enabled = true;
        const auto data = mesh();
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(data);
        const auto frame = result();
        QString error;
        for (int axis = 0; axis < 3; ++axis) {
            settings.field = ScalarField(axis);
            QVERIFY(coloring.setSettings(settings));
            QVERIFY2(coloring.applyResult(frame, data, mapper, &error), qPrintable(error));
            double minimum = 1e100, maximum = -1e100;
            for (int i = 0; i < 3; ++i) {
                minimum = qMin(minimum, frame->displacementMap.value(i)[axis]);
                maximum = qMax(maximum, frame->displacementMap.value(i)[axis]);
            }
            for (int i = 0; i < 3; ++i)
                QCOMPARE(colors(data)->GetValue(i),
                         (frame->displacementMap.value(i)[axis] - minimum) / (maximum - minimum));
            QCOMPARE(coloring.range().lookupMinimum, 0.0);
            QCOMPARE(coloring.range().lookupMaximum, 1.0);
        }
        settings.field = ScalarField::Magnitude;
        settings.mappingMode = ScalarMappingMode::Physical;
        settings.rangeMode = ColorRangeMode::Automatic;
        QVERIFY(coloring.setSettings(settings));
        QVERIFY(coloring.applyResult(frame, data, mapper));
        QCOMPARE(colors(data)->GetValue(0), std::sqrt(29.0));
        QCOMPARE(colors(data)->GetValue(1), 4.0);
        QCOMPARE(colors(data)->GetValue(2), std::sqrt(20.0));
        QCOMPARE(frame->displacementMap.value(0)[0], -2.0);
        QCOMPARE(data->GetPoint(0)[0], 100.0); // Coloring never deforms geometry.
    }
    void physicalAbsoluteAndManualRange() {
        ScalarColorMapper coloring;
        ScalarColorSettings s;
        s.enabled = true;
        s.field = ScalarField::DisplacementX;
        s.mappingMode = ScalarMappingMode::Physical;
        s.rangeMode = ColorRangeMode::Automatic;
        auto data = mesh();
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(data);
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyResult(result(), data, mapper));
        QCOMPARE(colors(data)->GetValue(0), -2.0);
        QCOMPARE(coloring.range().lookupMinimum, -2.0);
        s.valueMode = ScalarValueMode::Absolute;
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyResult(result(), data, mapper));
        QCOMPARE(colors(data)->GetValue(0), 2.0);
        QCOMPARE(coloring.range().dataMinimum, 0.0);
        QCOMPARE(coloring.range().dataMaximum, 2.0);
        s.rangeMode = ColorRangeMode::Manual;
        s.minimum = 0;
        s.maximum = 10;
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyResult(result(), data, mapper));
        QCOMPARE(mapper->GetScalarRange()[1], 10.0);
        QCOMPARE(mapper->GetLookupTable()->GetRange()[1], 10.0);
        double low[3], high[3];
        mapper->GetLookupTable()->GetColor(0, low);
        mapper->GetLookupTable()->GetColor(10, high);
        QVERIFY(low[2] > low[0]);
        QVERIFY(high[0] > high[2]);
        s.mappingMode = ScalarMappingMode::Normalized;
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyResult(result(), data, mapper));
        QCOMPARE(colors(data)->GetValue(0), 1.0);
        QCOMPARE(colors(data)->GetValue(1), 0.0);
        QCOMPARE(coloring.range().lookupMaximum, 1.0);
        s.enabled = false;
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyResult(result(), data, mapper));
        QVERIFY(!mapper->GetScalarVisibility());
        QVERIFY(!colors(data));
    }
    void genericFieldsAndDegenerateData() {
        ScalarColorMapper coloring;
        ScalarColorSettings s;
        s.enabled = true;
        auto data = mesh();
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(data);
        PointScalarField field;
        field.id = "strain.xx";
        field.unit = "microstrain";
        field.values = {{0, -5}, {1, -5}, {2, -5}};
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyField(field, data, mapper));
        for (int i = 0; i < 3; ++i)
            QCOMPARE(colors(data)->GetValue(i), 0.0);
        s.mappingMode = ScalarMappingMode::Physical;
        s.rangeMode = ColorRangeMode::Automatic;
        QVERIFY(coloring.setSettings(s));
        QVERIFY(coloring.applyField(field, data, mapper));
        QVERIFY(coloring.range().lookupMaximum > coloring.range().lookupMinimum);
        s.mappingMode = ScalarMappingMode::Normalized;
        QVERIFY(coloring.setSettings(s));
        field.values = {{0, -1e308}, {1, 0}, {2, 1e308}};
        QVERIFY(coloring.applyField(field, data, mapper));
        QCOMPARE(colors(data)->GetValue(0), 0.0);
        QCOMPARE(colors(data)->GetValue(1), 0.5);
        QCOMPARE(colors(data)->GetValue(2), 1.0);
        field.values = {{0, 0}, {1, 0}, {2, 0}};
        QVERIFY(coloring.applyField(field, data, mapper));
        QCOMPARE(colors(data)->GetValue(2), 0.0);
    }
    void invalidInputAndSettings() {
        ScalarColorMapper coloring;
        ScalarColorSettings s;
        s.enabled = true;
        QVERIFY(coloring.setSettings(s));
        s.maximum = s.minimum;
        QString error;
        QVERIFY(!coloring.setSettings(s, &error));
        QVERIFY(!error.isEmpty());
        QCOMPARE(coloring.settings().maximum, 50.0);
        s = coloring.settings();
        s.hueStart = -0.1;
        QVERIFY(!coloring.setSettings(s));
        auto data = mesh();
        auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData(data);
        PointScalarField field;
        field.id = "temperature";
        field.values = {{0, 1}, {1, 2}, {42, 3}};
        QVERIFY(!coloring.applyField(field, data, mapper));
        QVERIFY(!mapper->GetScalarVisibility());
        field.values = {{0, 1}, {1, 2}, {2, std::numeric_limits<double>::quiet_NaN()}};
        QVERIFY(!coloring.applyField(field, data, mapper));
        QVERIFY(!colors(data));
        QVERIFY(!coloring.applyResult({}, data, mapper));
    }
    void controlSwitchesWithoutNewFrame() {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        QFile file(directory.filePath("model.stl"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("solid test\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 "
                   "0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid test\n");
        file.close();
        VisualizationControl control;
        QSignalSpy errors(&control, &VisualizationControl::errorOccurred);
        QVERIFY(control.loadModel(file.fileName()));
        control.setExpectedModelRevision(7);
        control.setScalarColoringEnabled(true);
        control.setScalarField(ScalarField::DisplacementX);
        control.applyResult(result());
        QCOMPARE(control.scalarColorRange().dataMinimum, -2.0);
        control.setScalarField(ScalarField::DisplacementY);
        QCOMPARE(control.scalarColorRange().dataMinimum, -4.0);
        control.setDeformationScale(100);
        QCOMPARE(control.scalarColorRange().dataMinimum, -4.0);
        control.setScalarMappingMode(ScalarMappingMode::Physical);
        control.setColorRange(-10, 10);
        QCOMPARE(control.scalarColorRange().lookupMinimum, -10.0);
        auto invalid = control.scalarColorSettings();
        invalid.maximum = -100;
        QVERIFY(!control.setScalarColorSettings(invalid));
        QCOMPARE(control.scalarColorSettings().maximum, 10.0);
        PointScalarField field;
        field.id = "strain.xx";
        field.modelRevision = 6;
        field.values = {{0, 1}, {1, 2}, {2, 3}};
        QVERIFY(!control.setPointScalarField(field));
        field.modelRevision = 7;
        QVERIFY(control.setPointScalarField(field));
        QCOMPARE(control.scalarColorSettings().field, ScalarField::Custom);
        QCOMPARE(control.scalarColorRange().dataMaximum, 3.0);
        control.resetDeformation();
        QCOMPARE(control.scalarColorRange().dataMaximum, 0.0);
        control.unloadModel();
        QVERIFY(!control.originalModel());
        QVERIFY(control.loadModel(file.fileName()));
        control.setExpectedModelRevision(8);
        control.applyResult(result());
        QCOMPARE(control.scalarColorRange().dataMaximum, 0.0); // Stale model revision rejected.
        QVERIFY(errors.count() >= 2);
    }
};
QTEST_GUILESS_MAIN(VisualizationTests)
#include "visualization_tests.moc"
