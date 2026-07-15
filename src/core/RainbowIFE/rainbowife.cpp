#include "rainbowIFE.h"

#include "global.h"

#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <qthread.h>
#include <QDir>
#include <QTextStream>
#include <QStringList>
#include <QDateTime>
#include <QTime>

#include "global.h"

/*
 * 传入文件地址，将数据导入positionOriginal中
 */
rainbowIFE::rainbowIFE(QObject *parent) : QObject(parent){
    loadConfig("ifepara.ini");
    startStorageThread();  // 启动存储线程
}

rainbowIFE::~rainbowIFE() {
    stopStorageThread();  // 停止存储线程
}

void rainbowIFE::startStorageThread() {
    if (m_storageThread == nullptr) {
        m_storageThread = new DataStorageThread(this);

        // 设置存储路径
        QByteArray patharray = qgetenv("FOSD_XD_PATH");
        QString path = QString::fromLocal8Bit(patharray);

        //测试存储路径
        path = "data";

        if (!path.isEmpty()) {
            m_storageThread->setStoragePath(path);
        }

        m_storageThread->start();
        qDebug() << "IFE: 数据存储线程已启动";
    }
}

void rainbowIFE::stopStorageThread() {
    if (m_storageThread != nullptr) {
        m_storageThread->stopThread();
        delete m_storageThread;
        m_storageThread = nullptr;
        qDebug() << "IFE: 数据存储线程已停止";
    }
}

int rainbowIFE::getStorageQueueSize() const {
    if (m_storageThread != nullptr) {
        return m_storageThread->queueSize();
    }
    return 0;
}

void rainbowIFE::modifyOriginalCoordinates(const QString &filePath)
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

void rainbowIFE::shapefunction(double x_para,double y_para,double z_para,double node_u[16],double* disp){
    // x_para = 6732.1;
    // y_para = 180.8;
    // z_para = 9384.5;


    double R_inv_left[3][3] = {0.374603810000000,	0.00780956100000000,	0.927152100000000,
                            0.914273800000000,	0.166287510000000,	-0.369393890000000,
                            -0.157058618217253,	0.986047232229701,	0.0551518577896113};
    double R_inv_right[3][3] = {0.374603810000000,	0.00780956100000000,	-0.927152100000000,
0.914273800000000,	0.166287510000000,	0.369393890000000,
0.157058618217253,	-0.986047232229701,	0.0551518577896113};

    //****临时修改，限制-2300< z <2300 的点不运算
    if((z_para >= -2300) && (z_para <= 2300)){
        disp[0] = 0;
        disp[1] = 0;
        disp[2] = 0;
        return;
    }


    // double node_u_beam[14] = {0,
    //                0,
    //                0,
    //                0,
    //                0,
    //                0,
    //                -0.0595761161950408,
    //                0.00939891245511731,
    //                -2.61985737447772,
    //                57.8432949532917,
    //                343.483672902083,
    //                0.00351101568035298,
    //                -0.106903245584043,
    //                0.0169781110079015};
//    qDebug() << x_para << y_para << z_para;
    if (z_para > 0) {

        for (int i = 0; i < 8; i++) {
            node_u_beam[i+6] = node_u[8+i];
        }

        double local_origin[3] = {5023.0923, 117.13585, 6019.1089};

        double offset[3] = {x_para - local_origin[0], y_para - local_origin[1], z_para - local_origin[2]};

        double local_corrds[3] = {0};

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                local_corrds[i] += R_inv_left[i][j] * offset[j];
            }
        }

        if (local_corrds[2] > 0) {
            local_corrds[0] = (local_corrds[0]+2800)*2/5970-1;
        } else {
            local_corrds[0] = (local_corrds[0]+2710)*2/5970-1;
        }

        ShapeFunction_Beam(local_corrds[0], local_corrds[1], local_corrds[2], 5970.0, node_u_beam, disp);

        disp[0] = disp[1]/1000;
        disp[1] = disp[2]/1000;
        disp[2] = disp[0]/1000;
//        qDebug() << disp[0] << disp[1] << disp[2];
    } else {

        for (int i = 0; i < 8; i++) {
            node_u_beam[i+6] = node_u[i];
        }

        double local_origin[3] = {5023.0923, 117.13585, -6019.1089};

        double offset[3] = {x_para - local_origin[0], y_para - local_origin[1], z_para - local_origin[2]};

        double local_corrds[3] = {0};

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                local_corrds[i] += R_inv_right[i][j] * offset[j];
            }
        }

        if (local_corrds[2] > 0) {
            local_corrds[0] = (local_corrds[0]+2900)*2/5970-1;
        } else {
            local_corrds[0] = (local_corrds[0]+2610)*2/5970-1;
        }


        ShapeFunction_Beam(local_corrds[0], local_corrds[1], local_corrds[2], 5970, node_u_beam, disp);

//        qDebug() << disp[0] << disp[1] << disp[2];
//        z_para = -z_para;

//        double local_origin[3] = {5023.0923, 117.13585, 6019.1089};

//        double offset[3] = {x_para - local_origin[0], y_para - local_origin[1], z_para - local_origin[2]};

//        double local_corrds[3] = {0};

//        for (int i = 0; i < 3; ++i) {
//            for (int j = 0; j < 3; ++j) {
//                local_corrds[i] += R_inv_left[i][j] * offset[j];
//            }
//        }

//        if (local_corrds[2] > 0) {
//            local_corrds[0] = (local_corrds[0]+2900)*2/5970-1;
//        } else {
//            local_corrds[0] = (local_corrds[0]+2610)*2/5970-1;
//        }


//        ShapeFunction_Beam(local_corrds[0], local_corrds[1], local_corrds[2], 5970, node_u_beam, disp);

//        disp[2] = -disp[2];

        disp[0] = disp[1]/1000;
     disp[1] = disp[2]/1000;
     disp[2] = disp[0]/1000;
    }

//            disp[0] = disp[1]/1000;
//         disp[1] = disp[2]/1000;
//         disp[2] = disp[0]/1000;

//         disp[0] = disp[1];
//      disp[1] = disp[2];
//      disp[2] = disp[0];

}

void rainbowIFE::N_shapefunction(double x_para,double y_para,double z_para,double node_u[8],double* disp)//加入单元长度，将mn和xy建立等参关系
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

void rainbowIFE::ShapeFunction_New(double x_para, double y_para, double node_u[20], double *disp)
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

void rainbowIFE::ShapeFunction_Beam(double x_coord, double y_coord, double z_coord, double len, double node_u[14], double *disp)
{
    // 将物理坐标转换为局部坐标 x ∈ [-1, 1]
//    double x = 2.0 * x_coord / len - 1.0;
    double x = x_coord;
    double y11 = y_coord;
    double z11 = z_coord;
    double L = len;

    // 预计算常用项
    double x2 = x * x;
    double x2_minus_1 = x2 - 1.0;
    double half_minus_x_half = 0.5 - x / 2.0;
    double x_half_plus_half = x / 2.0 + 0.5;
    double x_minus_1 = x - 1.0;
    double x_plus_1 = x + 1.0;
    double two_x_minus_3 = 2.0 * x - 3.0;
    double two_x_plus_3 = 2.0 * x + 3.0;

    // 形函数矩阵 shapefun (3x14)
    // 第一行: u方向位移
    double shape_u[14] = {
        half_minus_x_half,                           // N1
        0,                                           // N2
        0,                                           // N3
        0,                                           // N4
        (x * z11 * x_minus_1) / 2.0,                // N5
        -(x * y11 * x_minus_1) / 2.0,               // N6
        -z11 * x2_minus_1,                          // N7
        y11 * x2_minus_1,                           // N8
        x_half_plus_half,                           // N9
        0,                                           // N10
        0,                                           // N11
        0,                                           // N12
        (x * z11 * x_plus_1) / 2.0,                 // N13
        -(x * y11 * x_plus_1) / 2.0                 // N14
    };

    // 第二行: v方向位移
    double shape_v[14] = {
        0,                                           // N1
        half_minus_x_half,                           // N2
        0,                                           // N3
        z11 * (x / 2.0 - 0.5),                      // N4
        0,                                           // N5
        (L * two_x_minus_3 * x2_minus_1) / 24.0,    // N6
        0,                                           // N7
        -(L * x * x2_minus_1) / 6.0,                // N8
        0,                                           // N9
        x_half_plus_half,                           // N10
        0,                                           // N11
        -z11 * (x / 2.0 + 0.5),                     // N12
        0,                                           // N13
        (L * two_x_plus_3 * x2_minus_1) / 24.0      // N14
    };

    // 第三行: w方向位移
    double shape_w[14] = {
        0,                                           // N1
        0,                                           // N2
        half_minus_x_half,                           // N3
        -y11 * (x / 2.0 - 0.5),                     // N4
        -(L * two_x_minus_3 * x2_minus_1) / 24.0,   // N5
        0,                                           // N6
        (L * x * x2_minus_1) / 6.0,                 // N7
        0,                                           // N8
        0,                                           // N9
        0,                                           // N10
        x_half_plus_half,                           // N11
        y11 * (x / 2.0 + 0.5),                      // N12
        -(L * two_x_plus_3 * x2_minus_1) / 24.0,    // N13
        0                                            // N14
    };

    // 计算位移: disp = shapefun * node_u
    disp[0] = 0.0;  // u
    disp[1] = 0.0;  // v
    disp[2] = 0.0;  // w

    for (int i = 0; i < 14; i++) {
        disp[0] += shape_u[i] * node_u[i];
        disp[1] += shape_v[i] * node_u[i];
        disp[2] += shape_w[i] * node_u[i];
    }
}

int rainbowIFE::getUnit(double x, double y)
{
    if(x<0.4 && x >= 0){
        return 1;
    }
    else if(x >= 0.4){
        return 2;
    }
    return -1;
}

void rainbowIFE::loadConfig(const QString &filePath)
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

void rainbowIFE::saveConfig(const QString &filePath)
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

void rainbowIFE::calculateDisp()
{
    //传递传感器应变值
    QMutexLocker locker(&mutexStrain);
    //swapFreeArray(all_data);
    limitAndZero(all_data);
    for (int i = 0; i < 16; ++i) {
        int srcIndex = (i < 8) ? (i + 8) : (i - 8);
        node_u[i] = all_data.free[srcIndex];
    }

//    QMutexLocker locker(&mutexStrain);
//    node_u[0]  = 0.0891408696770668;
//    node_u[1]  = -0.018775571137666702;
//    node_u[2]  = -8.148399353027344;
//    node_u[3]  = -101.8877182006836;
//    node_u[4]  = -318.6768798828125;
//    node_u[5]  = 0.0598243810236454;
//    node_u[6]  = -0.036320462822914124;
//    node_u[7]  = -0.020937006920576096;
//    node_u[8]  = -0.010799256153404713;
//    node_u[9]  = -0.004124469123780727;
//    node_u[10] = -2.8583545684814453;
//    node_u[11] = -25.96009063720703;
//    node_u[12] = 96.00071716308594;
//    node_u[13] = -0.028599070385098457;
//    node_u[14] = -0.05329131335020065;
//    node_u[15] = -0.007998616434633732;
}

//void rainbowIFE::getDemoData(const DWR_FOSD::NET_SEND_BUFF4 data) {

//    // 只在数据复制时持有锁，尽快释放
//    {
//        QMutexLocker locker(&strainMutex);
//        for (int i = 0; i < 12; i++){
//            all_data.strian[i] = data.strian[i];
//        }
//        for (int i = 0; i < 12; i++){
//            all_data.temper[i] = data.temper[i];
//        }
//        for (int i = 0; i < 16; i++){
//            all_data.free[i] = data.free[i];
//        }
//    }  // 锁在此释放

//    // 如果启用了存储，将数据入队到存储线程
//    if (m_storageEnabled && m_storageThread != nullptr) {
//        StorageData storageData;

//        // 复制数据
//        for (int i = 0; i < 12; i++) {
//            storageData.strian[i] = data.strian[i];
//            storageData.temper[i] = data.temper[i];
//        }
//        for (int i = 0; i < 16; i++) {
//            storageData.free[i] = data.free[i];
//        }

//        // 添加时间戳
//        storageData.timestamp = QTime::currentTime().toString("hh_mm_ss");

//        // 入队（非阻塞操作）
//        m_storageThread->enqueueData(storageData);
//    }

//    // 调试输出（可选，生产环境可注释掉以提高性能）
//    // for (int i = 0; i < 11; i++) {
//    //     qDebug() << all_data.free[i];
//    // }
//}

//测试使用接收数据
void rainbowIFE::GetDataTest(const NET_SEND_BUFF4 data)
{
    QMutexLocker locker(&mutexStrain);
    for (int i = 0; i < 12; i++){
        all_data.strian[i] = data.strian[i];
    }
    for (int i = 0; i < 12; i++){
        all_data.temper[i] = data.temper[i];
    }
    for (int i = 0; i < 16; i++){
        all_data.free[i] = data.free[i];
    }

    if (m_storageEnabled && m_storageThread != nullptr) {
        StorageData storageData;
        storageData.flag = data.flag;
        for (int i = 0; i < 12; i++) {
            storageData.strian[i] = data.strian[i];
            storageData.temper[i] = data.temper[i];
        }
        for (int i = 0; i < 16; i++) {
            storageData.free[i] = data.free[i];
        }
        storageData.timestamp = QTime::currentTime().toString("hh_mm_ss");
        m_storageThread->enqueueData(storageData);
    }

    // 调试输出（可选，生产环境可注释掉以提高性能）
    // for (int i = 0; i < 11; i++) {
    //     qDebug() << all_data.free[i];
    // }
}

void rainbowIFE::swapFreeArray(NET_SEND_BUFF4 &data)
{
    for(int i = 0; i < 8; ++i)
    {
        qSwap(data.free[i], data.free[i + 8]);
    }
}

void rainbowIFE::limitAndZero(NET_SEND_BUFF4 &data)
{

    bool exceed = false;
    for (int i = 0; i < 8; ++i) {
        if (std::fabs(data.free[i]) > threshold[i]) {
            exceed = true;
            break;
        }
    }
    if (exceed) {
        for (int i = 0; i < 8; ++i) {
            data.free[i] = 0.0;
        }
    }

    exceed = false;
    for (int i = 0; i < 8; ++i) {
        if (std::fabs(data.free[i + 8]) > threshold[i]) {
            exceed = true;
            break;
        }
    }
    if (exceed) {
        for (int i = 0; i < 8; ++i) {
            data.free[i + 8] = 0.0;
        }
    }
}
