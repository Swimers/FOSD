#include "disp3dpointpicker.h"
#include <QDebug>
#include <QString>

#include <vtkAutoInit.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>

#include "disp3ddeginedata.h"

disp3dpointpicker::disp3dpointpicker()
{

}

disp3dpointpicker::~disp3dpointpicker()
{

}

disp3dpointpicker *disp3dpointpicker::New()
{
    return new disp3dpointpicker;
}

void disp3dpointpicker::refreshPickPointText()
{
    if(pickflag){
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

