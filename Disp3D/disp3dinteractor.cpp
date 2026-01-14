#include "disp3dinteractor.h"
#include "disp3ddeginedata.h"

#include "vtkAutoInit.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPointPicker.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include "vtkRenderer.h"

#include <QDebug>
#include <QString>

vtkStandardNewMacro(disp3dinteractor);  //自动生成 New()方法以及一些必要的类型信息函数

disp3dinteractor::disp3dinteractor()
{
    textmapper = nullptr;
    textActor = nullptr;
    pickflag = false;
    this->actor = vtkTextActor::New();
    this->actor_sphere = vtkActor::New();
    this->actor_text = vtkVectorText::New();
}

disp3dinteractor::~disp3dinteractor()
{
<<<<<<< Updated upstream
    if (m_disp3dmodel && Flag_Pick) {
        ClearPicker();
    }

    if (m_TextActor) {
        if (m_disp3dmodel && m_disp3dmodel->GetVTKRenderer()) {
            m_disp3dmodel->GetVTKRenderer()->RemoveActor2D(m_TextActor);
        }
        m_TextActor->Delete();
        m_TextActor = nullptr;
    }
    if (m_SphereActor) {
        if (m_disp3dmodel && m_disp3dmodel->GetVTKRenderer()) {
            m_disp3dmodel->GetVTKRenderer()->RemoveActor(m_SphereActor);
        }
        m_SphereActor->Delete();
        m_SphereActor = nullptr;
    }

    m_disp3dmodel      = nullptr;
    lab_KeyPointID     = nullptr;
    lab_KeyPointRawPos = nullptr;
    lab_KeyPointDisplace = nullptr;
    lab_KeyPointNewPos   = nullptr;

    style = nullptr;
=======
    if (textmapper)
    textmapper->Delete();
    if (textActor)
    textActor -> Delete();
    if (actor_sphere)
    actor_sphere->Delete();
    this->actor->Delete();
}

void disp3dinteractor::setGrid(vtkUnstructuredGrid* Grid)
{
    this->Grid = Grid;                                                  //保存网格数据的引用
    long long number = Grid->GetNumberOfPoints();                       //获取网格中的点数量
    pickpointIDset = new int[static_cast<unsigned long long>(number)];  //为拾取点 ID 数组分配内存
    pointactor = new vtkActor*[static_cast<unsigned long long>(number)];//为点的 Actor 数组分配内存
}

vtkUnstructuredGrid* disp3dinteractor::getGrid()    //返回 disp3dinteractor 类中存储的 vtkUnstructuredGrid 对象指针
{
    return this->Grid;
}

DisplyNodeInfo disp3dinteractor::GetPickPointInfo()
{
    double(*point)[4] = new double[pickernumber][4];
    vtkPoints *points = this->Grid->GetPoints();
    for (unsigned long long i = 0; i < pickernumber; i++){
        point[i][0] = pickpointIDset[i];
        point[i][1] = points->GetPoint(pickpointIDset[i])[0];
        point[i][2] = points->GetPoint(pickpointIDset[i])[1];
        point[i][3] = points->GetPoint(pickpointIDset[i])[2];
    }
    return point;
>>>>>>> Stashed changes
}

void disp3dinteractor::OnLeftButtonDown()
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
        if (m_disp3dmodel->SolidModeNodePointToFlagPoint.find(ID)!= m_disp3dmodel->SolidModeNodePointToFlagPoint.end())
        {
            worldPoint[0] = m_disp3dmodel->new_node_info1[ID]->x_cord;
            worldPoint[1] = m_disp3dmodel->new_node_info1[ID]->y_cord;
            worldPoint[2] = m_disp3dmodel->new_node_info1[ID]->z_cord;
            double worldPoint1[3];
            worldPoint1[0] = worldPoint[0] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[ID]->xdisplace;
            worldPoint1[1] = worldPoint[1] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[ID]->ydisplace;
            worldPoint1[2] = worldPoint[2] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[ID]->zdisplace;
            OldID = ID;
            double DisplayCord[3];
            this->GetDefaultRenderer()->SetWorldPoint(worldPoint1[0], worldPoint1[1], worldPoint1[2], 1.0);
            this->GetDefaultRenderer()->WorldToDisplay();
            this->GetDefaultRenderer()->GetDisplayPoint(DisplayCord);

<<<<<<< Updated upstream
            sprintf(text, "%d", static_cast<int>(ID));
            m_TextActor->SetDisplayPosition(static_cast<int>(DisplayCord[0]), static_cast<int>(DisplayCord[1])); // 设置屏幕显示位置
            m_TextActor->SetInput(text);                                     // 显示文本
            m_TextActor->GetTextProperty()->SetColor(1, 0, 0);               // 设置文本颜色为红色
            m_TextActor->GetTextProperty()->SetFontSize(20);                 // 设置字体大小
            m_disp3dmodel->GetVTKRenderer()->AddActor2D(m_TextActor);        // 添加到渲染器中

            double bounds[6];
            m_disp3dmodel->GetVTKActor()->GetBounds(bounds);
            double dx = bounds[1] - bounds[0];
            double dy = bounds[3] - bounds[2];
            double dz = bounds[5] - bounds[4];
            double diag = std::sqrt(dx*dx + dy*dy + dz*dz);
            double radius = std::max(1e-6, diag * 0.008);               //自适应小球半径计算
=======
            sprintf(text,"%d", m_disp3dmodel->SolidModeNodePointToFlagPoint[ID]);
            //创建文本
            //actor->SetDisplayPosition(pos[0], pos[1]);
            actor->SetDisplayPosition(DisplayCord[0], DisplayCord[1]);
            //actor->SetPosition(m_disp3dmodel->new_node_info1[ID]->x_cord,m_disp3dmodel->new_node_info1[ID]->y_cord,m_disp3dmodel->new_node_info1[ID]->z_cord);
            actor->SetInput(text);
            actor->GetTextProperty()->SetColor(1, 0, 0);
            actor->GetTextProperty()->SetFontSize(20);
            this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor2D(actor);

            if(Flag_Fellow){
                vtkCamera* camera = this->GetCurrentRenderer()->GetActiveCamera();
                double focalPoint[3];
                focalPoint[0] = worldPoint[0];
                focalPoint[1] = worldPoint[1];
                focalPoint[2] = worldPoint[2];
                camera->SetFocalPoint(focalPoint);
                refreshPickPointText();
                this->m_disp3dmodel->refreshFlagPointTextPoisiton();
                this->Interactor->GetRenderWindow()->Render();
            }
>>>>>>> Stashed changes

            //创建小球
            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetRadius(0.003);
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());
<<<<<<< Updated upstream
            m_SphereActor->SetMapper(mapper);
            m_SphereActor->SetPosition(NewPos[0], NewPos[1], NewPos[2]); // 设置小球位置（初始坐标）
            if(Flag_CameraFellow && !Flag_CameraLock){
                vtkCamera* camera = this->GetCurrentRenderer()->GetActiveCamera();
                camera->SetFocalPoint(NewPos[0], NewPos[1], NewPos[2]);
            }
            m_disp3dmodel->GetVTKRenderer()->AddActor(m_SphereActor);
=======
            actor_sphere->SetMapper(mapper);
            actor_sphere->SetPosition(worldPoint[0], worldPoint[1],worldPoint[2]);
            this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor_sphere);
            this->GetInteractor()->GetRenderWindow()->Render();
>>>>>>> Stashed changes

            QString str0 = QString("当前拾取点: %1").arg(m_disp3dmodel->SolidModeNodePointToFlagPoint[ID]);
            QString str1 = QString("初始坐标:(%1, %2, %3)")
                               .arg(QString::number(worldPoint[0]*1000, 'f', 2))
                               .arg(QString::number(worldPoint[1]*1000, 'f', 2))
                               .arg(QString::number(worldPoint[2]*1000, 'f', 2));
            QString str2 = QString("变形量:(%1, %2, %3)")
                               .arg(QString::number(m_disp3dmodel->PointDisplaceSet[ID]->xdisplace*1000, 'f', 2))
                               .arg(QString::number(m_disp3dmodel->PointDisplaceSet[ID]->ydisplace*1000, 'f', 2))
                               .arg(QString::number(m_disp3dmodel->PointDisplaceSet[ID]->zdisplace*1000, 'f', 2));
            QString str3 = QString("当前坐标:(%1, %2, %3)")
                               .arg(QString::number(worldPoint[0]*1000+m_disp3dmodel->PointDisplaceSet[ID]->xdisplace*1000, 'f', 2))
                               .arg(QString::number(worldPoint[1]*1000+m_disp3dmodel->PointDisplaceSet[ID]->ydisplace*1000, 'f', 2))
                               .arg(QString::number(worldPoint[2]*1000+m_disp3dmodel->PointDisplaceSet[ID]->zdisplace*1000, 'f', 2));
            this->label0->setText(str0);
            this->label1->setText(str1);
            this->label2->setText(str2);
            this->label3->setText(str3);
            pickflag = true;
        }
        else{
            if(pickflag == true){
                this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor2D(actor);
                this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(actor_sphere);
                this->GetInteractor()->GetRenderWindow()->Render();
                pickflag = false;
            }
        }
    }
}

void disp3dinteractor::OnRightButtonDown()
{
    // 调用基类的右键按下事件处理函数
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    // 在这里添加您的自定义右键按下事件处理代码
    if(pickflag == true)
    {
        this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor2D(actor);
        this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->RemoveActor(actor_sphere);
        QString str0 = "当前拾取点:";
        QString str1 = "初始坐标:(*,*,*)";
        QString str2 = "变形量:(*,*,*)";
        QString str3 = "当前坐标:(*,*,*)";
        this->label0->setText(str0);
        this->label1->setText(str1);
        this->label2->setText(str2);
        this->label3->setText(str3);
        pickflag = false;
    }
}

void disp3dinteractor::OnMouseMove()
{
<<<<<<< Updated upstream
    if(Flag_CameraLock){
        return;
    }
    else {
        if (Flag_RightButtonPressed){   //鼠标右键按下移动
            int x, y;
            this->GetInteractor()->GetEventPosition(x, y);
            int dx = x - this->LastX;
            int dy = y - this->LastY;
            this->LastX = x;
            this->LastY = y;
            vtkRenderer* ren = this->GetDefaultRenderer();
            vtkCamera* cam = ren->GetActiveCamera();
            if (!ren) return;
            if (!cam) return;
            double right[3], up[3];
            cam->GetViewUp(up);
            cam->OrthogonalizeViewUp();
            double fp[3], pos[3], viewDir[3];
            cam->GetFocalPoint(fp);
            cam->GetPosition(pos);
            cam->GetDirectionOfProjection(viewDir);
            vtkMath::Cross(viewDir, up, right);
            vtkMath::Normalize(right);
            vtkMath::Normalize(up);
            double bounds[6];

            ren->ComputeVisiblePropBounds(bounds);
            double diagonal = sqrt(qPow(bounds[1]-bounds[0],2)+qPow(bounds[3]- bounds[2],2)+qPow(bounds[5]-bounds[4],2));
            double scale = 0.001 * diagonal;

            double move[3];
            move[0] = -dx * right[0] * scale - dy * up[0] * scale;
            move[1] = -dx * right[1] * scale - dy * up[1] * scale;
            move[2] = -dx * right[2] * scale - dy * up[2] * scale;
            for (int i = 0; i < 3; ++i){
                pos[i] += move[i];
                fp[i] += move[i];
            }
            cam->SetPosition(pos);
            cam->SetFocalPoint(fp);
            this->GetInteractor()->Render();
            RefreshText();
            m_disp3dmodel->FreshKeyPoint();
            }
        else{
            vtkInteractorStyleTrackballCamera::OnMouseMove();
            RefreshText();
            m_disp3dmodel->FreshKeyPoint();
            return;
        }
    }
=======
    vtkInteractorStyleTrackballCamera::OnMouseMove();
>>>>>>> Stashed changes
}

void disp3dinteractor::OnKeyUp()
{
    return;
}

//相机移动
void disp3dinteractor::OnKeyDown()
{
    vtkRenderWindowInteractor* iren = this->GetInteractor();
    std::string key = iren->GetKeySym();
    vtkCamera* camera = this->GetCurrentRenderer()->GetActiveCamera();

    // 获取相机当前方向
    double forward[3], right[3], up[3];

    // 计算相机前进方向（视线方向）
    camera->GetDirectionOfProjection(forward);
    forward[0] *= -1.0; forward[1] *= -1.0; forward[2] *= -1.0; // 取反，因为相机看向负Z轴

    // 计算相机右方向（垂直于视线和上方向）
    camera->GetViewUp(up);
    vtkMath::Cross(forward, up, right);

    // 归一化向量
    vtkMath::Normalize(forward);
    vtkMath::Normalize(right);
    vtkMath::Normalize(up);
    // 根据按键移动相机
    const double moveFactor = 0.01;
    double pos[3], focalPoint[3];
    camera->GetPosition(pos);         // 获取当前位置
    camera->GetFocalPoint(focalPoint); // 获取当前焦点
    if (key == "d") { // 左移
        pos[0] -= right[0] * moveFactor;
        pos[1] -= right[1] * moveFactor;
        pos[2] -= right[2] * moveFactor;
        focalPoint[0] -= right[0] * moveFactor;
        focalPoint[1] -= right[1] * moveFactor;
        focalPoint[2] -= right[2] * moveFactor;
    } else if (key == "a") { // 右移
        pos[0] += right[0] * moveFactor;
        pos[1] += right[1] * moveFactor;
        pos[2] += right[2] * moveFactor;
        focalPoint[0] += right[0] * moveFactor;
        focalPoint[1] += right[1] * moveFactor;
        focalPoint[2] += right[2] * moveFactor;
    } else if (key == "q") { // 上移
        pos[0] += up[0] * moveFactor;
        pos[1] += up[1] * moveFactor;
        pos[2] += up[2] * moveFactor;
        focalPoint[0] += up[0] * moveFactor;
        focalPoint[1] += up[1] * moveFactor;
        focalPoint[2] += up[2] * moveFactor;
    } else if (key == "e") { // 下移
        pos[0] -= up[0] * moveFactor;
        pos[1] -= up[1] * moveFactor;
        pos[2] -= up[2] * moveFactor;
        focalPoint[0] -= up[0] * moveFactor;
        focalPoint[1] -= up[1] * moveFactor;
        focalPoint[2] -= up[2] * moveFactor;
    }
    camera->SetPosition(pos);         // 设置新位置
    camera->SetFocalPoint(focalPoint); // 设置新焦点
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::Rotate()
{
    vtkInteractorStyleTrackballCamera::Rotate();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::Spin()
{
    vtkInteractorStyleTrackballCamera::Spin();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();

}

void disp3dinteractor::Pan()
{
    vtkInteractorStyleTrackballCamera::Pan();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::Dolly()
{
    vtkInteractorStyleTrackballCamera::Dolly();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::OnMouseWheelForward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::OnMouseWheelBackward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    refreshPickPointText();
    this->m_disp3dmodel->refreshFlagPointTextPoisiton();
    this->Interactor->GetRenderWindow()->Render();
}

void disp3dinteractor::OnKeyPress()
{
    return;
}

void disp3dinteractor::OnKeyRelease()
{
    return;
}

double *disp3dinteractor::caculatePos(double x, double y, double z)
{
    double point[3];
    double * minpointcord = new double[4];
    //int nodenum;
    vtkPoints *points = Grid->GetPoints();
    int pointnum = points->GetNumberOfPoints();
    double minlength = 100000.0;
    for (int i = 0; i < pointnum; i++){
        if ((!findpointID(i))){ //先判断当前的点有没有被拾取，如果有直接跳过，如果没有，参与比较
            point[0] = points->GetPoint(i)[0];
            point[1] = points->GetPoint(i)[1];
            point[2] = points->GetPoint(i)[2];
            double lengh = caculatelength(point[0], point[1], point[2], x, y, z);
            if (lengh - minlength<0){
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

double disp3dinteractor::caculatelength(double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2));
}

bool disp3dinteractor::findpointID(int number)
{
    bool found = false;
    for (int i = 0; i<pickernumber&&!found; i++){
        if (pickpointIDset[i]==number){
            found = true;
        }
    }
    return found;
}

void disp3dinteractor::refreshPickPointText()
{
    if(pickflag)
    {
        double DisplayCord[3];
        double worldPoint1[3];
        worldPoint1[0] = worldPoint[0] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[OldID]->xdisplace;
        worldPoint1[1] = worldPoint[1] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[OldID]->ydisplace;
        worldPoint1[2] = worldPoint[2] + m_disp3dmodel->Displacefactor*m_disp3dmodel->PointDisplaceSet[OldID]->zdisplace;
        this->GetDefaultRenderer()->SetWorldPoint(worldPoint1[0], worldPoint1[1], worldPoint1[2], 1.0);
        this->GetDefaultRenderer()->WorldToDisplay();
        this->GetDefaultRenderer()->GetDisplayPoint(DisplayCord);
        actor->SetDisplayPosition(DisplayCord[0], DisplayCord[1]);
        actor_sphere->SetPosition(worldPoint1[0],worldPoint1[1],worldPoint1[2]);
        QString str0 = QString("当前拾取点: %1").arg(m_disp3dmodel->SolidModeNodePointToFlagPoint[OldID]);
        QString str1 = QString("初始坐标:(%1, %2, %3)")
                           .arg(QString::number(worldPoint[0]*1000, 'f', 2))
                           .arg(QString::number(worldPoint[1]*1000, 'f', 2))
                           .arg(QString::number(worldPoint[2]*1000, 'f', 2));
        QString str2 = QString("变形量:(%1, %2, %3)")
                           .arg(QString::number(m_disp3dmodel->PointDisplaceSet[OldID]->xdisplace*1000, 'f', 2))
                           .arg(QString::number(m_disp3dmodel->PointDisplaceSet[OldID]->ydisplace*1000, 'f', 2))
                           .arg(QString::number(m_disp3dmodel->PointDisplaceSet[OldID]->zdisplace*1000, 'f', 2));
        QString str3 = QString("当前坐标:(%1, %2, %3)")
                           .arg(QString::number(worldPoint[0]*1000+m_disp3dmodel->PointDisplaceSet[OldID]->xdisplace*1000, 'f', 2))
                           .arg(QString::number(worldPoint[1]*1000+m_disp3dmodel->PointDisplaceSet[OldID]->ydisplace*1000, 'f', 2))
                           .arg(QString::number(worldPoint[2]*1000+m_disp3dmodel->PointDisplaceSet[OldID]->zdisplace*1000, 'f', 2));
        this->label0->setText(str0);
        this->label1->setText(str1);
        this->label2->setText(str2);
        this->label3->setText(str3);
    }
}

void disp3dinteractor::setpickflag(bool flag)
{
    pickflag = flag;
}

