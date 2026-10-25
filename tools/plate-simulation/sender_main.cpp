#include "PlateUdpSender.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include <cmath>
using namespace fosd::simulation;
class SenderConsole final : public QObject {
    Q_OBJECT
public slots:
    void fail(const QString &message) {
        QTextStream(stderr) << message << '\n';
        QCoreApplication::exit(1);
    }
};
int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("plate-udp-sender");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption({"host", "Receiver IP address.", "address", "127.0.0.1"});
    parser.addOption({"port", "Receiver UDP port.", "port", "9000"});
    parser.addOption({"rate", "Sample rate (Hz, 1..200).", "Hz", "50"});
    parser.addOption({"frequency", "Plate motion frequency (Hz).", "Hz", "0.35"});
    parser.addOption({"amplitude", "Primary transverse mode amplitude (mm).", "mm", "15"});
    parser.addOption({"duration", "Stop after N seconds (0 = until Ctrl+C).", "seconds", "0"});
    parser.process(app);
    bool portOk, rateOk, frequencyOk, amplitudeOk, durationOk;
    const int port = parser.value("port").toInt(&portOk);
    PlateSenderSettings settings;
    settings.address = QHostAddress(parser.value("host"));
    settings.port = quint16(port);
    settings.rateHz = parser.value("rate").toDouble(&rateOk);
    settings.motion.frequencyHz = parser.value("frequency").toDouble(&frequencyOk);
    settings.motion.amplitudeMm = parser.value("amplitude").toDouble(&amplitudeOk);
    const double duration = parser.value("duration").toDouble(&durationOk);
    if (!portOk || port < 1 || port > 65535 || !rateOk || !frequencyOk || !amplitudeOk ||
        !durationOk || !std::isfinite(duration) || duration < 0 || duration > 3600) {
        QTextStream(stderr) << "Invalid command line arguments\n";
        return 1;
    }
    PlateUdpSender sender;
    SenderConsole console;
    QString error;
    QObject::connect(&sender, &PlateUdpSender::errorOccurred, &console, &SenderConsole::fail);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, &sender, &PlateUdpSender::stop);
    if (!sender.start(settings, &error)) {
        QTextStream(stderr) << error << '\n';
        return 1;
    }
    QTimer limit;
    limit.setSingleShot(true);
    QObject::connect(&limit, &QTimer::timeout, &app, &QCoreApplication::quit);
    if (duration > 0)
        limit.start(int(std::ceil(duration * 1000)));
    QTextStream(stdout) << "Sending MP wavelength packets to " << settings.address.toString() << ':'
                        << port << " at " << settings.rateHz << " Hz; motion "
                        << settings.motion.frequencyHz << " Hz\n";
    return app.exec();
}
#include "sender_main.moc"
