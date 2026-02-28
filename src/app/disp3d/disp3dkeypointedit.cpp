#include "disp3dkeypointedit.h"
#include "ui_disp3dkeypointedit.h"

#include <QString>
#include <QDebug>

#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

// 静态成员初始化
disp3dkeypointedit* disp3dkeypointedit::m_instance = nullptr;

disp3dkeypointedit::disp3dkeypointedit(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::disp3dkeypointedit)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);  //关闭时删除对象

    ui->pbtn_SelectCube->setEnabled(false);
    ui->pbtn_ClearCube->setEnabled(false);

    EditInit();
}

disp3dkeypointedit::~disp3dkeypointedit()
{
    ClearDisp();

    delete ui;
    m_instance = nullptr;  // 销毁时清空单例指针
}

void disp3dkeypointedit::EditInit()
{
    QIntValidator *intValidator = new QIntValidator(this);  //整数过滤
    ui->edit_IDKeyPoint->setValidator(intValidator);
    QDoubleValidator *doubleValidator = new QDoubleValidator(this); //浮点数过滤
    ui->edit_Xmax->setValidator(doubleValidator);
    ui->edit_Ymax->setValidator(doubleValidator);
    ui->edit_Zmax->setValidator(doubleValidator);
    ui->edit_Xmin->setValidator(doubleValidator);
    ui->edit_Ymin->setValidator(doubleValidator);
    ui->edit_Zmin->setValidator(doubleValidator);
}

disp3dkeypointedit* disp3dkeypointedit::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new disp3dkeypointedit(parent);
    }
    return m_instance;
}

void disp3dkeypointedit::UpDataKeyInfor()
{
    ui->lab_NumPoint->setText(QString::number(m_disp3dmodel->GetModelNumPoints()));
    ui->lab_NumKeyPoint->setText(QString::number(m_disp3dmodel->GetModelNumKeyPoints()));

    ui->cbox_IDKeyPoint->clear();
    QList<vtkIdType> idList = m_disp3dmodel->keyPointSet.values();
    std::sort(idList.begin(), idList.end());
    for (vtkIdType id : idList){
        QString idStr = QString::number(id);
        ui->cbox_IDKeyPoint->addItem(idStr);
        ui->cbox_IDKeyPoint->currentIndexChanged(0);
    }
}

void disp3dkeypointedit::UpDataKeyPosition(vtkIdType id)
{
    QVector<double> coord = m_disp3dmodel->rawPointMap.value(id);
    ui->lab_XPosition->setText(QString::number(coord[0]));
    ui->lab_YPosition->setText(QString::number(coord[1]));
    ui->lab_ZPosition->setText(QString::number(coord[2]));
}

void disp3dkeypointedit::ClearDisp()
{
    auto renderer = m_disp3dmodel->GetVTKRenderer();
    if (cubeActor){
        renderer->RemoveActor(cubeActor);
        cubeActor = nullptr;
    }
    renderer->AddActor(cubeActor);
    m_disp3dmodel->GetVTKRenderWindow()->Render();

    ui->pbtn_SelectCube->setEnabled(false);
    ui->pbtn_ClearCube->setEnabled(false);
}

void disp3dkeypointedit::on_cbox_IDKeyPoint_currentIndexChanged(const QString &arg1)
{
    vtkIdType id = arg1.toInt();
    UpDataKeyPosition(id);
}

void disp3dkeypointedit::on_pbtn_FindKeyPoint_clicked()
{
    vtkIdType idstr = ui->edit_IDKeyPoint->text().toLongLong();
    if(m_disp3dmodel->keyPointSet.contains(idstr)){
        ui->cbox_IDKeyPoint->setCurrentText(ui->edit_IDKeyPoint->text());
    }
    else {
        ui->edit_IDKeyPoint->clear();
    }
}

void disp3dkeypointedit::on_pbtn_SelectAll_clicked()
{
    for (auto it = m_disp3dmodel->rawPointMap.constBegin(); it != m_disp3dmodel->rawPointMap.constEnd(); ++it){
        vtkIdType id = it.key();
        m_disp3dmodel->keyPointSet.insert(id);
    }
    UpDataKeyInfor();
    emit SelectSuccess();
}

void disp3dkeypointedit::on_pbtn_ClearAll_clicked()
{
    m_disp3dmodel->keyPointSet.clear();
    UpDataKeyInfor();
    ui->lab_XPosition->clear();
    ui->lab_YPosition->clear();
    ui->lab_ZPosition->clear();
    emit SelectSuccess();
}

void disp3dkeypointedit::on_pbtn_DispCube_clicked()
{
    CubePoint[0] = ui->edit_Xmax->text().toDouble();
    CubePoint[1] = ui->edit_Ymax->text().toDouble();
    CubePoint[2] = ui->edit_Zmax->text().toDouble();
    CubePoint[3] = ui->edit_Xmin->text().toDouble();
    CubePoint[4] = ui->edit_Ymin->text().toDouble();
    CubePoint[5] = ui->edit_Zmin->text().toDouble();

    auto renderer = m_disp3dmodel->GetVTKRenderer();
    if (cubeActor){
        renderer->RemoveActor(cubeActor);
        cubeActor = nullptr;
    }
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetBounds(
        CubePoint[3], CubePoint[0], // xmin, xmax
        CubePoint[4], CubePoint[1], // ymin, ymax
        CubePoint[5], CubePoint[2]  // zmin, zmax
    );

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cubeSource->GetOutputPort());
    cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(mapper);
    cubeActor->GetProperty()->SetColor(1, 0, 0);
    cubeActor->GetProperty()->SetOpacity(0.4);
    cubeActor->GetProperty()->SetRepresentationToWireframe();
    renderer->AddActor(cubeActor);
    m_disp3dmodel->GetVTKRenderWindow()->Render();

    ui->pbtn_ClearCube->setEnabled(true);
    ui->pbtn_SelectCube->setEnabled(true);
}

void disp3dkeypointedit::on_pbtn_SelectCube_clicked()
{
    for (auto it = m_disp3dmodel->rawPointMap.constBegin(); it != m_disp3dmodel->rawPointMap.constEnd(); ++it){
        vtkIdType id = it.key();
        const QVector<double> &coord = it.value();
        double x = coord[0], y = coord[1], z = coord[2];
        if (x >= CubePoint[3] && x <= CubePoint[0] &&
            y >= CubePoint[4] && y <= CubePoint[1] &&
            z >= CubePoint[5] && z <= CubePoint[2]){
            m_disp3dmodel->keyPointSet.insert(id);
        }
    }
    UpDataKeyInfor();
    ui->pbtn_SelectCube->setEnabled(false);
    ui->pbtn_ClearCube->setEnabled(false);
    emit SelectSuccess();
}

void disp3dkeypointedit::on_pbtn_ClearCube_clicked()
{
    for (auto it = m_disp3dmodel->rawPointMap.constBegin(); it != m_disp3dmodel->rawPointMap.constEnd(); ++it){
        vtkIdType id = it.key();
        const QVector<double> &coord = it.value();
        double x = coord[0], y = coord[1], z = coord[2];
        if (x >= CubePoint[3] && x <= CubePoint[0] &&
            y >= CubePoint[4] && y <= CubePoint[1] &&
            z >= CubePoint[5] && z <= CubePoint[2]){
            m_disp3dmodel->keyPointSet.remove(id);
        }
    }
    UpDataKeyInfor();
    ui->pbtn_SelectCube->setEnabled(false);
    ui->pbtn_ClearCube->setEnabled(false);
    emit SelectSuccess();
}

void disp3dkeypointedit::on_pushButton_clicked()
{
    ClearDisp();
}
