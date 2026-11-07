#include "ParameterCodec.h"
#include "IniConfigStore.h"
#include <QCoreApplication>
#include <QTextStream>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    if (app.arguments().size() != 2)
        return 2;
    fosd::config::IniConfigStore store(app.arguments()[1]);
    QString error;
    if (!store.writeAll(fosd::config::defaultCoreParameters(), &error)) {
        QTextStream(stderr) << error;
        return 1;
    }
    return 0;
}
