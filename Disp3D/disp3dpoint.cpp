#include "disp3dpoint.h"
#include "ui_disp3dpoint.h"

#include <QMessageBox>
#include <QDebug>
#include <Timer.h>
#include <QMouseEvent>

disp3dpoint::disp3dpoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disp3dpoint)
{
    ui->setupUi(this);


    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinMaxButtonsHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    rowcount = 0;
    ui->tablew_PointPara->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablew_PointPara->setRowCount(1);
    ui->tablew_PointPara->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablew_PointPara->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tablew_PointPara->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//所有列都扩展自适应宽度，填充充满整个屏幕宽度
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());        //去窗口边框
}

disp3dpoint::~disp3dpoint()
{
    delete ui;
}

void disp3dpoint::initialize()
{
    Timer time;
    qDebug() << "1";
    this->addDisplayNode = new disp3dselect(this);
    qDebug() << "2";
    addDisplayNode->GetDisplayModelPtr(m_disp3dmodel);
    qDebug() << "3";
    addDisplayNode->InitSelectPoint();
    qDebug() << "4";
    addDisplayNode->setModal(true);
    qDebug() << "5";
    this->editDisplayNode = new disp3dselect(this);
    editDisplayNode->GetDisplayModelPtr(m_disp3dmodel);
    editDisplayNode->InitSelectPoint();
    editDisplayNode->setModal(true);

    if(!m_disp3dmodel->DisplayCord.isEmpty()){
        for(int it = 0; it <m_disp3dmodel->DisplayCord.size(); it++){
            rowcount = ui->tablew_PointPara->rowCount();
            QTableWidgetItem *item0=new QTableWidgetItem(QString::number(m_disp3dmodel->SolidModeNodePointToFlagPoint[m_disp3dmodel->DisplayCord.at(it)->bianhao]));
            QTableWidgetItem *item1=new QTableWidgetItem(QString::number(m_disp3dmodel->DisplayCord.at(it)->x_cord));
            QTableWidgetItem *item2=new QTableWidgetItem(QString::number(m_disp3dmodel->DisplayCord.at(it)->y_cord));
            QTableWidgetItem *item3=new QTableWidgetItem(QString::number(m_disp3dmodel->DisplayCord.at(it)->z_cord));
            ui->tablew_PointPara->setItem(rowcount-1,0,item0);
            ui->tablew_PointPara->setItem(rowcount-1,1,item1);
            ui->tablew_PointPara->setItem(rowcount-1,2,item2);
            ui->tablew_PointPara->setItem(rowcount-1,3,item3);
            ui->tablew_PointPara->setRowCount(rowcount+1);
        }
    }
    connect(this->addDisplayNode,SIGNAL(selectNodeInf(int,double,double,double)),
            this,SLOT(deal_selectNode(int,double,double,double)));
    connect(this->editDisplayNode,SIGNAL(editNodeInf(int,int,double,double,double)),
            this,SLOT(deal_editNode(int,int,double,double,double)));
}

void disp3dpoint::deal_selectNode(int num, double x, double y, double z)
{
    rowcount=ui->tablew_PointPara->rowCount();
    QTableWidgetItem *item0=new QTableWidgetItem(QString::number(m_disp3dmodel->SolidModeNodePointToFlagPoint[num]));
    QTableWidgetItem *item1=new QTableWidgetItem(QString::number(x));
    QTableWidgetItem *item2=new QTableWidgetItem(QString::number(y));
    QTableWidgetItem *item3=new QTableWidgetItem(QString::number(z));
    ui->tablew_PointPara->setItem(rowcount-1,0,item0);
    ui->tablew_PointPara->setItem(rowcount-1,1,item1);
    ui->tablew_PointPara->setItem(rowcount-1,2,item2);
    ui->tablew_PointPara->setItem(rowcount-1,3,item3);
    ui->tablew_PointPara->setRowCount(rowcount+1);
}

void disp3dpoint::deal_editNode(int ini, int num, double x, double y, double z)
{
    ui->tablew_PointPara->item(edit_pos-1,0)->setText(QString::number(m_disp3dmodel->SolidModeNodePointToFlagPoint[num]));
    ui->tablew_PointPara->item(edit_pos-1,1)->setText(QString::number(x));
    ui->tablew_PointPara->item(edit_pos-1,2)->setText(QString::number(y));
    ui->tablew_PointPara->item(edit_pos-1,3)->setText(QString::number(z));
}

void disp3dpoint::on_pbtn_AddPoint_clicked()
{
    Timer time;
    addDisplayNode->show();
}

void disp3dpoint::on_pbtn_DelectPoint_clicked()
{
    int pos=ui->tablew_PointPara->currentRow()+1;
    if(m_disp3dmodel->DisplayCord.size()==0){
        QMessageBox msg(this);
        msg.setWindowTitle(QString::fromLocal8Bit("警告"));
        msg.setText(QString::fromLocal8Bit("点集为空，没有点可删除!"));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        if(msg.exec()==QMessageBox::Ok){
            msg.close();
        }
    }
    else{
        if(pos==0 || pos==rowcount+1){
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("请先选中数据项!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok){
                msg.close();
            }
        }
        else{
            QTableWidgetItem *item=ui->tablew_PointPara->item(pos-1,0);
            QString nodeNo=item->data(Qt::DisplayRole).toString();
            bool found=false;
            QList <struct cordinfo*> ::iterator it;
            for (it = m_disp3dmodel->DisplayCord.begin(); it != m_disp3dmodel->DisplayCord.end()&&!found; ){
                if ((*(it))->bianhao == m_disp3dmodel->SolidModeFlagPointToNodePoint[nodeNo.toInt()]){
                    delete *it;
                    m_disp3dmodel->DisplayCord.erase(it++);
                    found = true;
                }
                else{
                    it++;
                }
            }
            found = false;
            QList<struct cordinfo*>::iterator it1 ;
            for (it1 = m_disp3dmodel->CurrentDisplayCord.begin(); it1 != m_disp3dmodel->CurrentDisplayCord.end()&&!found; ){
                if ((*(it1))->bianhao == m_disp3dmodel->SolidModeFlagPointToNodePoint[nodeNo.toInt()]){
                    delete *it1;
                    m_disp3dmodel->CurrentDisplayCord.erase(it1++);
                    found = true;
                }
                else{
                    it1++;
                }
            }
            ui->tablew_PointPara->removeRow(pos-1);
            rowcount--;
            ui->tablew_PointPara->setRowCount(rowcount+1);
            //向主界面发送信号
            emit delete_node(nodeNo.toInt());
        }
    }
}

void disp3dpoint::on_pbtn_EditPoint_clicked()
{
    int pos=ui->tablew_PointPara->currentRow()+1;
    edit_pos=pos;
    if(m_disp3dmodel->DisplayCord.size()==0){
        QMessageBox msg(this);
        msg.setWindowTitle(QString::fromLocal8Bit("警告"));
        msg.setText(QString::fromLocal8Bit("点集为空，没有点可编辑!"));
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        if(msg.exec()==QMessageBox::Ok){
            msg.close();
        }
    }
    else{
        if(pos==0 || pos==rowcount+1){
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("请先选中数据项!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok){
                msg.close();
            }
        }
        else{
            QTableWidgetItem *item=ui->tablew_PointPara->item(pos-1,0);
            QString selectNode=item->data(Qt::DisplayRole).toString();
            int nodeNo = m_disp3dmodel->SolidModeFlagPointToNodePoint[selectNode.toInt()];
            this->editDisplayNode->flagAnum(false,nodeNo);
            this->editDisplayNode->show();

        }
     }
}



void disp3dpoint::on_pbtn_EditPoint_2_clicked()
{
    close();
}

void disp3dpoint::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        move(e->globalPos() - p);       //移到左上角
    }
}

void disp3dpoint::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        p = e->globalPos() - this->frameGeometry().topLeft();   //求坐标差值:当前点击坐标-窗口左上角坐标
    }
}
