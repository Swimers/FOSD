#include "selectdisplaynode.h"
#include "ui_selectdisplaynode.h"
#include <QMessageBox>
#include <QDebug>

SelectDisplayNode::SelectDisplayNode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDisplayNode)
{
    ui->setupUi(this);

    ui->setupUi(this);
    this->flag=true;//true为添加，false为编辑，默认为添加
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

    connect(ui->comboBox,  SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comboBox_currentIndexChanged(const QString &)));
    connect(ui->ok,  &QPushButton::clicked, this, &SelectDisplayNode::on_ok_clicked);
    connect(ui->btnFind,  &QPushButton::clicked, this, &SelectDisplayNode::on_btnFind_clicked);
}

SelectDisplayNode::~SelectDisplayNode()
{
    delete ui;
}

void SelectDisplayNode::initialize()
{
    int S_number = displaymodel->SolidModeFlagPointToNodePoint.size();//实体模型上重要点的编号
    for(int i=1;i<=S_number;i++)
    {
        ui->comboBox->addItem(QString::number(i));
    }
}

void SelectDisplayNode::on_comboBox_currentIndexChanged(const QString &arg1)
{
    arg = arg1;
    int node_num=arg.toInt();
    int nodeNo = displaymodel->SolidModeFlagPointToNodePoint[node_num];
//    x_coordination=(*nodes[node_num]).cord[0];
//    y_coordination=(*nodes[node_num]).cord[1];
//    z_coordination=(*nodes[node_num]).cord[2];
    ui->x_node->setText(QString::number((*(displaymodel->new_node_info1[nodeNo])).x_cord));
    ui->y_node->setText(QString::number((*(displaymodel->new_node_info1[nodeNo])).y_cord));
    ui->z_node->setText(QString::number((*(displaymodel->new_node_info1[nodeNo])).z_cord));
}

void SelectDisplayNode::on_btnFind_clicked()
{
    QString inputText = ui->editNum->text(); // 获取用户输入的文本
    bool isNumeric; // 标志变量，用于标识是否是数字
    int integerValue = inputText.toInt(&isNumeric);
    if(!isNumeric) return;

    int S_number = ui->comboBox->count();//实体模型上重要点的编号
    if(integerValue>S_number||integerValue<=1) return;

    ui->comboBox->setCurrentIndex(integerValue-1);
}

void SelectDisplayNode::on_ok_clicked()
{
    if(this->flag==true)
    {
        //QString arg=ui->comboBox->currentText();
        int nodeNo=displaymodel->SolidModeFlagPointToNodePoint[arg.toInt()];
        bool found = 0;
        QList<struct cordinfo*>::iterator t;
        for (t = displaymodel->DisplayCord.begin(); t != displaymodel->DisplayCord.end() && !found; t++)
        {
            if ((*t)->bianhao == nodeNo)
            {
                found = true;
            }
        }
        if(!found)
        {
            struct cordinfo* cordinfo = new struct cordinfo;
            (*cordinfo).bianhao =(*(displaymodel->new_node_info1[nodeNo])).bianhao;
            (*cordinfo).x_cord = (*(displaymodel->new_node_info1[nodeNo])).x_cord;
            (*cordinfo).y_cord = (*(displaymodel->new_node_info1[nodeNo])).y_cord;
            (*cordinfo).z_cord = (*(displaymodel->new_node_info1[nodeNo])).z_cord;
            struct cordinfo* cordinfo1 = new struct cordinfo;
            //定义一个显示动态当前结点的坐标的变量
            (*cordinfo1).bianhao = (*(displaymodel->new_node_info1[nodeNo])).bianhao;
            (*cordinfo1).x_cord = (*(displaymodel->new_node_info1[nodeNo])).x_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[(*cordinfo1).bianhao]->xdisplace;
            (*cordinfo1).y_cord = (*(displaymodel->new_node_info1[nodeNo])).y_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[(*cordinfo1).bianhao]->ydisplace;
            (*cordinfo1).z_cord = (*(displaymodel->new_node_info1[nodeNo])).z_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[(*cordinfo1).bianhao]->zdisplace;
            selected_node=(*cordinfo).bianhao;
            x_coordination= (*cordinfo).x_cord;
            y_coordination= (*cordinfo).y_cord;
            z_coordination= (*cordinfo).z_cord;
            x_coordination_deformed= (*cordinfo1).x_cord;
            y_coordination_deformed= (*cordinfo1).y_cord;
            z_coordination_deformed= (*cordinfo1).z_cord;
            displaymodel->DisplayCord.push_back(cordinfo);//记录重要点的初始位置
            displaymodel->CurrentDisplayCord.push_back(cordinfo1);//记录重要点的当前位置
            emit seleNodeInf(selected_node,x_coordination,y_coordination,z_coordination);//变形前
            emit seleNodeInf_deformed(selected_node,x_coordination_deformed,y_coordination_deformed
                                      ,z_coordination_deformed);

//            delete cordinfo1;
//            cordinfo1=nullptr;
//            delete cordinfo;
//            cordinfo=nullptr;
    //        emit seleNodeInf(selected_node);
        }
        else
        {
            QMessageBox msg(this);
            msg.setWindowTitle(QString::fromLocal8Bit("警告"));
            msg.setText(QString::fromLocal8Bit("该点已在关键点集合中!"));
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);
            if(msg.exec()==QMessageBox::Ok)
            {
                msg.close();
            }
        }

    }
    else
    {
        //QString arg=ui->comboBox->currentText();
        int nodeNo=displaymodel->SolidModeFlagPointToNodePoint[arg.toInt()];
        bool found = false;
        QList<struct cordinfo*>::iterator t;
        for (t = displaymodel->DisplayCord.begin(); t != displaymodel->DisplayCord.end() && !found; t++)
        {
            if ((*t)->bianhao == nodeNo)
            {
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
            if(msg.exec()==QMessageBox::Ok)
            {
                msg.close();
            }
        }
        else
         {
            found = false;
            QList<struct cordinfo*>::iterator t;
            for (t = displaymodel->DisplayCord.begin(); t != displaymodel->DisplayCord.end()&&!found;)//先删除显示坐标集合中之前的数据
            {
                if ((*(t))->bianhao == this->inicord)
                {
                    delete *t;
                    displaymodel->DisplayCord.erase(t++);
                    found = true;
                }
                else
                {
                    t++;
                }
            }
            //产生新的集合元素数据
            struct cordinfo* cordinfo = new struct cordinfo;
            (*cordinfo).bianhao = nodeNo;
            (*cordinfo).x_cord = (*(displaymodel->new_node_info1[nodeNo])).x_cord;
            (*cordinfo).y_cord = (*(displaymodel->new_node_info1[nodeNo])).y_cord;
            (*cordinfo).z_cord = (*(displaymodel->new_node_info1[nodeNo])).z_cord;
            displaymodel->DisplayCord.push_back(cordinfo);
            //当前存储显示重要点的集合数据也需要修改
            QList<struct cordinfo*>::iterator t1;
            for (t1 = displaymodel->CurrentDisplayCord.begin(); t1 != displaymodel->CurrentDisplayCord.end() && !found;)
            {
                if ((*(t1))->bianhao == this->inicord)
                {
                    delete *t1;
                    displaymodel->CurrentDisplayCord.erase(t1++);
                    found = true;
                }
                else
                {
                    t1++;
                }
            }
            struct cordinfo* cordinfo1 = new struct cordinfo; //当前时刻点的重要位移
            (*cordinfo1).bianhao = nodeNo;
            (*cordinfo1).x_cord = (*(displaymodel->new_node_info1[nodeNo])).x_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeNo]->xdisplace;
            (*cordinfo1).y_cord = (*(displaymodel->new_node_info1[nodeNo])).y_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeNo]->ydisplace;
            (*cordinfo1).z_cord = (*(displaymodel->new_node_info1[nodeNo])).z_cord + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[nodeNo]->zdisplace;
            displaymodel->CurrentDisplayCord.push_back(cordinfo1);
            emit editNodeInf(this->inicord,(*cordinfo).bianhao,(*cordinfo).x_cord,
                             (*cordinfo).y_cord,(*cordinfo).z_cord);

            emit editNodeInf_deformed(this->inicord,(*cordinfo1).bianhao,(*cordinfo1).x_cord,
                                      (*cordinfo1).y_cord,(*cordinfo1).z_cord);

          }
    }

        this->close();

}

void SelectDisplayNode::flagAnum(bool flag,int iniCord)
{
    this->flag=flag;
    this->inicord=iniCord;
}








