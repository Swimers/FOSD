#ifndef DISP3DWINDOW_H
#define DISP3DWINDOW_H

#include <QWidget>
#include "displaymodel.h"

namespace Ui {
class Disp3DWindow;
}

class Disp3DWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Disp3DWindow(QWidget *parent = nullptr);
    ~Disp3DWindow();
    void Init();

private slots:
    void on_btnLoadModel_clicked();

private:
    Ui::Disp3DWindow *ui;
    displayModel* displaymodel = nullptr;

};

#endif // DISP3DWINDOW_H
