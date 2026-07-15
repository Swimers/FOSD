#ifndef DISP3DVIEWDOCKPANEL_H
#define DISP3DVIEWDOCKPANEL_H

#include <QWidget>

#include "disp3ddockpanel.h"

class QDockWidget;

namespace Ui {
class Disp3DViewDockPanel;
}

class Disp3DViewDockPanel : public QWidget, public Disp3DDockPanel
{
    Q_OBJECT

public:
    explicit Disp3DViewDockPanel(QWidget *parent = nullptr);
    ~Disp3DViewDockPanel();

    QString dockId() const override;
    QDockWidget *dockWidget() const override;
    int gridSize() const;
    int selectedViewIndex() const;
    bool xoyGridChecked() const;
    bool xozGridChecked() const;
    bool zoyGridChecked() const;
    void resetGridChecks();

signals:
    void axesVisibleChanged(bool visible);
    void axesSizeChanged(int size);
    void gridVisibleChanged(bool visible);
    void gridSizeChangeFinished(int size);
    void xoyGridVisibleChanged(bool visible);
    void xozGridVisibleChanged(bool visible);
    void zoyGridVisibleChanged(bool visible);
    void cameraLockChanged(bool locked);
    void cameraFollowChanged(bool followed);
    void switchViewRequested(int index);

private:
    Ui::Disp3DViewDockPanel *ui = nullptr;
    QDockWidget *m_dock = nullptr;
};

#endif // DISP3DVIEWDOCKPANEL_H
