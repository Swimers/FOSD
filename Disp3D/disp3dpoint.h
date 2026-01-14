#ifndef DISP3DPOINT_H
#define DISP3DPOINT_H

#include <QDialog>
#include <disp3dmodel.h>
#include <disp3dselect.h>

namespace Ui {
class disp3dpoint;
}

class disp3dpoint : public QDialog
{
    Q_OBJECT

public:
    explicit disp3dpoint(QWidget *parent = nullptr);
    ~disp3dpoint();

public:
    int rowcount;

public:
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}
    void initialize();

public:
    disp3dselect * addDisplayNode = nullptr;
    disp3dselect * editDisplayNode = nullptr;

private slots:
    void deal_selectNode(int num,double x,double y,double z);
    void deal_editNode(int ini,int num,double x,double y,double z);
    void on_pbtn_AddPoint_clicked();
    void on_pbtn_DelectPoint_clicked();
    void on_pbtn_EditPoint_clicked();
    void on_pbtn_EditPoint_2_clicked();

signals:
    void delete_node(int nodeNo);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    Ui::disp3dpoint *ui;

    disp3dmodel * m_disp3dmodel;

    QPoint p;
    int edit_pos;
};

#endif // DISP3DPOINT_H
