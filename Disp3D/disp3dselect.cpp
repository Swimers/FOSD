#include "disp3dselect.h"
#include "ui_disp3dselect.h"

#include <QMessageBox>
#include <QDebug>
#include <QMouseEvent>

disp3dselect::disp3dselect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disp3dselect)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());        //去窗口边框

    this->flag = true;//true为添加，false为编辑，默认为添加
    this->inicord=0;
    selected_node=0;
    x_coordination=0;
    y_coordination=0;
    z_coordination=0;
    x_coordination_deformed=0;
    y_coordination_deformed=0;
    z_coordination_deformed=0;

    //ui->node_number->setStyleSheet("border :1px ;background-color: rgba(0,0,0,0)");
    //ui->node_coordination->setStyleSheet("border :1px ;background-color: rgba(0,0,0,0)");
}

disp3dselect::~disp3dselect()
{
    delete ui;
}

void disp3dselect::InitSelectPoint()
{
    int S_number = m_disp3dmodel->SolidModeFlagPointToNodePoint.size();//实体模型上重要点的编号
    for(int i=1;i<=S_number;i++){
        ui->cbox_SelectPoint->addItem(QString::number(i));
    }
}

void disp3dselect::flagAnum(bool flag,int iniCord)
{
    this->flag = flag;
    this->inicord = iniCord;
}

void disp3dselect::on_pbtn_SelectSet_clicked()
{
    if(this->flag==true)
    {
        QString arg = ui->cbox_SelectPoint->currentText();
        int nodeNo = m_disp3dmodel->SolidModeFlagPointToNodePoint[arg.toInt()];
        bool found = 0;
        QList<struct cordinfo*>::iterator t;
        for (t = m_disp3dmodel->DisplayCord.begin(); t != m_disp3dmodel->DisplayCord.end() && !found; t++){
            if ((*t)->bianhao == nodeNo){
                found = true;
            }
        }
        if(!found){
            struct cordinfo* cordinfo = new struct cordinfo;
            (*cordinfo).bianhao =(*(m_disp3dmodel->new_node_info1[nodeNo])).bianhao;
            (*cordinfo).x_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).x_cord;
            (*cordinfo).y_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).y_cord;
            (*cordinfo).z_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).z_cord;
            struct cordinfo* cordinfo1 = new struct cordinfo;
            //定义一个显示动态当前结点的坐标的变量
            (*cordinfo1).bianhao = (*(m_disp3dmodel->new_node_info1[nodeNo])).bianhao;
            (*cordinfo1).x_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).x_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[(*cordinfo1).bianhao]->xdisplace;
            (*cordinfo1).y_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).y_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[(*cordinfo1).bianhao]->ydisplace;
            (*cordinfo1).z_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).z_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[(*cordinfo1).bianhao]->zdisplace;
            selected_node=(*cordinfo).bianhao;
            x_coordination= (*cordinfo).x_cord;
            y_coordination= (*cordinfo).y_cord;
            z_coordination= (*cordinfo).z_cord;
            x_coordination_deformed= (*cordinfo1).x_cord;
            y_coordination_deformed= (*cordinfo1).y_cord;
            z_coordination_deformed= (*cordinfo1).z_cord;
            m_disp3dmodel->DisplayCord.push_back(cordinfo);//记录重要点的初始位置
            m_disp3dmodel->CurrentDisplayCord.push_back(cordinfo1);//记录重要点的当前位置
            emit selectNodeInf(selected_node,x_coordination,y_coordination,z_coordination);//变形前
            emit selectNodeInf_deformed(selected_node,x_coordination_deformed,y_coordination_deformed
                                      ,z_coordination_deformed);

//            delete cordinfo1;
//            cordinfo1=nullptr;
//            delete cordinfo;
//            cordinfo=nullptr;
//            emit seleNodeInf(selected_node);
        }
        else{
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("该点已在关键点集合中!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok){
                msg.close();
            }
        }

    }
    else
    {
        QString arg=ui->cbox_SelectPoint->currentText();
        int nodeNo=m_disp3dmodel->SolidModeFlagPointToNodePoint[arg.toInt()];
        bool found = false;
        QList<struct cordinfo*>::iterator t;
        for (t = m_disp3dmodel->DisplayCord.begin(); t != m_disp3dmodel->DisplayCord.end() && !found; t++){
            if ((*t)->bianhao == nodeNo){
                found = true;
            }
        }
        if (found)
        {
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("该点已在关键点集合中!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok){
                msg.close();
            }
        }
        else{
            found = false;
            QList<struct cordinfo*>::iterator t;
            for (t = m_disp3dmodel->DisplayCord.begin(); t != m_disp3dmodel->DisplayCord.end()&&!found;)//先删除显示坐标集合中之前的数据{
                if ((*(t))->bianhao == this->inicord){
                    delete *t;
                    m_disp3dmodel->DisplayCord.erase(t++);
                    found = true;
                }
                else{
                    t++;
                }
            }
            //产生新的集合元素数据
            struct cordinfo* cordinfo = new struct cordinfo;
            (*cordinfo).bianhao = nodeNo;
            (*cordinfo).x_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).x_cord;
            (*cordinfo).y_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).y_cord;
            (*cordinfo).z_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).z_cord;
            m_disp3dmodel->DisplayCord.push_back(cordinfo);
            //当前存储显示重要点的集合数据也需要修改
            QList<struct cordinfo*>::iterator t1;
            for (t1 = m_disp3dmodel->CurrentDisplayCord.begin(); t1 != m_disp3dmodel->CurrentDisplayCord.end() && !found;){
                if ((*(t1))->bianhao == this->inicord){
                    delete *t1;
                    m_disp3dmodel->CurrentDisplayCord.erase(t1++);
                    found = true;
                }
                else{
                    t1++;
                }
            }
            struct cordinfo* cordinfo1 = new struct cordinfo; //当前时刻点的重要位移
            (*cordinfo1).bianhao = nodeNo;
            (*cordinfo1).x_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).x_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[nodeNo]->xdisplace;
            (*cordinfo1).y_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).y_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[nodeNo]->ydisplace;
            (*cordinfo1).z_cord = (*(m_disp3dmodel->new_node_info1[nodeNo])).z_cord + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[nodeNo]->zdisplace;
            m_disp3dmodel->CurrentDisplayCord.push_back(cordinfo1);
            emit editNodeInf(this->inicord,(*cordinfo).bianhao,(*cordinfo).x_cord,
                             (*cordinfo).y_cord,(*cordinfo).z_cord);

            emit editNodeInf_deformed(this->inicord,(*cordinfo1).bianhao,(*cordinfo1).x_cord,
                                      (*cordinfo1).y_cord,(*cordinfo1).z_cord);
          }
        this->close();
}

void disp3dselect::on_pbtn_FindPoint_clicked()
{
    QString inputText = ui->edit_PoontNum->text(); // 获取用户输入的文本
    bool isNumeric; // 标志变量，用于标识是否是数字
    int integerValue = inputText.toInt(&isNumeric);
    if(!isNumeric) return;

    int S_number = ui->cbox_SelectPoint->count();//实体模型上重要点的编号
    if(integerValue>S_number||integerValue<=1) return;

    ui->cbox_SelectPoint->setCurrentIndex(integerValue-1);
}

void disp3dselect::on_cbox_SelectPoint_currentIndexChanged(const QString &arg1)
{
    int node_num=arg1.toInt();
    int nodeNo = m_disp3dmodel->SolidModeFlagPointToNodePoint[node_num];
//    x_coordination=(*nodes[node_num]).cord[0];
//    y_coordination=(*nodes[node_num]).cord[1];
//    z_coordination=(*nodes[node_num]).cord[2];
    ui->edit_NodeX->setText(QString::number((*(m_disp3dmodel->new_node_info1[nodeNo])).x_cord));
    ui->edit_NodeY->setText(QString::number((*(m_disp3dmodel->new_node_info1[nodeNo])).y_cord));
    ui->edit_NodeZ ->setText(QString::number((*(m_disp3dmodel->new_node_info1[nodeNo])).z_cord));
}

void disp3dselect::on_pbtn_SelectCancel_clicked()
{
    close();
}

void disp3dselect::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        move(e->globalPos() - p);                               //移到左上角
    }
}

void disp3dselect::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        p = e->globalPos() - this->frameGeometry().topLeft();   //求坐标差值:当前点击坐标-窗口左上角坐标
    }
}
