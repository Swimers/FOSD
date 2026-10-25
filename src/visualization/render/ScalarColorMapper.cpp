#include "ScalarColorMapper.h"
#include <vtkDoubleArray.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <algorithm>
#include <cmath>
#include <limits>

namespace fosd {
namespace visualization {
namespace {
const char *rawName = "FOSD.RawScalars";
const char *mappedName = "FOSD.ColorScalars";
bool fail(QString *error, const QString &message) {
    if (error)
        *error = message;
    return false;
}

bool validField(ScalarField field) {
    return field == ScalarField::DisplacementX || field == ScalarField::DisplacementY ||
           field == ScalarField::DisplacementZ || field == ScalarField::Magnitude ||
           field == ScalarField::Custom;
}
} // namespace
ScalarColorMapper::ScalarColorMapper() : m_lookupTable(vtkSmartPointer<vtkLookupTable>::New()) {
}

ScalarColorMapper::~ScalarColorMapper() = default;
bool ScalarColorMapper::setSettings(const ScalarColorSettings &settings, QString *error) {
    if (error)
        error->clear();
    if (!validField(settings.field) ||
        (settings.mappingMode != ScalarMappingMode::Normalized &&
         settings.mappingMode != ScalarMappingMode::Physical) ||
        (settings.valueMode != ScalarValueMode::Signed &&
         settings.valueMode != ScalarValueMode::Absolute) ||
        (settings.rangeMode != ColorRangeMode::Automatic &&
         settings.rangeMode != ColorRangeMode::Manual))
        return fail(error, QStringLiteral("Unknown scalar mapping mode or field"));
    if (!std::isfinite(settings.minimum) || !std::isfinite(settings.maximum) ||
        settings.minimum >= settings.maximum)
        return fail(error, QStringLiteral("Color range requires finite minimum < maximum"));
    if (!std::isfinite(settings.hueStart) || !std::isfinite(settings.hueEnd) ||
        settings.hueStart < 0 || settings.hueStart > 1 || settings.hueEnd < 0 ||
        settings.hueEnd > 1)
        return fail(error, QStringLiteral("Hue endpoints must be in [0, 1]"));
    m_settings = settings;
    return true;
}

void ScalarColorMapper::setScalarField(ScalarField field) {
    auto settings = m_settings;
    settings.field = field;
    setSettings(settings);
}

void ScalarColorMapper::setColorRangeMode(ColorRangeMode mode) {
    auto settings = m_settings;
    settings.rangeMode = mode;
    setSettings(settings);
}

void ScalarColorMapper::setColorRange(double minimum, double maximum) {
    auto settings = m_settings;
    settings.minimum = minimum;
    settings.maximum = maximum;
    setSettings(settings);
}

bool ScalarColorMapper::applyResult(const core::CalculationResultPtr &result, vtkPolyData *mesh,
                                    vtkPolyDataMapper *mapper, QString *error) {
    if (!m_settings.enabled) {
        clear(mesh, mapper);
        if (error)
            error->clear();
        return true;
    }
    if (!result || m_settings.field == ScalarField::Custom) {
        clear(mesh, mapper);
        return fail(error, QStringLiteral("No displacement field available"));
    }
    PointScalarField field;
    field.id = QStringLiteral("displacement");
    field.unit = QStringLiteral("mm");
    field.modelRevision = result->modelRevision;
    for (auto point = result->displacementMap.constBegin();
         point != result->displacementMap.constEnd(); ++point) {
        const auto &d = point.value();
        if (d.size() != 3 || !std::isfinite(d[0]) || !std::isfinite(d[1]) || !std::isfinite(d[2])) {
            clear(mesh, mapper);
            return fail(error, QStringLiteral("Invalid displacement at point %1").arg(point.key()));
        }
        double value = 0;
        switch (m_settings.field) {
            case ScalarField::DisplacementX:
                value = d[0];
                break;
            case ScalarField::DisplacementY:
                value = d[1];
                break;
            case ScalarField::DisplacementZ:
                value = d[2];
                break;
            case ScalarField::Magnitude:
                value = std::hypot(std::hypot(d[0], d[1]), d[2]);
                break;
            case ScalarField::Custom:
                break;
        }
        field.values.insert(field.values.constEnd(), point.key(), value);
    }
    return applyField(field, mesh, mapper, error);
}

bool ScalarColorMapper::applyField(const PointScalarField &field, vtkPolyData *mesh,
                                   vtkPolyDataMapper *mapper, QString *error) {
    if (error)
        error->clear();
    if (!m_settings.enabled) {
        clear(mesh, mapper);
        return true;
    }
    if (!mesh || !mapper || !mesh->GetNumberOfPoints() || field.id.isEmpty() ||
        field.values.size() != mesh->GetNumberOfPoints()) {
        clear(mesh, mapper);
        return fail(error, QStringLiteral("Scalar field must cover every model point"));
    }
    auto raw = vtkSmartPointer<vtkDoubleArray>::New();
    raw->SetName(rawName);
    auto mapped = vtkSmartPointer<vtkDoubleArray>::New();
    mapped->SetName(mappedName);
    const vtkIdType count = mesh->GetNumberOfPoints();
    raw->SetNumberOfValues(count);
    mapped->SetNumberOfValues(count);
    double minimum = std::numeric_limits<double>::max(),
           maximum = std::numeric_limits<double>::lowest();
    auto value = field.values.constBegin();
    for (vtkIdType id = 0; id < count; ++id, ++value) {
        if (value == field.values.constEnd() || value.key() != id ||
            !std::isfinite(value.value())) {
            clear(mesh, mapper);
            return fail(
                error,
                QStringLiteral("Missing or nonfinite scalar at point %1").arg(qlonglong(id)));
        }
        raw->SetValue(id, value.value());
        const double transformed = m_settings.valueMode == ScalarValueMode::Absolute
                                       ? std::abs(value.value())
                                       : value.value();
        mapped->SetValue(id, transformed);
        minimum = std::min(minimum, transformed);
        maximum = std::max(maximum, transformed);
    }
    ScalarColorRange range;
    range.dataMinimum = minimum;
    range.dataMaximum = maximum;
    if (m_settings.mappingMode == ScalarMappingMode::Normalized) {
        const double span = maximum - minimum;
        for (vtkIdType id = 0; id < count; ++id) {
            const double value = mapped->GetValue(id);
            // Match legacy min/max normalization, including constant fields.
            const double normalized = span < 1e-12 ? 0.0
                                                   : std::isfinite(span)
                                                         ? (value - minimum) / span
                                                         : (value * 0.5 - minimum * 0.5) /
                                                               (maximum * 0.5 - minimum * 0.5);
            mapped->SetValue(id, normalized);
        }
        range.lookupMinimum = 0;
        range.lookupMaximum = 1;
    } else if (m_settings.rangeMode == ColorRangeMode::Manual) {
        range.lookupMinimum = m_settings.minimum;
        range.lookupMaximum = m_settings.maximum;
    } else {
        range.lookupMinimum = minimum;
        range.lookupMaximum = maximum;
        if (minimum == maximum) {
            const double padding = std::max(1.0, std::abs(minimum) * 1e-6);
            if (std::isfinite(maximum + padding))
                range.lookupMaximum += padding;
            else
                range.lookupMinimum -= padding;
        }
    }
    m_lookupTable->SetNumberOfTableValues(256);
    m_lookupTable->SetHueRange(m_settings.hueStart, m_settings.hueEnd);
    m_lookupTable->SetTableRange(range.lookupMinimum, range.lookupMaximum);
    m_lookupTable->Build();
    mesh->GetPointData()->RemoveArray(rawName);
    mesh->GetPointData()->RemoveArray(mappedName);
    mesh->GetPointData()->AddArray(raw);
    mesh->GetPointData()->AddArray(mapped);
    mapper->SetLookupTable(m_lookupTable);
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray(mappedName);
    mapper->SetColorModeToMapScalars();
    mapper->InterpolateScalarsBeforeMappingOn();
    mapper->SetScalarRange(range.lookupMinimum, range.lookupMaximum);
    mapper->UseLookupTableScalarRangeOn();
    mapper->ScalarVisibilityOn();
    m_rawScalars = raw;
    m_scalars = mapped;
    m_range = range;
    mesh->Modified();
    mapper->Modified();
    return true;
}

void ScalarColorMapper::clear(vtkPolyData *mesh, vtkPolyDataMapper *mapper) {
    if (mapper)
        mapper->ScalarVisibilityOff();
    if (mesh) {
        mesh->GetPointData()->RemoveArray(rawName);
        mesh->GetPointData()->RemoveArray(mappedName);
        mesh->Modified();
    }
    m_scalars = nullptr;
    m_rawScalars = nullptr;
    m_range = ScalarColorRange();
}
} // namespace visualization
} // namespace fosd
