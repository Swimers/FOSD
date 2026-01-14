#ifndef CORDDISPLAYSET_H
#define CORDDISPLAYSET_H

#include <QDialog>
#include "displaymodel.h"
#include "selectdisplaynode.h"

namespace Ui {
class CordDisplaySet;
}

class CordDisplaySet : public QDialog
{
    Q_OBJECT

public:
    explicit CordDisplaySet(QWidget *parent = nullptr);
    ~CordDisplaySet();
    int rowcount;
    void initialize();
    void GetDisplayModelPtr(displayModel* dd)
    {
        displaymodel = dd;
    }

private slots:
    void on_AddCord_clicked();
    void on_DeleteCord_clicked();
    void on_EditCord_clicked();
    void deal_seleNode(int num,double x,double y,double z);
    void deal_editNode(int ini,int num,double x,double y,double z);
signals:
    void delete_node(int nodeNo);

public:
    SelectDisplayNode *addDisplayNode = nullptr;
    SelectDisplayNode *editDisplayNode = nullptr;
private:
    Ui::CordDisplaySet *ui;
    displayModel* displaymodel = nullptr;
    int edit_pos;
};

#endif // CORDDISPLAYSET_H
