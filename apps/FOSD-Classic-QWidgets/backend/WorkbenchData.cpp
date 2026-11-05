#include "WorkbenchRuntime.h"
#include <cmath>
namespace fosd {
namespace application {
using namespace ui::classic;
namespace {
QVariant table(const QStringList &headers, const QVariantList &rows) {
    return QVariantMap{{"headers", headers}, {"rows", rows}};
}

QString xyz(const std::array<double, 3> &p) {
    return QStringLiteral("%1, %2, %3")
        .arg(p[0], 0, 'g', 8)
        .arg(p[1], 0, 'g', 8)
        .arg(p[2], 0, 'g', 8);
}
} // namespace
bool WorkbenchRuntime::dataAction(const UiRequest &request, UiResponse *response) {
    const auto &f = request.fields;
    const auto &action = request.action;
    response->success = true;
    response->message = tr("设置已应用");
    if (request.panel == PanelId::Plot && action == "pbtn_Observe") {
        m_observing = !m_observing;
        m_presentedResult = 0;
        response->fields[action] = m_observing ? tr("暂停观测") : tr("开始观测");
    } else if (request.panel == PanelId::PlotSettings && action == "pbtn_SenerSet") {
        auto configuration = m_configuration;
        configuration.channelCalibration.channelIndices.clear();
        configuration.channelCalibration.calibrationFactors.clear();
        for (int i = 1; i <= 12; ++i) {
            bool indexOk = false, factorOk = false;
            const int source =
                text(f, QStringLiteral("edit_Sensor%1Order").arg(i)).toInt(&indexOk) - 1;
            const double factor =
                text(f, QStringLiteral("edit_Sensor%1Para").arg(i)).toDouble(&factorOk);
            if (!indexOk || !factorOk) {
                response->success = false;
                response->message = tr("传感器序号和系数必须为有效数值");
                return true;
            }
            configuration.channelCalibration.channelIndices.push_back(source);
            configuration.channelCalibration.calibrationFactors.push_back(factor);
        }
        response->success = configure(configuration, &response->message);
    } else if (request.panel == PanelId::HardwareData && action == "checkBox") {
        m_hardwareRefresh = f.value(action).toBool();
        m_presentedHardware.reset();
    } else
        return false;
    return true;
}

void WorkbenchRuntime::publishCalibration() {
    emit presentationChanged(
        PanelId::Connection,
        {{"textEdit_APara", QString::number(m_configuration.strainCalibration.A, 'g', 12)},
         {"textEdit_BPara", QString::number(m_configuration.strainCalibration.B, 'g', 12)}});
    QVariantMap fields;
    for (int i = 0; i < 12; ++i) {
        const auto &channels = m_configuration.channelCalibration;
        fields[QStringLiteral("edit_Sensor%1Order").arg(i + 1)] = QString::number(
            i < int(channels.channelIndices.size()) ? channels.channelIndices[size_t(i)] + 1
                                                    : i + 1);
        fields[QStringLiteral("edit_Sensor%1Para").arg(i + 1)] = QString::number(
            i < int(channels.calibrationFactors.size()) ? channels.calibrationFactors[size_t(i)]
                                                        : 1.0,
            'g', 12);
    }
    emit presentationChanged(PanelId::PlotSettings, fields);
}

void WorkbenchRuntime::publishModel() {
    const auto model = m_visualization->originalModel();
    const auto points = m_visualization->keyPoints();
    QVariantList rows;
    QStringList ids;
    if (model)
        for (auto it = model->constBegin(); it != model->constEnd(); ++it)
            ids.append(QString::number(it.key()));
    for (const auto &point : points) {
        QVector<double> coordinates = model ? model->value(point.pointId) : QVector<double>();
        if (!m_originalKeyCoordinates)
            coordinates =
                m_result ? m_result->displacementMap.value(point.pointId) : QVector<double>(3, 0.0);
        if (coordinates.size() == 3)
            rows.append(QVariant(QVariantList{qlonglong(point.pointId), coordinates[0],
                                              coordinates[1], coordinates[2]}));
    }
    const auto count = QString::number(model ? model->size() : 0),
               keys = QString::number(points.size());
    const QStringList headers = m_originalKeyCoordinates
                                    ? QStringList{"ID", "X0 (mm)", "Y0 (mm)", "Z0 (mm)"}
                                    : QStringList{"ID", "dX (mm)", "dY (mm)", "dZ (mm)"};
    emit presentationChanged(PanelId::KeyPointSelect,
                             {{"tablew_PointPara", table(headers, rows)},
                              {"cbox_KeyPointID", QVariantMap{{"items", ids}}},
                              {"lab_NumPoints", count},
                              {"lab_NumKeyPoints", keys}});
    emit presentationChanged(PanelId::KeyPointEdit,
                             {{"cbox_IDKeyPoint", QVariantMap{{"items", ids}}},
                              {"lab_NumPoint", count},
                              {"lab_NumKeyPoint", keys}});
    emit presentationChanged(
        PanelId::InfoPanel,
        {{"lab_NumPoints", count},
         {"lab_NumKeyPoints", keys},
         {"lab_NumPolys", QString::number(m_visualization->modelPolygonCount())}});
    if (!m_result) {
        emit presentationChanged(PanelId::InfoPanel,
                                 {{"lab_MaxDeformation", QStringLiteral("--")}});
        emit presentationChanged(
            PanelId::Plot,
            {{"table_Disp2DPara",
              table({"ID", "X0", "Y0", "Z0", "dX", "dY", "dZ", "X", "Y", "Z"}, {})}});
    }
}

void WorkbenchRuntime::publishLatest() {
    if (m_baselineClock.isValid() && m_baselineClock.elapsed() > m_baselineTimeout) { m_baselineClock.invalidate(); notice(tr("置零失败：采样超时")); }
    QVariantMap raw;
    if (m_udpLogPending) {
        m_udpLogPending = false;
        const auto entry = udpLogEntry();
        m_udpLogLines.append(entry);
        while (m_udpLogLines.size() > 200)
            m_udpLogLines.removeFirst();
        raw["ptedit_UdpRec"] = QVariantMap{{"append", entry}};
    }
    const char *names[] = {"ptedit_SerialRec", "ptedit_UdpRec", "ptedit_TcpRec"};
    for (int i = 0; i < 3; ++i) {
        if (!m_rawDirty[i])
            continue;
        m_rawDirty[i] = false;
        const auto display = rawDisplay(i);
        if (display != m_presentedRaw[i]) {
            raw[names[i]] = display;
            m_presentedRaw[i] = display;
        }
    }
    if (!raw.isEmpty())
        emit presentationChanged(PanelId::Connection, raw);
    if (m_frame && m_presentedFrame != m_frame->revision) {
        const auto frame = std::dynamic_pointer_cast<const core::WavelengthFrame>(m_frame->data);
        if (frame) {
            QVariantList rows;
            for (const auto &record:frame->records) {
                const auto key=QString("CH%1_%2").arg(record.channel).arg(record.sensor);
                auto &samples=m_history[key]; samples.append(QVariant(QVariantList{m_historyClock.elapsed()/1000.0, record.wavelength}));
                while(samples.size()>300) samples.removeFirst();
            }
            QVariantList series;
            for(auto it=m_history.constBegin();it!=m_history.constEnd();++it) {
                int ch=it.key().mid(2).section('_',0,0).toInt();
                if(!m_filterChannels || m_channels.contains(ch)) series.append(QVariantMap{{"name",it.key()},{"samples",it.value()}});
            }
            emit presentationChanged(PanelId::Connection,{{"cplot_Spectrum",QVariantMap{{"series",series}}}});
            for (const auto &record : frame->records) {
                if (m_filterChannels && !m_channels.contains(record.channel))
                    continue;
                rows.append(QVariant(QVariantList{record.channel, record.sensor,
                                                  QString::number(record.wavelength, 'f', 6)}));
                if (rows.size() >= 2048)
                    break;
            }
            emit presentationChanged(
                PanelId::Connection,
                {{"tablev_Wavelength", table({tr("通道"), tr("传感器"), tr("波长 (nm)")}, rows)}});
        }
        m_presentedFrame = m_frame->revision;
    }
    if (m_hardwareRefresh && m_hardware && m_hardware->sourceFrame &&
        m_presentedHardware != m_hardware) {
        QVariantList rows;
        for (int i = 0; i < m_hardware->selectedStrains.size(); ++i)
            rows.append(QVariant(QVariantList{i + 1, m_hardware->sourceIndices.value(i) + 1,
                                              m_hardware->selectedStrains[i]}));
        emit presentationChanged(
            PanelId::HardwareData,
            {{"tableWidget", table({tr("IFE 输入"), tr("波长源序号"), tr("标定应变")}, rows)}});
        m_presentedHardware = m_hardware;
    }
    if (m_result && m_presentedResult != m_result->revision) {
        if (m_observing) {
            QVariantList rows;
            for (auto point = m_result->deformedModel.constBegin();
                 point != m_result->deformedModel.constEnd(); ++point) {
                const auto d = m_result->displacementMap.value(point.key());
                const auto p = point.value();
                if (d.size() == 3 && p.size() == 3)
                    rows.append(
                        QVariant(QVariantList{point.key(), p[0] - d[0], p[1] - d[1], p[2] - d[2],
                                              d[0], d[1], d[2], p[0], p[1], p[2]}));
                if (rows.size() >= 256)
                    break;
            }
            emit presentationChanged(
                PanelId::Plot,
                {{"table_Disp2DPara",
                  table({"ID", "X0", "Y0", "Z0", "dX", "dY", "dZ", "X", "Y", "Z"}, rows)}});
        }
        double maximum = 0;
        for (const auto &d : m_result->displacementMap)
            if (d.size() == 3)
                maximum = qMax(maximum, std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]));
        if (m_running) emit presentationChanged(
            PanelId::InfoPanel, {{"lab_MaxDeformation", QString::number(maximum, 'g', 8) + " mm"}});
        m_presentedResult = m_result->revision;
        // Only key-point rows change with deformation; avoid rebuilding all-ID combo boxes per frame.
        const auto points = m_visualization->keyPoints();
        QVariantList rows;
        for (const auto &point : points) {
            const auto original = m_visualization->originalModel();
            const auto coordinates = m_originalKeyCoordinates && original
                                         ? original->value(point.pointId)
                                         : m_result->displacementMap.value(point.pointId);
            if (coordinates.size() == 3)
                rows.append(QVariant(QVariantList{qlonglong(point.pointId), coordinates[0],
                                                  coordinates[1], coordinates[2]}));
        }
        const QStringList headers = m_originalKeyCoordinates
                                        ? QStringList{"ID", "X0 (mm)", "Y0 (mm)", "Z0 (mm)"}
                                        : QStringList{"ID", "dX (mm)", "dY (mm)", "dZ (mm)"};
        if (!points.isEmpty())
            emit presentationChanged(PanelId::KeyPointSelect,
                                     {{"tablew_PointPara", table(headers, rows)}});
    }
    const auto selection = m_visualization->selectedPoint();
    if (selection.valid && m_running) {
        emit presentationChanged(
            PanelId::KeyPointEdit,
            {{"lab_XPosition", QString::number(selection.originalPositionMm[0])},
             {"lab_YPosition", QString::number(selection.originalPositionMm[1])},
             {"lab_ZPosition", QString::number(selection.originalPositionMm[2])}});
        const auto coordinates =
            m_originalKeyCoordinates ? selection.originalPositionMm : selection.displacementMm;
        emit presentationChanged(PanelId::KeyPointSelect,
                                 {{"lab_XPos", QString::number(coordinates[0])},
                                  {"lab_YPos", QString::number(coordinates[1])},
                                  {"lab_ZPos", QString::number(coordinates[2])}});
    }
}
} // namespace application
} // namespace fosd
