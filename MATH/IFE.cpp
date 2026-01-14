#include "IFE.h"
#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include "config.h"

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
                        double node_u[70],double* disp)//加入单元长度，将mn和xy建立等参关系
{
    // 这里传进来的参数应该是m，写错了

    double y1,y2,y3,y4;
    double x1,x2,x3,x4;
    x_para = x_para+0.26;
    y_para = y_para+0.26;

    int unit = getUnit(x_para,y_para,z_para);

    if(unit == 1)
    {
        x1 = 0*0.001;
        x2 = 0*0.001;
        x3 = 90*0.001;
        x4 = 90*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;

        x_para = (x_para - 0.045)/0.045;
        y_para = (y_para-0.26)/0.26;
    }
    else if(unit == 2)
    {
        x1 = 90*0.001;
        x2 = 90*0.001;
        x3 = 170*0.001;
        x4 = 170*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;
        x_para = (x_para - 0.13)/0.04;
        y_para = (y_para-0.26)/0.26;
    }
    else if(unit == 3)
    {
        x1 = 170*0.001;
        x2 = 170*0.001;
        x3 = 260*0.001;
        x4 = 260*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;
        x_para = (x_para - 0.215)/0.045;
        y_para = (y_para-0.26)/0.26;
    }
    else if(unit == 4)
    {
        x1 = 260*0.001;
        x2 = 260*0.001;
        x3 = 350*0.001;
        x4 = 350*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;
        x_para = (x_para - 0.305)/0.045;
        y_para = (y_para-0.26)/0.26;
    }
    else if(unit == 5)
    {
        x1 = 350*0.001;
        x2 = 350*0.001;
        x3 = 430*0.001;
        x4 = 430*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;
        x_para = (x_para - 0.39)/0.04;
        y_para = (y_para-0.26)/0.26;
    }
    else if(unit == 6)
    {
        x1 = 430*0.001;
        x2 = 430*0.001;
        x3 = 520*0.001;
        x4 = 520*0.001;
        y1 = 520*0.001;
        y2 = 0*0.001;
        y3 = 0*0.001;
        y4 = 520*0.001;
        x_para = (x_para - 0.475)/0.045;
        y_para = (y_para-0.26)/0.26;
    }
    else{
        return;
    }

    double N1=1/4.0*(1-x_para)*(1+y_para);
    double N2=1/4.0*(1-x_para)*(1-y_para);
    double N3=1/4.0*(1+x_para)*(1-y_para);
    double N4=1/4.0*(1+x_para)*(1+y_para);
    double N5=1/16.0*(1-x_para*x_para)*(1-y_para);
    double N6=1/16.0*(1+x_para)*(1-y_para*y_para);
    double N7=1/16.0*(1-x_para*x_para)*(1+y_para);
    double N8=1/16.0*(1-x_para)*(1-y_para*y_para);
    double L1=(y1-y4)*N7-(y2-y1)*N8;
    double L2=(y2-y1)*N8-(y3-y2)*N5;
    double L3=(y3-y2)*N5-(y4-y3)*N6;
    double L4=(y4-y3)*N6-(y1-y4)*N7;
    double M2=(x1-x2)*N8-(x2-x3)*N5;
    double M3=(x2-x3)*N5-(x3-x4)*N6;
    double M4=(x3-x4)*N6-(x4-x1)*N7;   
    double M1=(x4-x1)*N7-(x1-x2)*N8;

    disp[0] = (N1*node_u[5*(unit-1)+0]+N2*node_u[5*(unit-1)+0+35]+N3*node_u[5*(unit-1)+0+35+5]+N4*node_u[5*(unit-1)+0+5])
             + M1*0+M2*0+M3*0+M4*0
             + z_para*(N1*node_u[5*(unit-1)+4]+N2*node_u[5*(unit-1)+4+35]+N3*node_u[5*(unit-1)+4+35+5]+N4*node_u[5*(unit-1)+4+5]);
    disp[1] = (N1*node_u[5*(unit-1)+1]+N2*node_u[5*(unit-1)+1+35]+N3*node_u[5*(unit-1)+1+35+5]+N4*node_u[5*(unit-1)+1+5])
             + M1*0+M2*0+M3*0+M4*0
             - z_para*(N1*node_u[5*(unit-1)+3]+N2*node_u[5*(unit-1)+3+35]+N3*node_u[5*(unit-1)+3+35+5]+N4*node_u[5*(unit-1)+3+5]);
    disp[2] = (N1*node_u[5*(unit-1)+2]+N2*node_u[5*(unit-1)+2+35]+N3*node_u[5*(unit-1)+2+35+5]+N4*node_u[5*(unit-1)+2+5])
            -(L1*node_u[5*(unit-1)+3]+L2*node_u[5*(unit-1)+3+35]+L3*node_u[5*(unit-1)+3+35+5]+L4*node_u[5*(unit-1)+3+5])
            -(M1*node_u[5*(unit-1)+4]+M2*node_u[5*(unit-1)+4+35]+M3*node_u[5*(unit-1)+4+35+5]+M4*node_u[5*(unit-1)+4+5]);
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

int IFE::getUnit(float x, float y, float z)
{
    x = x*1000;
    if(x<90 && x >= 0){
        return 1;
    }
    else if(x<170 && x >= 90){
        return 2;
    }
    else if(x<260 && x >= 170){
        return 3;
    }
    else if(x<350 && x >= 260){
        return 4;
    }
    else if(x<430 && x >= 350){
        return 5;
    }
    else if(x<=520 && x >= 430){
        return 6;
    }

    return -1;

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
    //sortedstrain[0]=stran[1]*0.000525182*1000;
    //sortedstrain[1]=stran[5]*0.0002688973*1000;
    //sortedstrain[2]=stran[4]*0.000730002*1000;
    //sortedstrain[3]=stran[0]*0.0003959347*1000;
    //sortedstrain[4]=stran[3]*0.000534296*1000;
    //sortedstrain[5]=stran[2]*0.000723779*1000;
    sortedstrain[0] = -0.00001;
    sortedstrain[1] = -0.00001;
    sortedstrain[2] = -0.00001;
    sortedstrain[3] = -0.00001;
    sortedstrain[4] = -0.00001;
    sortedstrain[5] = -0.00001;
#endif
#if 1
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
#endif
    //sortedstrain[3] = sortedstrain[1];
    //sortedstrain[4] = sortedstrain[1];
    //sortedstrain[5] = sortedstrain[1];
    //sortedstrain[1] = sortedstrain[0];
    //sortedstrain[2] = sortedstrain[0];
    sortedstrain[3] = 1;


    //乘以类刚度矩阵
    float a = Config::getInstance().getFilterCoeff();
    for (int i = 0; i < 70; i++)
    {
        new_node_u[i] = 0;
        for (int j = 0; j < 6; j++) {
            new_node_u[i] += aa_k[i][j]*sortedstrain[j];
        }
        node_u[i] = (1-a)*old_node_u[i]+a*new_node_u[i];
        old_node_u[i] = node_u[i];

        //qDebug()<<i << node_u[i];
    }
    for(int i = 0; i < 14;i++){
        node_u[i*5] = node_u[i*5+2]*node_u[i*5+4];
    }
}
