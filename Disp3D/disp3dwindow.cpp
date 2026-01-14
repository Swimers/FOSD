#include "disp3dwindow.h"
#include "ui_disp3dwindow.h"


#include <QDebug>

Disp3DWindow::Disp3DWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Disp3DWindow)
{
    ui->setupUi(this);

    Init();

}
void Disp3DWindow::Init()
{
    displaymodel = new displayModel();
    ui->qvtkWidget->SetRenderWindow(displaymodel->GetvtkRenderWindow());               //将displaymodel的vtkRenderWindow窗口内容映射到qvtkWidget中。
    displaymodel->SetvtkRenderNumAndLayout(1);
    displaymodel->SetvtkRenderBackColor2(displayModel::rendererIndex1,0.625,0.6797,0.762,0.109, 0.1797, 0.2773);

    //MyActor* actor1=new MyActor();
    //actor1->readdata("./Data/Input/jiyi.txt");
}



Disp3DWindow::~Disp3DWindow()
{
    delete displaymodel;
    delete ui;
}


void Disp3DWindow::on_btnLoadModel_clicked()
{
    displaymodel->readData("./Data/Input/jiyi.txt");
    displaymodel->test();
}
