#ifndef TESTB3POCKET_H
#define TESTB3POCKET_H

#include <teststorge.h>
#include <QString>
#include <QByteArray>
#include <QRegularExpression>
#include <QDebug>
#include <QtEndian>
#include <cstring>

class testb3pocket
{
public:
    testb3pocket();

public:
    NET_SEND_BUFF4 AnalysisB3(const QString &b3);
};

#endif // TESTB3POCKET_H
