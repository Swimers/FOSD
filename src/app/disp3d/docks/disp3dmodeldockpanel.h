#ifndef DISP3DMODELDOCKPANEL_H
#define DISP3DMODELDOCKPANEL_H

#include <QWidget>
#include <QVector>

#include "disp3ddockpanel.h"

class QDockWidget;

namespace Ui {
class Disp3DModelDockPanel;
}

class Disp3DModelDockPanel : public QWidget, public Disp3DDockPanel
{
    Q_OBJECT

public:
    explicit Disp3DModelDockPanel(QWidget *parent = nullptr);
    ~Disp3DModelDockPanel();

    QString dockId() const override;
    QDockWidget *dockWidget() const override;
    int fps() const;
    int scalarIndex() const;

signals:
    void modelLoadRequested();
    void modelClearRequested();
    void modelMergeRequested();
    void modelExportRequested();
    void keyPointEditRequested();
    void keyPointSelectRequested();
    void scalarSetRequested(int index);
    void scalarClearRequested();
    void fpsChanged(int fps);
    void colorSelected();

private:
    Ui::Disp3DModelDockPanel *ui = nullptr;
    QDockWidget *m_dock = nullptr;
};

#endif // DISP3DMODELDOCKPANEL_H
