#include "IFE.h"
#include "global.h"

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
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
            qDebug()<<"Failed to open file:" << file.errorString();
        }
        else{
            for(int i=0;i<MARKPOINTNUM;i++){
                QString line=file.readLine().trimmed();
                QString valuex=line.section(",",0,0,QString::SectionSkipEmpty).trimmed();
                QString valuey=line.section(",",1,1,QString::SectionSkipEmpty).trimmed();
                QString valuez=line.section(",",2,2,QString::SectionSkipEmpty).trimmed();
                positionOriginal[i][0]=valuex.toFloat() - axis[0];
                positionOriginal[i][1]=valuey.toFloat() - axis[1];
                positionOriginal[i][2]=valuez.toFloat() - axis[2];
            }
        }
    }
}

void IFE::N_shapefunction(double x_para,double y_para,double z_para,double node_u[8],double* disp)//加入单元长度，将mn和xy建立等参关系
{
    double y1,y2,y3,y4;
    double x1,x2,x3,x4;
    //x_para = x_para+0.26;
    y_para = y_para-0.12;

    int unit = getUnit(x_para,y_para);

    if(unit == 1){
        x1 = 0;
        x2 = 0.4;
        x3 = 0.4;
        x4 = 0;
        y1 = 0*0.001;
        y2 = 0*0.001;
        y3 = 0.3;
        y4 = 0.4;
    }
    else if(unit == 2){
        x1 = 0.4;
        x2 = 1.4;
        x3 = 1.4;
        x4 = 0.4;
        y1 = 0;
        y2 = 0;
        y3 = 0.2;
        y4 = 0.3;
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

void IFE::ShapeFunction_New(double x_para, double y_para, double node_u[20], double *disp)
{
    x_para = x_para / 1000.0;
    y_para = y_para / 1000.0;
    double u_[20] = {0};
    double x_nodes[4];
    double y_nodes[4];


    int unit = getUnit(x_para,y_para);

    if(unit == 1){
        x_nodes[0]=0;   x_nodes[1]=0.4;   x_nodes[2]=0.4;   x_nodes[3]=0;
        y_nodes[0]=0;   y_nodes[1]=0;     y_nodes[2]=0.3;   y_nodes[3]=0.4;
//        x1 = 0;     x2 = 0.4;   x3 = 0.4;   x4 = 0;
//        y1 = 0;     y2 = 0;     y3 = 0.3;   y4 = 0.4;
        for (int i=0;i<5;i++) {
            u_[i+5] = node_u[i];
        }
        for (int i=0;i<5;i++) {
            u_[i+10] = node_u[i+15];
        }
    }
    else if(unit == 2){
        x_nodes[0]=0.4; x_nodes[1]=1.4;   x_nodes[2]=1.4;   x_nodes[3]=0.4;
        y_nodes[0]=0;   y_nodes[1]=0;     y_nodes[2]=0.2;   y_nodes[3]=0.36;
//        x1 = 0.4;   x2 = 1.4;   x3 = 1.4;   x4 = 0.4;
//        y1 = 0;     y2 = 0;     y3 = 0.2;   y4 = 0.36;
        for (int i=0;i<20;i++) {
            u_[i] = node_u[i];
        }
    }
    else{
        return;
    }

    double s = 0.0;
    double t = 0.0;
    const int max_iter = 20;
    const double tol = 1e-8;
    for (int iter = 0; iter < max_iter; ++iter) {
        double N1 = 0.25 * (1 - s) * (1 - t);
        double N2 = 0.25 * (1 + s) * (1 - t);
        double N3 = 0.25 * (1 + s) * (1 + t);
        double N4 = 0.25 * (1 - s) * (1 + t);
        double dN1ds = -0.25 * (1 - t);
        double dN1dt = -0.25 * (1 - s);
        double dN2ds = 0.25 * (1 - t);
        double dN2dt = -0.25 * (1 + s);
        double dN3ds = 0.25 * (1 + t);
        double dN3dt = 0.25 * (1 + s);
        double dN4ds = -0.25 * (1 + t);
        double dN4dt = 0.25 * (1 - s);
        double x_est = N1 * x_nodes[0] + N2 * x_nodes[1] + N3 * x_nodes[2] + N4 * x_nodes[3];
        double y_est = N1 * y_nodes[0] + N2 * y_nodes[1] + N3 * y_nodes[2] + N4 * y_nodes[3];
        double dx_ds = dN1ds * x_nodes[0] + dN2ds * x_nodes[1] + dN3ds * x_nodes[2] + dN4ds * x_nodes[3];
        double dy_ds = dN1ds * y_nodes[0] + dN2ds * y_nodes[1] + dN3ds * y_nodes[2] + dN4ds * y_nodes[3];
        double dx_dt = dN1dt * x_nodes[0] + dN2dt * x_nodes[1] + dN3dt * x_nodes[2] + dN4dt * x_nodes[3];
        double dy_dt = dN1dt * y_nodes[0] + dN2dt * y_nodes[1] + dN3dt * y_nodes[2] + dN4dt * y_nodes[3];
        double det = dx_ds * dy_dt - dy_ds * dx_dt;
        if (qFuzzyIsNull(det)) {
            qDebug() << "警告: 雅可比矩阵行列式接近零，迭代可能不收敛";
            break;
        }
        double J_inv[2][2] = {
            {dy_dt / det, -dy_ds / det},
            {-dx_dt / det, dx_ds / det}
        };
        double delta_s = J_inv[0][0] * (x_para - x_est) + J_inv[0][1] * (y_para - y_est);
        double delta_t = J_inv[1][0] * (x_para - x_est) + J_inv[1][1] * (y_para - y_est);
        s += delta_s;
        t += delta_t;
        s = qBound(-1.0, s, 1.0);
        t = qBound(-1.0, t, 1.0);
    }

    double N1 = 0.25 * (1 - s) * (1 - t);
    double N2 = 0.25 * (1 + s) * (1 - t);
    double N3 = 0.25 * (1 + s) * (1 + t);
    double N4 = 0.25 * (1 - s) * (1 + t);
    double u_val = N1 * u_[0] + N2 * u_[5] + N3 * u_[10] + N4 * u_[15];
    double v_val = N1 * u_[1] + N2 * u_[6] + N3 * u_[11] + N4 * u_[16];
    double w_val = N1 * u_[2] + N2 * u_[7] + N3 * u_[12] + N4 * u_[17];
    double theta_x = N1 * u_[3] + N2 * u_[8] + N3 * u_[13] + N4 * u_[18];
    double theta_y = N1 * u_[4] + N2 * u_[9] + N3 * u_[14] + N4 * u_[19];
    const double h = 0.01; // 假设h的值为1，原代码未定义

    disp[0] = u_val + h * theta_y;
    disp[1] = v_val - h * theta_x;
    disp[2] = w_val;
}

void IFE::ShapeFunction_New_HYY(double x_para, double y_para, double node_u[22], double *disp)
{
    x_para = x_para / 1000.0;
    y_para = y_para / 1000.0;
    double u_[20] = {0};
    double x_nodes[4];
    double y_nodes[4];


    int unit = getUnit(x_para,y_para);

    if(unit == 1){
        x_nodes[0]=0;   x_nodes[1]=0.7;   x_nodes[2]=0.7;   x_nodes[3]=0;
        y_nodes[0]=0;   y_nodes[1]=0;     y_nodes[2]=0.3;   y_nodes[3]=0.4;
//        x1 = 0;     x2 = 0.4;   x3 = 0.4;   x4 = 0;
//        y1 = 0;     y2 = 0;     y3 = 0.3;   y4 = 0.4;
        for (int i=0;i<5;i++) {
            u_[i+5] = node_u[i + 8];
        }
        for (int i=0;i<5;i++) {
            u_[i+10] = node_u[i + 8];
        }
    }
    else if(unit == 2){
        x_nodes[0]=0.7; x_nodes[1]=1.4;   x_nodes[2]=1.4;   x_nodes[3]=0.7;
        y_nodes[0]=0;   y_nodes[1]=0;     y_nodes[2]=0.2;   y_nodes[3]=0.36;
//        x1 = 0.4;   x2 = 1.4;   x3 = 1.4;   x4 = 0.4;
//        y1 = 0;     y2 = 0;     y3 = 0.2;   y4 = 0.36;
        for (int i=0;i<5;i++) {
            u_[i] = node_u[i + 16];
        }
        for (int i=0;i<5;i++) {
            u_[i+5] = node_u[i + 8];
        }
        for (int i=0;i<5;i++) {
            u_[i+10] = node_u[i + 8];
        }
        for (int i=0;i<5;i++) {
            u_[i+15] = node_u[i + 16];
        }
    }
    else{
        return;
    }

    double s = 0.0;
    double t = 0.0;
    const int max_iter = 20;
    const double tol = 1e-8;
    for (int iter = 0; iter < max_iter; ++iter) {
        double N1 = 0.25 * (1 - s) * (1 - t);
        double N2 = 0.25 * (1 + s) * (1 - t);
        double N3 = 0.25 * (1 + s) * (1 + t);
        double N4 = 0.25 * (1 - s) * (1 + t);
        double dN1ds = -0.25 * (1 - t);
        double dN1dt = -0.25 * (1 - s);
        double dN2ds = 0.25 * (1 - t);
        double dN2dt = -0.25 * (1 + s);
        double dN3ds = 0.25 * (1 + t);
        double dN3dt = 0.25 * (1 + s);
        double dN4ds = -0.25 * (1 + t);
        double dN4dt = 0.25 * (1 - s);
        double x_est = N1 * x_nodes[0] + N2 * x_nodes[1] + N3 * x_nodes[2] + N4 * x_nodes[3];
        double y_est = N1 * y_nodes[0] + N2 * y_nodes[1] + N3 * y_nodes[2] + N4 * y_nodes[3];
        double dx_ds = dN1ds * x_nodes[0] + dN2ds * x_nodes[1] + dN3ds * x_nodes[2] + dN4ds * x_nodes[3];
        double dy_ds = dN1ds * y_nodes[0] + dN2ds * y_nodes[1] + dN3ds * y_nodes[2] + dN4ds * y_nodes[3];
        double dx_dt = dN1dt * x_nodes[0] + dN2dt * x_nodes[1] + dN3dt * x_nodes[2] + dN4dt * x_nodes[3];
        double dy_dt = dN1dt * y_nodes[0] + dN2dt * y_nodes[1] + dN3dt * y_nodes[2] + dN4dt * y_nodes[3];
        double det = dx_ds * dy_dt - dy_ds * dx_dt;
        if (qFuzzyIsNull(det)) {
            qDebug() << "警告: 雅可比矩阵行列式接近零，迭代可能不收敛";
            break;
        }
        double J_inv[2][2] = {
            {dy_dt / det, -dy_ds / det},
            {-dx_dt / det, dx_ds / det}
        };
        double delta_s = J_inv[0][0] * (x_para - x_est) + J_inv[0][1] * (y_para - y_est);
        double delta_t = J_inv[1][0] * (x_para - x_est) + J_inv[1][1] * (y_para - y_est);
        s += delta_s;
        t += delta_t;
        s = qBound(-1.0, s, 1.0);
        t = qBound(-1.0, t, 1.0);
    }

    double N1 = 0.25 * (1 - s) * (1 - t);
    double N2 = 0.25 * (1 + s) * (1 - t);
    double N3 = 0.25 * (1 + s) * (1 + t);
    double N4 = 0.25 * (1 - s) * (1 + t);
    double u_val = N1 * u_[0] + N2 * u_[5] + N3 * u_[10] + N4 * u_[15];
    double v_val = N1 * u_[1] + N2 * u_[6] + N3 * u_[11] + N4 * u_[16];
    double w_val = N1 * u_[2] + N2 * u_[7] + N3 * u_[12] + N4 * u_[17];
    double theta_x = N1 * u_[3] + N2 * u_[8] + N3 * u_[13] + N4 * u_[18];
    double theta_y = N1 * u_[4] + N2 * u_[9] + N3 * u_[14] + N4 * u_[19];
    const double h = 0.01; // 假设h的值为1，原代码未定义

    disp[0] = u_val + h * theta_y;
    disp[1] = v_val - h * theta_x;
    disp[2] = w_val;
}

void IFE::ShapeFunction_New_ZB(double x_para, double y_para, double z_para, double node_u[22], double *disp)
{
    x_para /= 1000.0;
    y_para /= 1000.0;
    z_para /= 1000.0;

    const double total_L = 1.4;
    const double L = total_L / 2.0;

    disp[0] = 0.0;
    disp[1] = 0.0;
    disp[2] = 0.0;

    if (x_para < 0.0 || x_para > total_L) {
        return;
    }

    double u_[14] = {0.0};
    double x_left = 0.0;

    // 第一个梁单元：[0, 0.7]
    if (x_para <= L) {
        for (int i = 0; i < 14; ++i) {
            u_[i] = node_u[i];
        }
    }
    // 第二个梁单元：[0.7, 1.4]
    else {
        x_left = L;

        // 第二单元左端 = 第一单元右端，6 个共享自由度
        for (int i = 0; i < 6; ++i) {
            u_[i] = node_u[i + 8];
        }

        // 第二单元段内自由度：[uz_mid, uy_mid]
        u_[6] = node_u[14];
        u_[7] = node_u[15];

        // 第二单元右端 6 个自由度
        for (int i = 0; i < 6; ++i) {
            u_[i + 8] = node_u[i + 16];
        }
    }

    // 当前点在所选单元内的归一化坐标：左端 -1，右端 +1
    const double x = 2.0 * (x_para - x_left) / L - 1.0;

    const double L1[2] = {
        0.5 * (1.0 - x),
        0.5 * (1.0 + x)
    };

    const double L2[3] = {
        0.5 * x * (x - 1.0),
        1.0 - x * x,
        0.5 * x * (x + 1.0)
    };

    const double N3[3] = {
        L / 24.0 * (1.0 - x * x) * (2.0 * x - 3.0),
        L / 24.0 * (1.0 - x * x) * (-4.0 * x),
        L / 24.0 * (1.0 - x * x) * (2.0 * x + 3.0)
    };

    // shapefun 的前 3 行
    const double u_val = L1[0] * u_[0] + L1[1] * u_[8];

    const double v_val =
        L1[0] * u_[1] - N3[0] * u_[5] - N3[1] * u_[7] +
        L1[1] * u_[9] - N3[2] * u_[13];

    const double w_val =
        L1[0] * u_[2] + N3[0] * u_[4] + N3[1] * u_[6] +
        L1[1] * u_[10] + N3[2] * u_[12];

    // shapefun 的后 3 行
    const double theta_x = L1[0] * u_[3] + L1[1] * u_[11];

    const double theta_y =
        L2[0] * u_[4] + L2[1] * u_[6] + L2[2] * u_[12];

    const double theta_z =
        L2[0] * u_[5] + L2[1] * u_[7] + L2[2] * u_[13];

    // AA * [u, v, w, theta_x, theta_y, theta_z]^T
    disp[0] = u_val + z_para * theta_y - y_para * theta_z;
    disp[1] = v_val - z_para * theta_x;
    disp[2] = w_val + y_para * theta_x;
}

int IFE::getUnit(double x, double y)
{
    if(x<0.7 && x >= 0){
        return 1;
    }
    else if(x >= 0.4){
        return 2;
    }
    return -1;
}

void IFE::loadConfig(const QString &filePath)
{
    if(filePath.isEmpty() == false)
    {
        QSettings settings(filePath, QSettings::IniFormat);
        // 读取 order
        for (int i = 0; i < SensorNum; i++) {
            order[i] = settings.value(QString("order_%1").arg(i)).toInt();
        }
        // 读取 para
        for (int i = 0; i < SensorNum; i++) {
            para[i] = settings.value(QString("para_%1").arg(i)).toDouble();
        }
        for (int i = 0; i < 3; i++) {
            axis[i] = settings.value(QString("axis_%1").arg(i)).toDouble();
        }
    }
}

void IFE::saveConfig(const QString &filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);
    // 保存 order
    for (int i = 0; i < SensorNum; i++) {
        settings.setValue(QString("order_%1").arg(i), order[i]);
    }
    // 保存 para
    for (int i = 0; i < SensorNum; i++) {
        settings.setValue(QString("para_%1").arg(i), para[i]);
    }
    for (int i = 0; i < 3; i++) {
        settings.setValue(QString("axis_%1").arg(i), axis[i]);
    }
}

#include "global.h"
void IFE::calculateDisp()
{
    //传递传感器应变值
    QMutexLocker locker(&mutexStrain);
    for (int i = 0; i < SensorNum; ++i) {
        sortedstrain[i] = strain[order[i] - 1] * static_cast<float>(para[i]);
    }

//    sortedstrain[0] = 6.6519e-004;
//    sortedstrain[1] = 4.0582e-005;
//    sortedstrain[2] = -6.11E-05;
//    sortedstrain[3] = -5.42E-04;
//    sortedstrain[4] = 7.6332e-005;
//    sortedstrain[5] = 6.6399e-005;

//    sortedstrain[6] = 1.518e-005;
//    sortedstrain[7] = -8.63E-05;
//    sortedstrain[8] = -2.99E-04;
//    sortedstrain[9] = -3.53E-05;
//    sortedstrain[10] = -8.00E-06;
//    sortedstrain[11] = 1.4629e-004;

    //乘以类刚度矩阵
    for (int i = 0; i < 22; i++){
        node_u[i] = 0;
        for (int j = 0; j < 12; j++){
            node_u[i] += aa_k_New[i][j] * sortedstrain[j];
        }
    }
}
