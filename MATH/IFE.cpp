#include "IFE.h"
#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QSettings>

/*
 * 传入文件地址，将数据导入positionOriginal中
 */
IFE::IFE(){
    loadConfig("ifepara.ini");
}

void IFE::modifyOriginalCoordinates(const QString &filePath)
{
    if(filePath.isEmpty() == false)
    {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            qDebug()<<"Failed to open file:" << file.errorString();
        }
        else
        {
            for(int i=0;i<MARKPOINTNUM;i++)
            {
                QString line=file.readLine().trimmed();
                QString valuex=line.section(",",0,0,QString::SectionSkipEmpty).trimmed();
                QString valuey=line.section(",",1,1,QString::SectionSkipEmpty).trimmed();
                QString valuez=line.section(",",2,2,QString::SectionSkipEmpty).trimmed();
                positionOriginal[i][0]=valuex.toFloat();
                positionOriginal[i][1]=valuey.toFloat();
                positionOriginal[i][2]=valuez.toFloat();
            }
        }
    }
}

void IFE::N_shapefunction(
                        double x_para,double y_para,double z_para,
                        double node_u[8],double* disp)//加入单元长度，将mn和xy建立等参关系
{
    x_para=x_para/1000-1;  //等参处理
    double L=2000;
    double NN_x[14]={(1/2.0) - (x_para/2.0), 0, 0, 0, (x_para*z_para*(x_para - 1.0))/2.0, -(x_para*y_para*(x_para - 1.0))/2.0,
                     -z_para*(x_para*x_para - 1.0), y_para*(x_para*x_para - 1.0), (x_para/2.0) + (1/2.0), 0, 0, 0,
                     (x_para*z_para*(x_para + 1.0))/2.0, -(x_para*y_para*(x_para + 1.0))/2.0};
    double NN_y[14]={0, (1/2.0) - (x_para/2.0), 0, z_para*((x_para/2.0) - (1/2.0)), 0, (L*(2.0*x_para - 3)*(x_para*x_para - 1))/24.0,
                     0, -(L*x_para*(x_para*x_para - 1))/6, 0, (x_para/2.0) + (1/2.0), 0, -z_para*((x_para/2.0) + (1.0/2.0)),
                     0, (L*(2.0*x_para + 3.0)*(x_para*x_para - 1.0))/24.0};
    double NN_z[14]={0, 0, (1/2.0) - ((x_para/2.0)), -y_para*(((x_para/2.0)) - (1/2.0)), -(L*(2.0*x_para - 3.0)*(x_para*x_para - 1))/24.0, 0,
                     (L*x_para*(x_para*x_para - 1))/6, 0, 0, 0, ((x_para/2.0)) + (1/2.0), y_para*(((x_para/2.0)) + (1/2.0)),
                     -(L*(2.0*x_para + 3.0)*(x_para*x_para - 1.0))/24.0, 0};

    disp[0] = 0;
    disp[1] = 0;
    disp[2] = 0;
    for(int i=6;i<14;i++)
        disp[0] += NN_x[i]*node_u[i-6];
    for(int i=6;i<14;i++)
        disp[1] += NN_y[i]*node_u[i-6];
    for(int i=6;i<14;i++)
        disp[2] += NN_z[i]*node_u[i-6];
}

void IFE::loadConfig(const QString &filePath)
{
    if(filePath.isEmpty() == false)
    {
        QSettings settings(filePath, QSettings::IniFormat);
        // 读取 order
        for (int i = 0; i < 6; i++) {
            order[i] = settings.value(QString("order_%1").arg(i)).toInt();
        }

        // 读取 para
        for (int i = 0; i < 6; i++) {
            para[i] = settings.value(QString("para_%1").arg(i)).toDouble();
        }
    }
}

void IFE::saveConfig(const QString &filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);
    // 保存 order
    for (int i = 0; i < 6; i++) {
        settings.setValue(QString("order_%1").arg(i), order[i]);
    }

    // 保存 para
    for (int i = 0; i < 6; i++) {
        settings.setValue(QString("para_%1").arg(i), para[i]);
    }
}


/*
 * 功能:单独为二维显示提供的计算，三维使用会有一些冗余
 */
#include "global.h"
void IFE::calculateDisp()
{
#if 0
    //处理应变顺序
    double stran[6] = {0.0001241,-0.00023885,6.38E-05,-0.000383128,	-0.000304764,-2.08E-05};
    float sortedstrain[6];
    sortedstrain[0]=stran[1]*0.000525182*1000;
    sortedstrain[1]=stran[5]*0.0002688973*1000;
    sortedstrain[2]=stran[4]*0.000730002*1000;
    sortedstrain[3]=stran[0]*0.0003959347*1000;
    sortedstrain[4]=stran[3]*0.000534296*1000;
    sortedstrain[5]=stran[2]*0.000723779*1000;
#endif
    float sortedstrain[6];
    {
        QMutexLocker locker(&strainMutex);
        sortedstrain[0]=strain[order[0]-1]*(float)para[0];
        sortedstrain[1]=strain[order[1]-1]*(float)para[1];
        sortedstrain[2]=strain[order[2]-1]*(float)para[2];
        sortedstrain[3]=strain[order[3]-1]*(float)para[3];
        sortedstrain[4]=strain[order[4]-1]*(float)para[4];
        sortedstrain[5]=strain[order[5]-1]*(float)para[5];
    }


    //乘以类刚度矩阵
    for (int i = 0; i <= 7; i++)
    {
        node_u[i] = 0;
        for (int j = 0; j <= 5; j++)
        {
            node_u[i] += aa_k[i][j] * sortedstrain[j];
        }
    }

    //node_u[3]=node_u[3]*0.0003;
    //node_u[1]=node_u[1]*0.0003;
    //qDebug()<<node_u[0]<<node_u[1]<<node_u[2]<<node_u[3]<<node_u[4]<<node_u[5];

    //计算mark点位移
#if 0
    double mark_u[24];
    for (int i=0;i<=23;i++)
    {
        mark_u[i] = 0;
        for(int j=0;j<=7;j++)
        {
            mark_u[i] += matrix_mark[i][j]*node_u[j];
        }
    }
    N_shapefunction[positionOriginal[]]
    //将变形分类
    for (int i = 0; i < MARKPOINTNUM; i++)
    {
        positionDisp[i][0] = mark_u[3*i];
        positionDisp[i][1] = mark_u[3*i+1];
        positionDisp[i][2] = mark_u[3*i+2];
    }
#endif
    for(int i=0; i < MARKPOINTNUM; i++)
    {
        double Globalbeampointdisplace[3];
        N_shapefunction(positionOriginal[i][0],positionOriginal[i][1],positionOriginal[i][2],node_u,Globalbeampointdisplace);
        positionDisp[i][0] = Globalbeampointdisplace[0];
        positionDisp[i][1] = Globalbeampointdisplace[1];
        positionDisp[i][2] = Globalbeampointdisplace[2];
    }

    for(int i = 0; i < MARKPOINTNUM; i++)
    {
        for(int j=0;j<=2;j++)
            positionFinal[i][j] = positionDisp[i][j]+positionOriginal[i][j];
    }
}
