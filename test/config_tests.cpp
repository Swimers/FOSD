#include "config/IniConfigStore.h"
#include <QFile>
#include <QLockFile>
#include <QSettings>
#include <QTemporaryDir>
#include <QtTest>
#include <atomic>
#include <limits>
#include <thread>

using namespace fosd::config;
namespace {
QByteArray fileBytes(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return {};
    return file.readAll();
}
void writeKeys(IniConfigStore *store, const QString &prefix, std::atomic<bool> *failed) {
    for (int i = 0; i < 20; ++i) {
        QString error;
        if (!store->writeValue("shared", prefix + QString::number(i), i, &error)) failed->store(true);
    }
}
}
class ConfigTests final : public QObject {
    Q_OBJECT
private slots:
    void roundTripAndFormatting();
    void manualIniAndFreshReads();
    void moduleIsolationAndRemoval();
    void invalidDocuments_data();
    void invalidDocuments();
    void missingAndInvalidArguments();
    void lockAndWriteFailures();
    void concurrentUpdates();
};
void ConfigTests::roundTripAndFormatting() {
    QTemporaryDir directory; QVERIFY(directory.isValid());
    const QString path = directory.filePath("nested/settings.ini");
    IniConfigStore store(path); IConfigStore &api = store;
    const ConfigSection communication{{"host", QString::fromUtf8("中文设备")}, {"port", 9000},
        {"enabled", true}, {"empty", QString("")}, {"gain", 1.25},
        {"channels", QStringList{"1", "2", "16"}}, {"oneChannel", QStringList{"3"}},
        {"emptyChannels", QStringList{}}, {"literal", "@hello,world;=\\folder\nnext"},
        {"largeId", QVariant::fromValue(std::numeric_limits<qint64>::max())}};
    const ConfigData expected{{"communication", communication}, {"storage", {{"format", "csv"}}}};
    QString error = "old error";
    QVERIFY2(api.writeAll(expected, &error), qPrintable(error)); QVERIFY(error.isEmpty());
    IniConfigStore reopened(path); ConfigData actual;
    QVERIFY2(reopened.readAll(&actual, &error), qPrintable(error)); QCOMPARE(actual.size(), 2);
    for (auto item = communication.constBegin(); item != communication.constEnd(); ++item) {
        const QVariant value = actual["communication"].value(item.key());
        if (item.value().type() == QVariant::StringList) QCOMPARE(value.toStringList(), item.value().toStringList());
        else QCOMPARE(value.toString(), item.value().toString());
    }
    const auto bytes = fileBytes(path);
    QVERIFY(bytes.contains("[communication]")); QVERIFY(bytes.contains("[storage]"));
    QVERIFY(bytes.contains("port=9000")); QVERIFY(bytes.contains(QString::fromUtf8("中文设备").toUtf8()));
    QSettings standard(path, QSettings::IniFormat); standard.setIniCodec("UTF-8");
    QCOMPARE(standard.value("communication/port").toInt(), 9000);
    QVERIFY(standard.value("communication/enabled").toBool());
    QCOMPARE(standard.value("communication/largeId").toLongLong(), std::numeric_limits<qint64>::max());
    ConfigSection detached; QVERIFY(api.readModule("communication", &detached)); detached["port"] = 1;
    QVariant value; QVERIFY(api.readValue("communication", "port", &value)); QCOMPARE(value.toInt(), 9000);
    IniConfigStore other(directory.filePath("other.ini"));
    QVERIFY(other.writeModule("communication", {{"port", 3}}));
    QVERIFY(api.readValue("communication", "port", &value)); QCOMPARE(value.toInt(), 9000);
}
void ConfigTests::manualIniAndFreshReads() {
    QTemporaryDir directory;
    const auto path = directory.filePath("manual.ini");
    QFile file(path); QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("; edited by hand\n[communication]\nport=3456\nenabled=true\n[General]\nversion=2\n"); file.close();
    IniConfigStore store(path); QVariant value;
    QVERIFY(store.readValue("communication", "port", &value)); QCOMPARE(value.toInt(), 3456);
    QVERIFY(store.readValue("General", "version", &value)); QCOMPARE(value.toInt(), 2);
    QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
    file.write("[communication]\nport=4567\n"); file.close();
    QVERIFY(store.readValue("communication", "port", &value)); QCOMPARE(value.toInt(), 4567);
    QVERIFY(store.writeValue("General", "version", 3));
    QVERIFY(fileBytes(path).contains("[General]"));
    QVERIFY(store.readValue("General", "version", &value)); QCOMPARE(value.toInt(), 3);
    QVERIFY(store.writeAll({})); ConfigData empty; QVERIFY(store.readAll(&empty)); QVERIFY(empty.isEmpty());
}
void ConfigTests::moduleIsolationAndRemoval() {
    QTemporaryDir directory; IniConfigStore store(directory.filePath("config.ini"));
    QVERIFY(store.writeModule("communication", {{"port", 1234}, {"enabled", true}}));
    QVERIFY(store.writeModule("storage", {{"format", "csv"}}));
    QVERIFY(store.writeValue("communication", "tcp.host", "localhost"));
    QVERIFY(store.writeValue("visualization", "scale", 10.0));
    ConfigSection section; QVERIFY(store.readModule("communication", &section));
    QCOMPARE(section.size(), 3); QVERIFY(section.contains("tcp.host"));
    QVERIFY(store.writeModule("communication", {{"port", 4567}}));
    QVERIFY(store.readModule("communication", &section)); QCOMPARE(section.size(), 1);
    QVERIFY(store.readModule("storage", &section)); QCOMPARE(section["format"].toString(), QStringLiteral("csv"));
    QVERIFY(store.removeValue("visualization", "scale"));
    QVERIFY(!store.readModule("visualization", &section)); // INI has no persisted empty groups.
    QVERIFY(store.writeModule("temporary", {{"x", 1}})); QVERIFY(store.writeModule("temporary", {}));
    QVERIFY(!store.readModule("temporary", &section));
    QVERIFY(store.removeModule("communication"));
    const auto before = fileBytes(store.filePath()); QString error;
    QVERIFY(!store.removeValue("storage", "missing", &error)); QVERIFY(!error.isEmpty());
    QVERIFY(!store.removeModule("visualization", &error)); QCOMPARE(fileBytes(store.filePath()), before);
    QVERIFY(store.readModule("storage", &section, &error)); QVERIFY(error.isEmpty());
}
void ConfigTests::invalidDocuments_data() {
    QTest::addColumn<QByteArray>("bytes");
    QTest::newRow("truncated-section") << QByteArray("[communication\nport=1\n");
    QTest::newRow("nested-section") << QByteArray("[communication/nested]\nport=1\n");
    QTest::newRow("nested-key") << QByteArray("[communication]\nudp\\port=1\n");
}
void ConfigTests::invalidDocuments() {
    QFETCH(QByteArray, bytes); QTemporaryDir directory;
    const auto path = directory.filePath("bad.ini");
    QFile file(path); QVERIFY(file.open(QIODevice::WriteOnly)); QCOMPARE(file.write(bytes), qint64(bytes.size())); file.close();
    IniConfigStore store(path); QString error; const ConfigData sentinel{{"sentinel", {{"value", true}}}};
    ConfigData output = sentinel;
    QVERIFY(!store.readAll(&output, &error)); QVERIFY(!error.isEmpty()); QCOMPARE(output, sentinel);
    QVERIFY(!store.writeValue("communication", "port", 1234, &error));
    QVERIFY(!store.writeModule("storage", {}, &error)); QCOMPARE(fileBytes(path), bytes);
    QVERIFY(store.writeAll({{"communication", {{"port", 9000}}}}, &error)); QVERIFY(error.isEmpty());
    QVERIFY(store.readAll(&output)); QCOMPARE(output["communication"]["port"].toInt(), 9000);
}
void ConfigTests::missingAndInvalidArguments() {
    QTemporaryDir directory; IniConfigStore store(directory.filePath("missing.ini"));
    QString error; ConfigData output; QVariant value(5);
    QVERIFY(!store.readAll(&output, &error)); QVERIFY(!error.isEmpty()); QVERIFY(!QFile::exists(store.filePath()));
    QVERIFY(!store.removeModule("missing")); QVERIFY(!store.readAll(nullptr, &error));
    QVERIFY(!store.readModule("x", nullptr, &error)); QVERIFY(!store.readValue("x", "key", nullptr, &error));
    QVERIFY(!store.writeModule(" ", {}, &error)); QVERIFY(!store.writeValue("x/y", "key", 1, &error));
    QVERIFY(!store.writeValue("x", "key/child", 1, &error)); QVERIFY(!store.writeValue("x", " key ", 1, &error));
    QVERIFY(!store.writeValue("x", "key", QVariant(), &error));
    QVERIFY(!store.writeValue("x", "key", QVariantMap{{"nested", 1}}, &error));
    QVERIFY(!store.writeValue("x", "key", std::numeric_limits<double>::infinity(), &error));
    QVERIFY(!QFile::exists(store.filePath())); QVERIFY(store.writeValue("x", "key", 1));
    QVERIFY(!store.readValue("x", "missing", &value, &error)); QCOMPARE(value.toInt(), 5);
    ConfigSection section{{"sentinel", true}}; QVERIFY(!store.readModule("missing", &section));
    QCOMPARE(section.value("sentinel").toBool(), true);
    const auto before = fileBytes(store.filePath());
    QVERIFY(!store.writeAll({{"bad/name", {{"key", 1}}}}, &error)); QCOMPARE(fileBytes(store.filePath()), before);
    QVERIFY(!store.writeValue("x", "KEY", 2, &error)); QCOMPARE(fileBytes(store.filePath()), before);
    QVERIFY(!store.writeValue("X", "other", 2, &error)); QCOMPARE(fileBytes(store.filePath()), before);
    IniConfigStore empty(" "); QVERIFY(!empty.writeAll({}, &error));
    IniConfigStore invalidTimeout(directory.filePath("invalid.ini"), -1);
    QVERIFY(!invalidTimeout.writeAll({}, &error)); QVERIFY(!QFile::exists(invalidTimeout.filePath()));
}
void ConfigTests::lockAndWriteFailures() {
    QTemporaryDir directory; IniConfigStore store(directory.filePath("locked.ini"), 0);
    QVERIFY(store.writeValue("storage", "format", "csv")); const auto before = fileBytes(store.filePath());
    QLockFile blocker(store.filePath() + ".lock"); QVERIFY(blocker.tryLock(0)); QString error;
    QVERIFY(!store.writeValue("storage", "format", "sqlite", &error));
    QVERIFY(error.contains("lock")); QCOMPARE(fileBytes(store.filePath()), before); blocker.unlock();
    QVERIFY(store.writeValue("storage", "format", "sqlite", &error)); QVERIFY(error.isEmpty());
    IniConfigStore directoryTarget(directory.path()); QVERIFY(!directoryTarget.writeAll({}, &error)); QVERIFY(!error.isEmpty());
    QFile parent(directory.filePath("file-not-directory")); QVERIFY(parent.open(QIODevice::WriteOnly)); parent.write("original"); parent.close();
    IniConfigStore blocked(parent.fileName() + "/config.ini");
    QVERIFY(!blocked.writeAll({}, &error)); QCOMPARE(fileBytes(parent.fileName()), QByteArray("original"));
}
void ConfigTests::concurrentUpdates() {
    QTemporaryDir directory; const auto path = directory.filePath("shared.ini");
    IniConfigStore first(path, 5000), second(path, 5000); std::atomic<bool> failed{false};
    std::thread a(writeKeys, &first, QString("a"), &failed);
    std::thread b(writeKeys, &second, QString("b"), &failed);
    std::thread c(writeKeys, &first, QString("c"), &failed);
    a.join(); b.join(); c.join(); QVERIFY(!failed.load());
    ConfigSection values; QVERIFY(first.readModule("shared", &values)); QCOMPARE(values.size(), 60);
    for (const auto &prefix : {QString("a"), QString("b"), QString("c")})
        for (int i = 0; i < 20; ++i) QCOMPARE(values[prefix + QString::number(i)].toInt(), i);
}
QTEST_GUILESS_MAIN(ConfigTests)
#include "config_tests.moc"
