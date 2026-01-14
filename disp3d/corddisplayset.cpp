#include "corddisplayset.h"
#include "ui_corddisplayset.h"
#include <QMessageBox>
#include <QDebug>

CordDisplaySet::CordDisplaySet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CordDisplaySet)
{
    ui->setupUi(this);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinMaxButtonsHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    rowcount=0;
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
}

CordDisplaySet::~CordDisplaySet()
{
    if(this->addDisplayNode) delete addDisplayNode;
    if(this->editDisplayNode) delete editDisplayNode;
    delete ui;
}

#include "Timer.h"
void CordDisplaySet::initialize()
{
    Timer time;
    this->addDisplayNode=new SelectDisplayNode(this);
    addDisplayNode->GetDisplayModelPtr(displaymodel);
    addDisplayNode->initialize();
    addDisplayNode->setModal(true);
    this->editDisplayNode=new SelectDisplayNode(this);
    editDisplayNode->GetDisplayModelPtr(displaymodel);
    editDisplayNode->initialize();
    editDisplayNode->setModal(true);
    if(!displaymodel->DisplayCord.isEmpty())
    {
        for(int it = 0; it <displaymodel->DisplayCord.size(); it++)
        {
            rowcount=ui->tableWidget->rowCount();
            QTableWidgetItem *item0=new QTableWidgetItem(QString::number(displaymodel->SolidModeNodePointToFlagPoint[displaymodel->DisplayCord.at(it)->bianhao]));
            QTableWidgetItem *item1=new QTableWidgetItem(QString::number(displaymodel->DisplayCord.at(it)->x_cord));
            QTableWidgetItem *item2=new QTableWidgetItem(QString::number(displaymodel->DisplayCord.at(it)->y_cord));
            QTableWidgetItem *item3=new QTableWidgetItem(QString::number(displaymodel->DisplayCord.at(it)->z_cord));
            ui->tableWidget->setItem(rowcount-1,0,item0);
            ui->tableWidget->setItem(rowcount-1,1,item1);
            ui->tableWidget->setItem(rowcount-1,2,item2);
            ui->tableWidget->setItem(rowcount-1,3,item3);
            ui->tableWidget->setRowCount(rowcount+1);
        }
    }
    connect(this->addDisplayNode,SIGNAL(seleNodeInf(int,double,double,double)),
            this,SLOT(deal_seleNode(int,double,double,double)));
    connect(this->editDisplayNode,SIGNAL(editNodeInf(int,int,double,double,double)),
            this,SLOT(deal_editNode(int,int,double,double,double)));

}

void CordDisplaySet::on_AddCord_clicked()
{
    Timer time;
    addDisplayNode->show();
}

void CordDisplaySet::on_DeleteCord_clicked()
{
    int pos=ui->tableWidget->currentRow()+1;
    if(displaymodel->DisplayCord.size()==0)
    {
        QMessageBox msg(this);
        msg.setWindowTitle(QString::fromLocal8Bit("警告"));
        msg.setText(QString::fromLocal8Bit("点集为空，没有点可删除!"));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        if(msg.exec()==QMessageBox::Ok)
        {
            msg.close();
        }
    }
    else
    {
        if(pos==0||pos==rowcount+1)
        {
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("请先选中数据项!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok)
            {
                msg.close();
            }
        }
        else
        {
            QTableWidgetItem *item=ui->tableWidget->item(pos-1,0);
            QString nodeNo=item->data(Qt::DisplayRole).toString();
            bool found=false;
            QList <struct cordinfo*> ::iterator it;
            for (it = displaymodel->DisplayCord.begin(); it != displaymodel->DisplayCord.end()&&!found; )
            {
                if ((*(it))->bianhao == displaymodel->SolidModeFlagPointToNodePoint[nodeNo.toInt()])
                {
                    delete *it;
                    displaymodel->DisplayCord.erase(it++);
                    found = true;
//                    it = displaymodel->DisplayCord.erase(it);
//                    found = true;
                }
                else
                {
                    it++;
                }
            }
            found = false;
            QList<struct cordinfo*>::iterator it1 ;
            for (it1 = displaymodel->CurrentDisplayCord.begin(); it1 != displaymodel->CurrentDisplayCord.end()&&!found; )
            {
                if ((*(it1))->bianhao == displaymodel->SolidModeFlagPointToNodePoint[nodeNo.toInt()])
                {
                    //先释放指针对象,进行一些销毁性的内容
                    //在释放
                    delete *it1;
                    displaymodel->CurrentDisplayCord.erase(it1++);
                    found = true;
                }
                else
                {
                    it1++;
                }
            }
            ui->tableWidget->removeRow(pos-1);
            rowcount--;
            ui->tableWidget->setRowCount(rowcount+1);

            //向主界面发送信号
            emit delete_node(nodeNo.toInt());
        }
    }
}

void CordDisplaySet::on_EditCord_clicked()
{
    int pos=ui->tableWidget->currentRow()+1;
    edit_pos=pos;
    if(displaymodel->DisplayCord.size()==0)
    {
        QMessageBox msg(this);
        msg.setWindowTitle(QString::fromLocal8Bit("警告"));
        msg.setText(QString::fromLocal8Bit("点集为空，没有点可编辑!"));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        if(msg.exec()==QMessageBox::Ok)
        {
            msg.close();
        }
    }
    else
    {
        if(pos==0||pos==rowcount+1)
        {
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("请先选中数据项!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok)
            {
                msg.close();
            }
        }
        else
        {
            QTableWidgetItem *item=ui->tableWidget->item(pos-1,0);
            QString selectNode=item->data(Qt::DisplayRole).toString();
            int nodeNo = displaymodel->SolidModeFlagPointToNodePoint[selectNode.toInt()];
            this->editDisplayNode->flagAnum(false,nodeNo);
            this->editDisplayNode->show();

        }
     }
}

void CordDisplaySet::deal_seleNode(int num, double x, double y, double z)
{
    rowcount=ui->tableWidget->rowCount();
    QTableWidgetItem *item0=new QTableWidgetItem(QString::number(displaymodel->SolidModeNodePointToFlagPoint[num]));
    QTableWidgetItem *item1=new QTableWidgetItem(QString::number(x));
    QTableWidgetItem *item2=new QTableWidgetItem(QString::number(y));
    QTableWidgetItem *item3=new QTableWidgetItem(QString::number(z));
    ui->tableWidget->setItem(rowcount-1,0,item0);
    ui->tableWidget->setItem(rowcount-1,1,item1);
    ui->tableWidget->setItem(rowcount-1,2,item2);
    ui->tableWidget->setItem(rowcount-1,3,item3);
    ui->tableWidget->setRowCount(rowcount+1);
}

void CordDisplaySet::deal_editNode(int ini, int num, double x, double y, double z)
{
        ui->tableWidget->item(edit_pos-1,0)->setText(QString::number(displaymodel->SolidModeNodePointToFlagPoint[num]));
        ui->tableWidget->item(edit_pos-1,1)->setText(QString::number(x));
        ui->tableWidget->item(edit_pos-1,2)->setText(QString::number(y));
        ui->tableWidget->item(edit_pos-1,3)->setText(QString::number(z));
}
