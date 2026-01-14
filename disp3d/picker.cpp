#include "picker.h"
#include"vtkAutoInit.h"
#include "vtkRenderWindowInteractor.h"
#include"vtkPointPicker.h"
#include"vtkRendererCollection.h"
#include"vtkRenderWindow.h"
#include"vtkSphereSource.h"
#include"vtkPolyDataMapper.h"
#include"SelfDefineData.h"
#include"vtkTextActor.h"
#include"vtkTextProperty.h"
#include"vtkProperty.h"
#include <QDebug>
#include <QString>

//#include"vtkAbstractPicker.h"
//VTK_MODULE_INIT(vtkRenderingFreeType);
#pragma execution_character_set("utf-8")


vtkStandardNewMacro(PointPickerInteractorStyle);
int * pickpointIDset = nullptr;
vtkActor**pointactor;
int pickernumber = 0;

void PointPickerInteractorStyle::setGrid(vtkUnstructuredGrid* Grid)
{
    this->Grid = Grid;
    int number = Grid->GetNumberOfPoints();
    pickpointIDset = new int[number];
    pointactor =new vtkActor*[number];
}

vtkUnstructuredGrid* PointPickerInteractorStyle::getGrid()
{
    return this->Grid;
}

void PointPickerInteractorStyle::setpickflag(bool flag)
{
    pickflag = flag;
}

DisplyNodeInfo PointPickerInteractorStyle::GetPickPointInfo()
{
    double(*point)[4] = new double[pickernumber][4];
    vtkPoints * points = this->Grid->GetPoints();
    for (int i = 0; i < pickernumber; i++)
    {
        point[i][0] = pickpointIDset[i];
        point[i][1] = points->GetPoint(pickpointIDset[i])[0];
        point[i][2] = points->GetPoint(pickpointIDset[i])[1];
        point[i][3] = points->GetPoint(pickpointIDset[i])[2];
    }
    return point;
}

//鼠标左键
void PointPickerInteractorStyle::OnLeftButtonDown()
 {
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

    int* pos = this->GetInteractor()->GetEventPosition();
    vtkPointPicker * pick1 = vtkPointPicker::New();
    //pick1->SetTolerance(0.005);
    pick1->SetTolerance(0.005);
    pick1->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
    char text[10] = { '\0'};
    if (pick1->GetPointId() != -1)
    {
        vtkIdType ID = pick1->GetPointId()+1;
        if (displaymodel->SolidModeNodePointToFlagPoint.find(ID)!= displaymodel->SolidModeNodePointToFlagPoint.end())
        {
            worldPoint[0] = displaymodel->new_node_info1[ID]->x_cord;
            worldPoint[1] = displaymodel->new_node_info1[ID]->y_cord;
            worldPoint[2] = displaymodel->new_node_info1[ID]->z_cord;
            double worldPoint1[3];
            worldPoint1[0] = worldPoint[0] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[ID]->xdisplace;
            worldPoint1[1] = worldPoint[1] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[ID]->ydisplace;
            worldPoint1[2] = worldPoint[2] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[ID]->zdisplace;
            OldID = ID;
            double DisplayCord[3];
            this->GetDefaultRenderer()->SetWorldPoint(worldPoint1[0], worldPoint1[1], worldPoint1[2], 1.0);
            this->GetDefaultRenderer()->WorldToDisplay();
            this->GetDefaultRenderer()->GetDisplayPoint(DisplayCord);

            sprintf(text,"%d", displaymodel->SolidModeNodePointToFlagPoint[ID]);
            //创建文本
            //actor->SetDisplayPosition(pos[0], pos[1]);
            actor->SetDisplayPosition(DisplayCord[0], DisplayCord[1]);
            //actor->SetPosition(displaymodel->new_node_info1[ID]->x_cord,displaymodel->new_node_info1[ID]->y_cord,displaymodel->new_node_info1[ID]->z_cord);
            actor->SetInput(text);
            actor->GetTextProperty()->SetColor(1, 0, 0);
            actor->GetTextProperty()->SetFontSize(20);
            this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(actor);

            //创建小球
            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetRadius(0.003);
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
                mapper->SetInputConnection(sphereSource->GetOutputPort());
            actor_sphere->SetMapper(mapper);
            actor_sphere->SetPosition(displaymodel->new_node_info1[ID]->x_cord,displaymodel->new_node_info1[ID]->y_cord,displaymodel->new_node_info1[ID]->z_cord);
            this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor_sphere);

            this->GetInteractor()->GetRenderWindow()->Render();

            QString str0 = QString("当前拾取点: %1").arg(displaymodel->SolidModeNodePointToFlagPoint[ID]);
            QString str1 = QString("初始坐标:(%1, %2, %3)")
                               .arg(QString::number(worldPoint[0]*1000, 'f', 2))
                               .arg(QString::number(worldPoint[1]*1000, 'f', 2))
                               .arg(QString::number(worldPoint[2]*1000, 'f', 2));

            QString str2 = QString("变形量:(%1, %2, %3)")
                               .arg(QString::number(displaymodel->PointDisplaceSet[ID]->xdisplace*1000, 'f', 2))
                               .arg(QString::number(displaymodel->PointDisplaceSet[ID]->ydisplace*1000, 'f', 2))
                               .arg(QString::number(displaymodel->PointDisplaceSet[ID]->zdisplace*1000, 'f', 2));

            QString str3 = QString("当前坐标:(%1, %2, %3)")
                               .arg(QString::number(worldPoint[0]*1000+displaymodel->PointDisplaceSet[ID]->xdisplace*1000, 'f', 2))
                               .arg(QString::number(worldPoint[1]*1000+displaymodel->PointDisplaceSet[ID]->ydisplace*1000, 'f', 2))
                               .arg(QString::number(worldPoint[2]*1000+displaymodel->PointDisplaceSet[ID]->zdisplace*1000, 'f', 2));
            this->label0->setText(str0);
            this->label1->setText(str1);
            this->label2->setText(str2);
            this->label3->setText(str3);

            pickflag = true;
        }
        else
        {
            if(pickflag == true)
            {
                this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor2D(actor);
                this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(actor_sphere);
                this->GetInteractor()->GetRenderWindow()->Render();
                pickflag = false;
            }
        }
    }
}

void PointPickerInteractorStyle::OnRightButtonDown()
{
    // 调用基类的右键按下事件处理函数
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();

    // 在这里添加您的自定义右键按下事件处理代码
    if(pickflag == true)
    {
        this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor2D(actor);
        this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(actor_sphere);

        QString str0 = (QString)"当前拾取点:";
        QString str1 = (QString)"初始坐标:(*,*,*)";
        QString str2 = (QString)"变形量:(*,*,*)";
        QString str3 = (QString)"当前坐标:(*,*,*)";
        this->label0->setText(str0);
        this->label1->setText(str1);
        this->label2->setText(str2);
        this->label3->setText(str3);

        pickflag = false;
    }
}
//鼠标移动
void PointPickerInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}
//按键按下
void PointPickerInteractorStyle::OnKeyDown()
{
/*	CFrameWnd* pFram = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
    CView * view1 = pFram->GetActiveFrame()->GetActiveView();
    view1->SendMessage(WM_USER_SHEZHI, (WPARAM)"hehh", NULL);
    //vtkInteractorStyleTrackballCamera::OnMouseMove();*/
    vtkInteractorStyleTrackballCamera::OnKeyDown();

}
//按键弹回
void PointPickerInteractorStyle::OnKeyUp()
{
/*	CFrameWnd* pFram = STATIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
    CView * view1 = pFram->GetActiveFrame()->GetActiveView();
    view1->SendMessage(WM_USER_SHEZHI, (WPARAM)"", NULL);
    //vtkInteractorStyleTrackballCamera::OnMouseMove();*/
    vtkInteractorStyleTrackballCamera::OnKeyUp();

}

double PointPickerInteractorStyle::caculatelength(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
}

double* PointPickerInteractorStyle::caculatePos(double x, double y, double z)
{
    double point[3];
    double * minpointcord = new double[4];
    //int nodenum;
    vtkPoints *points = Grid->GetPoints();
    int pointnum = points->GetNumberOfPoints();
    double minlength = 100000.0;
    for (int i = 0; i < pointnum; i++)
    {
        if ((!findpointID(i))) //先判断当前的点有没有被拾取，如果有直接跳过，如果没有，参与比较
        {

            point[0] = points->GetPoint(i)[0];
            point[1] = points->GetPoint(i)[1];
            point[2] = points->GetPoint(i)[2];
            double lengh = caculatelength(point[0], point[1], point[2], x, y, z);
            if (lengh - minlength<0)
            {
                minlength = lengh;
                minpointcord[0] = point[0];
                minpointcord[1] = point[1];
                minpointcord[2] = point[2];
                minpointcord[3] = i;
            }
        }
    }
    return minpointcord;
}

bool PointPickerInteractorStyle::findpointID(int number)
{
    bool found = false;
    for (int i = 0; i<pickernumber&&!found; i++)
    {
        if (pickpointIDset[i]==number)
        {
            found = true;
        }
    }
    return found;
}

void PointPickerInteractorStyle::refreshPickPointText()
{
    if(pickflag)
    {
        double DisplayCord[3];
        double worldPoint1[3];
        worldPoint1[0] = worldPoint[0] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[OldID]->xdisplace;
        worldPoint1[1] = worldPoint[1] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[OldID]->ydisplace;
        worldPoint1[2] = worldPoint[2] + displaymodel->Displacefactor*displaymodel->PointDisplaceSet[OldID]->zdisplace;
        this->GetDefaultRenderer()->SetWorldPoint(worldPoint1[0], worldPoint1[1], worldPoint1[2], 1.0);
        this->GetDefaultRenderer()->WorldToDisplay();
        this->GetDefaultRenderer()->GetDisplayPoint(DisplayCord);
        actor->SetDisplayPosition(DisplayCord[0], DisplayCord[1]);
        actor_sphere->SetPosition(worldPoint1[0],worldPoint1[1],worldPoint1[2]);


        QString str0 = QString("当前拾取点: %1").arg(displaymodel->SolidModeNodePointToFlagPoint[OldID]);
        QString str1 = QString("初始坐标:(%1, %2, %3)")
                           .arg(QString::number(worldPoint[0]*1000, 'f', 2))
                           .arg(QString::number(worldPoint[1]*1000, 'f', 2))
                           .arg(QString::number(worldPoint[2]*1000, 'f', 2));
        QString str2 = QString("变形量:(%1, %2, %3)")
                           .arg(QString::number(displaymodel->PointDisplaceSet[OldID]->xdisplace*1000, 'f', 2))
                           .arg(QString::number(displaymodel->PointDisplaceSet[OldID]->ydisplace*1000, 'f', 2))
                           .arg(QString::number(displaymodel->PointDisplaceSet[OldID]->zdisplace*1000, 'f', 2));
        QString str3 = QString("当前坐标:(%1, %2, %3)")
                           .arg(QString::number(worldPoint[0]*1000+displaymodel->PointDisplaceSet[OldID]->xdisplace*1000, 'f', 2))
                           .arg(QString::number(worldPoint[1]*1000+displaymodel->PointDisplaceSet[OldID]->ydisplace*1000, 'f', 2))
                           .arg(QString::number(worldPoint[2]*1000+displaymodel->PointDisplaceSet[OldID]->zdisplace*1000, 'f', 2));
        this->label0->setText(str0);
        this->label1->setText(str1);
        this->label2->setText(str2);
        this->label3->setText(str3);

    }
}
//旋转
void PointPickerInteractorStyle::Rotate()
{
     vtkInteractorStyleTrackballCamera::Rotate();

     refreshPickPointText();
     this->displaymodel->refreshFlagPointTextPoisiton();


     this->Interactor->GetRenderWindow()->Render();
}

void PointPickerInteractorStyle::Spin()
 {
     vtkInteractorStyleTrackballCamera::Spin();

     refreshPickPointText();
     this->displaymodel->refreshFlagPointTextPoisiton();

     this->Interactor->GetRenderWindow()->Render();

 }
//相机平移，滚轮平移
void PointPickerInteractorStyle::Pan()
{
    vtkInteractorStyleTrackballCamera::Pan();


    refreshPickPointText();
    this->displaymodel->refreshFlagPointTextPoisiton();

    this->Interactor->GetRenderWindow()->Render();
}


//相机拉伸，右键拉伸
void PointPickerInteractorStyle::Dolly()
 {
     vtkInteractorStyleTrackballCamera::Dolly();


     refreshPickPointText();
     this->displaymodel->refreshFlagPointTextPoisiton();

     this->Interactor->GetRenderWindow()->Render();
 }

//滚轮向前
void PointPickerInteractorStyle::OnMouseWheelForward()
 {
     vtkInteractorStyleTrackballCamera::OnMouseWheelForward();

     refreshPickPointText();
     this->displaymodel->refreshFlagPointTextPoisiton();

     this->Interactor->GetRenderWindow()->Render();
 }


//滚轮向后
void  PointPickerInteractorStyle::OnMouseWheelBackward()
 {
     vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();

    refreshPickPointText();
    this->displaymodel->refreshFlagPointTextPoisiton();

     this->Interactor->GetRenderWindow()->Render();
 }

