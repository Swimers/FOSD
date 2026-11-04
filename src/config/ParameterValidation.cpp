#include "ParameterCodec.h"
#include <QSet>
namespace fosd { namespace config {
bool validateCoreSection(const QString &group, const ConfigSection &v, bool ready, QString *error) {
    if (error) error->clear();
    bool ok = true;
    // Values/types/finite numbers and fixed array lengths are checked by codec.
    if (group == "communication.serial") {
        ok = v["baudRate"].toInt() > 0 && v["baudRate"].toInt() <= 4000000 &&
             v["dataBits"].toInt() >= 5 && v["dataBits"].toInt() <= 8 &&
             QSet<int>{0,2,3,4,5}.contains(v["parity"].toInt()) &&
             QSet<int>{1,2,3}.contains(v["stopBits"].toInt()) &&
             QSet<int>{0,1,2}.contains(v["flowControl"].toInt()) && (!ready || !v["portName"].toString().trimmed().isEmpty());
    } else if (group == "communication.udp") {
        ok = !v["bindAddress"].toString().isEmpty() &&
             (v["defaultPeer.address"].toString().isEmpty() == (v["defaultPeer.port"].toInt() == 0));
    } else if (group == "communication.tcp") {
        ok = QSet<int>{0,1}.contains(v["role"].toInt()) && !v["listenAddress"].toString().isEmpty() &&
             (v["role"].toInt() != 0 || !v["host"].toString().trimmed().isEmpty()) && (!ready || v["port"].toInt() > 0);
    } else if (group == "calculation") {
        const auto baseline = v["strainCalibration.originalWavelength"].toStringList();
        const auto indices = v["channelCalibration.channelIndices"].toStringList();
        const auto factors = v["channelCalibration.calibrationFactors"].toStringList();
        ok = QSet<int>{0,1}.contains(v["algorithm"].toInt()) && v["strainCalibration.calibrationFactor"].toDouble() > 0 && indices.size() == factors.size();
        for (const auto &s : baseline) ok = ok && s.toDouble() > 0;
        for (int i=0; i<indices.size(); ++i) ok = ok && indices[i].toInt() >= 0 && indices[i].toInt() < baseline.size() && factors[i].toDouble() > 0;
        if (ready) ok = ok && !baseline.isEmpty() && !indices.isEmpty() && v["strainCalibration.A"].toDouble() != 0;
    } else if (group == "storage") {
        const int flags = v["dataTypes"].toInt();
        ok = QSet<int>{0,1}.contains(v["format"].toInt()) && flags > 0 && !(flags & ~7) && (!ready || !v["path"].toString().trimmed().isEmpty());
        const auto ids = v["pointIds"].toStringList(); QSet<qint64> seen;
        for (const auto &s : ids) { const auto id=s.toLongLong(); ok=ok && id>=0 && !seen.contains(id); seen.insert(id); }
        if (flags & 4) ok = ok && (v["allPoints"].toBool() ? ids.isEmpty() : !ids.isEmpty());
    } else {
        for (auto it=v.constBegin(); it!=v.constEnd(); ++it) {
            if (it.key().contains("Color", Qt::CaseInsensitive) || it.key() == "color")
                for (const auto &x : it.value().toStringList()) ok = ok && x.toDouble()>=0 && x.toDouble()<=1;
        }
        if (group == "visualization.scalar") ok = ok && v["minimum"].toDouble()<v["maximum"].toDouble() && v["field"].toInt()>=0 && v["field"].toInt()<=4 && QSet<int>{0,1}.contains(v["mappingMode"].toInt()) && QSet<int>{0,1}.contains(v["valueMode"].toInt()) && QSet<int>{0,1}.contains(v["rangeMode"].toInt()) && v["hueStart"].toDouble()>=0 && v["hueStart"].toDouble()<=1 && v["hueEnd"].toDouble()>=0 && v["hueEnd"].toDouble()<=1;
        if (v.contains("fontSize")) ok = ok && v["fontSize"].toInt()>=1 && v["fontSize"].toInt()<=256;
        if (v.contains("lengthMm")) ok = ok && v["lengthMm"].toDouble()>0;
        if (group == "visualization.grid") ok = ok && v["halfExtentMm"].toDouble()>0 && v["divisions"].toInt()>=1 && v["divisions"].toInt()<=500 && v["opacity"].toDouble()>=0 && v["opacity"].toDouble()<=1 && v["lineWidth"].toDouble()>0 && v["lineWidth"].toDouble()<=20;
        if (group == "visualization.camera") ok = ok && QSet<int>{0,1}.contains(v["projection"].toInt()) && v["viewAngleDegrees"].toDouble()>=1 && v["viewAngleDegrees"].toDouble()<=170 && v["parallelScaleMm"].toDouble()>0;
        if (group == "visualization.world") ok = ok && v["groundHalfExtentMm"].toDouble()>0 && v["skyRadiusMm"].toDouble()>0;
        if (group == "visualization.keyPoint") ok = ok && v["radiusMm"].toDouble()>0 && v["sphereResolution"].toInt()>=4 && v["sphereResolution"].toInt()<=128;
        if (group == "visualization.picking") ok = ok && v["tolerance"].toDouble()>0 && v["tolerance"].toDouble()<=1 && v["clickTolerancePixels"].toInt()>=0 && v["clickTolerancePixels"].toInt()<=100;
    }
    if (!ok && error) *error = QStringLiteral("Invalid core parameters in [%1]%2").arg(group, ready ? " for operation" : "");
    return ok;
}
} }
