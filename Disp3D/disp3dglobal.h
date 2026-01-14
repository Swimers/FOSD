#ifndef DISP3DGLOBAL_H
#define DISP3DGLOBAL_H

#include <QVector>
#include <vtkTextActor.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkFloatArray.h>

typedef struct SelectedPointEntity{
    vtkTextActor* keyPointText;
    vtkActor* keyPointEntity;
}SelectedPointEntity;

struct ModelScalars
{
    vtkSmartPointer<vtkFloatArray> dispX;   // X方向位移
    vtkSmartPointer<vtkFloatArray> dispY;   // Y方向位移
    vtkSmartPointer<vtkFloatArray> dispZ;   // Z方向位移
    vtkSmartPointer<vtkFloatArray> strain;  // 应变
    vtkSmartPointer<vtkFloatArray> temp;    // 温度

    void Init()
    {
        dispX = vtkSmartPointer<vtkFloatArray>::New();
        dispY = vtkSmartPointer<vtkFloatArray>::New();
        dispZ = vtkSmartPointer<vtkFloatArray>::New();
        strain = vtkSmartPointer<vtkFloatArray>::New();
        temp = vtkSmartPointer<vtkFloatArray>::New();

        dispX->SetName("Displacement_X");
        dispY->SetName("Displacement_Y");
        dispZ->SetName("Displacement_Z");
        strain->SetName("Strain");
        temp->SetName("Temperature");
    }
};


#endif // DISP3DGLOBAL_H
