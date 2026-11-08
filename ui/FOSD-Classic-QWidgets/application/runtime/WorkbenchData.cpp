#include "FOSD-Classic-QWidgets/application/runtime/WorkbenchRuntime.h"
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
    if (request.panel == PanelId::ChannelCalibration && action == "pbtn_SenerSet") {
        auto configuration = m_configuration;
        configuration.channelCalibration.channelIndices.clear();
        configuration.channelCalibration.calibrationFactors.clear();
        configuration.channelCalibration.calibrationOffsets.clear();
        for (int i = 1; i <= 12; ++i) {
            bool indexOk = false, factorOk = false, offsetOk = false;
            const int source =
                text(f, QStringLiteral("edit_Sensor%1Order").arg(i)).toInt(&indexOk) - 1;
            const double factor =
                text(f, QStringLiteral("edit_Sensor%1Para").arg(i)).toDouble(&factorOk);
            const double offset =
                text(f, QStringLiteral("edit_Sensor%1Offset").arg(i)).toDouble(&offsetOk);
            if (!indexOk || !factorOk || !offsetOk || !std::isfinite(offset)) {
                response->success = false;
                response->message = tr("输入 %1：源序号、倍率和偏置必须为有效数值，偏置须为有限数值").arg(i);
                return true;
            }
            configuration.channelCalibration.channelIndices.push_back(source);
            configuration.channelCalibration.calibrationFactors.push_back(factor);
            configuration.channelCalibration.calibrationOffsets.push_back(offset);
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
        fields[QStringLiteral("edit_Sensor%1Offset").arg(i + 1)] = QString::number(
            i < int(channels.calibrationOffsets.size()) ? channels.calibrationOffsets[size_t(i)]
                                                        : 0.0,
            'g', 12);
    }
    emit presentationChanged(PanelId::ChannelCalibration, fields);
}

void WorkbenchRuntime::publishModel() {
    const auto model = m_visualization->originalModel();
    publishMarkers();
    emit presentationChanged(PanelId::InfoPanel,
        {{"lab_NumPoints", QString::number(model ? model->size() : 0)},
         {"lab_NumPolys", QString::number(m_visualization->modelPolygonCount())}});
    if (!m_result) {
        emit presentationChanged(PanelId::InfoPanel,
                                 {{"lab_MaxDeformation", QStringLiteral("--")}});
    }
}

void WorkbenchRuntime::publishLatest() {
    if (m_baselineClock.isValid() && m_baselineClock.elapsed() > m_baselineTimeout) { m_baselineClock.invalidate(); notice(tr("置零失败：采样超时")); }
    QVariantMap raw;
    flushUdpTraffic();
    const char *names[] = {"ptedit_SerialRec", "ptedit_UdpRec", "ptedit_TcpRec"};
    for (int i = 0; i < 3; ++i) {
        QVariantList entries;
        for (const auto &entry : m_traffic[i]) {
            if (m_resetTraffic[i] || entry.id > m_publishedTrafficId[i])
                entries.append(QVariantMap{{"text",formatTraffic(i,entry)},{"outgoing",entry.outgoing}});
        }
        if (!entries.isEmpty() || m_resetTraffic[i])
            raw[names[i]] = QVariantMap{{"entries",entries},{"clear",m_resetTraffic[i]}};
        if (!m_traffic[i].isEmpty())
            m_publishedTrafficId[i] = m_traffic[i].last().id;
        m_resetTraffic[i] = false;
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
        double maximum = 0;
        for (const auto &d : m_result->displacementMap)
            if (d.size() == 3)
                maximum = qMax(maximum, std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]));
        if (m_running) emit presentationChanged(
            PanelId::InfoPanel, {{"lab_MaxDeformation", QString::number(maximum, 'g', 8) + " mm"}});
        m_presentedResult = m_result->revision;
    }
}
} // namespace application
} // namespace fosd
