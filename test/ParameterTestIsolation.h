#pragma once
#include <QTemporaryDir>
#include <QUuid>

// Each case gets a private runtime INI, so tests neither share persisted state
// nor modify a developer's application configuration beside the executable.
class ParameterTestIsolation {
public:
    ParameterTestIsolation() : m_hadOverride(qEnvironmentVariableIsSet("FOSD_CONFIG_FILE")), m_previous(qgetenv("FOSD_CONFIG_FILE")) {}
    ~ParameterTestIsolation() { if (m_hadOverride) qputenv("FOSD_CONFIG_FILE", m_previous); else qunsetenv("FOSD_CONFIG_FILE"); }
    void reset() { qputenv("FOSD_CONFIG_FILE", m_directory.filePath(QUuid::createUuid().toString(QUuid::WithoutBraces) + ".ini").toUtf8()); }
private:
    QTemporaryDir m_directory;
    bool m_hadOverride;
    QByteArray m_previous;
};
