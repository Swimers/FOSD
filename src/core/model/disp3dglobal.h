#ifndef DISP3DGLOBAL_H
#define DISP3DGLOBAL_H

#include <QVector>
#include <vtkSmartPointer.h>
#include <vtkDoubleArray.h>
#include <vtkTextActor.h>
#include <vtkActor.h>

typedef struct SelectedPointEntity{
    vtkTextActor* keyPointText;
    vtkActor* keyPointEntity;
}SelectedPointEntity;

struct ModelScalars
{
    vtkSmartPointer<vtkDoubleArray> dispX;   // X方向位移
    vtkSmartPointer<vtkDoubleArray> dispY;   // Y方向位移
    vtkSmartPointer<vtkDoubleArray> dispZ;   // Z方向位移
    vtkSmartPointer<vtkDoubleArray> strain;  // 应变
    vtkSmartPointer<vtkDoubleArray> temp;    // 温度

    void Init()
    {
        dispX = vtkSmartPointer<vtkDoubleArray>::New();
        dispY = vtkSmartPointer<vtkDoubleArray>::New();
        dispZ = vtkSmartPointer<vtkDoubleArray>::New();
        strain = vtkSmartPointer<vtkDoubleArray>::New();
        temp = vtkSmartPointer<vtkDoubleArray>::New();

        dispX->SetName("Displacement_X");
        dispY->SetName("Displacement_Y");
        dispZ->SetName("Displacement_Z");
        strain->SetName("Strain");
        temp->SetName("Temperature");
    }
};

#endif // DISP3DGLOBAL_H
