#ifndef DISP3DINFODOCKPANEL_H
#define DISP3DINFODOCKPANEL_H

#include <QWidget>

#include "disp3ddockpanel.h"

class QDockWidget;

namespace Ui {
class Disp3DInfoDockPanel;
}

class Disp3DInfoDockPanel : public QWidget, public Disp3DDockPanel
{
    Q_OBJECT

public:
    explicit Disp3DInfoDockPanel(QWidget *parent = nullptr);
    ~Disp3DInfoDockPanel();

    QString dockId() const override;
    QDockWidget *dockWidget() const override;
    bool defaultVisible() const override;

    void setModelInfo(uint pointCount, uint polyCount, uint keyPointCount);
    void setMaxDeformation(const QString &value);
    void clearModelInfo();

private:
    Ui::Disp3DInfoDockPanel *ui = nullptr;
    QDockWidget *m_dock = nullptr;
};

#endif // DISP3DINFODOCKPANEL_H
