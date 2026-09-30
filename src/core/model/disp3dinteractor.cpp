#include "disp3dinteractor.h"

#include "vtkAutoInit.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCallbackCommand.h"
#include "vtkRenderer.h"
#include "vtkPointPicker.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"

#include <QDebug>
#include <QString>
#include <QtMath>
#include <algorithm>
#include <cmath>

vtkStandardNewMacro(disp3dinteractor);

disp3dinteractor::disp3dinteractor()
{
    m_SphereActor = vtkActor::New();
    m_SphereActor->PickableOff();
    Flag_Pick = false;
}

disp3dinteractor::~disp3dinteractor()
{
    if (m_disp3dmodel && Flag_Pick) {
        ClearPicker();
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
}

void disp3dinteractor::OnLeftButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

    int* pos = this->GetInteractor()->GetEventPosition();
    vtkSmartPointer<vtkPointPicker> pick1 = vtkSmartPointer<vtkPointPicker>::New();
    pick1->SetTolerance(0.009);
    pick1->PickFromListOn();
    pick1->AddPickList(m_disp3dmodel->GetVTKActor());
    pick1->Pick(pos[0], pos[1], 0, m_disp3dmodel->GetVTKRenderer());
    vtkIdType ID = pick1->GetPointId();

    if (ID != -1){
        if(m_disp3dmodel->keyPointSet.contains(ID)){
            QVector<double> RawPos = m_disp3dmodel->rawPointMap.value(ID);
            QVector<double> Displace = m_disp3dmodel->pointDisplace.value(ID);
            QVector<double> NewPos = m_disp3dmodel->pointpos.value(ID);

            double bounds[6];
            m_disp3dmodel->GetVTKActor()->GetBounds(bounds);
            double dx = bounds[1] - bounds[0];
            double dy = bounds[3] - bounds[2];
            double dz = bounds[5] - bounds[4];
            double diag = std::sqrt(dx*dx + dy*dy + dz*dz);
            double radius = std::max(1e-6, diag * 0.008);               //自适应小球半径计算

            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetRadius(radius);                            // 设置小球半径
            sphereSource->SetPhiResolution(6);                         // 设置小球面数
            sphereSource->SetThetaResolution(6);
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());
            m_SphereActor->SetMapper(mapper);
            m_SphereActor->SetPosition(NewPos[0], NewPos[1], NewPos[2]); // 设置小球位置（初始坐标）
            if(Flag_CameraFellow && !Flag_CameraLock){
                vtkCamera* camera = this->GetCurrentRenderer()->GetActiveCamera();
                camera->SetFocalPoint(NewPos[0], NewPos[1], NewPos[2]);
            }
            m_disp3dmodel->GetVTKRenderer()->AddActor(m_SphereActor);

            if (m_pointIdOverlay) {
                m_pointIdOverlay->setText(QString::number(ID));
                UpdatePointIdOverlay(NewPos.constData());
            }

            m_disp3dmodel->GetVTKRenderWindow()->Render();

            QString PointID = QString::number(ID);
            QString PointRawPos = QString("(%1, %2, %3)")
                                   .arg(QString::number(RawPos[0], 'f', 2))
                                   .arg(QString::number(RawPos[1], 'f', 2))
                                   .arg(QString::number(RawPos[2], 'f', 2));
            QString PointDisplace = QString("(%1, %2, %3)")
                                        .arg(QString::number(Displace[0], 'f', 2))
                                        .arg(QString::number(Displace[1], 'f', 2))
                                        .arg(QString::number(Displace[2], 'f', 2));
            QString PointNewPos = QString("(%1, %2, %3)")
                                   .arg(QString::number(NewPos[0], 'f', 2))
                                   .arg(QString::number(NewPos[1], 'f', 2))
                                   .arg(QString::number(NewPos[2], 'f', 2));
            lab_KeyPointID->setText(PointID);
            lab_KeyPointRawPos->setText(PointRawPos);
            lab_KeyPointDisplace->setText(PointDisplace);
            lab_KeyPointNewPos->setText(PointNewPos);

            Flag_Pick = true;
            OldID = ID;
        }
        else {
            if (Flag_Pick == true) {                
                ClearPicker();
                lab_KeyPointID->setText("非关键点");
                Flag_Pick = false;
            }
        }
    }

    RefreshText();
}

void disp3dinteractor::OnRightButtonDown()
{
    Flag_RightButtonPressed = true;
    GetInteractor()->GetEventPosition(this->LastX, this->LastY);
    GetInteractor()->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_SIZEALL);
    GrabFocus(this->EventCallbackCommand);

    ClearPicker();
    RefreshText();
}

void disp3dinteractor::OnRightButtonUp()
{
    Flag_RightButtonPressed = false;
    ReleaseFocus();
    GetInteractor()->GetRenderWindow()->SetCurrentCursor(VTK_CURSOR_DEFAULT);
}

void disp3dinteractor::OnMouseMove()
{
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
}

void disp3dinteractor::OnKeyUp()
{
    return;
}

void disp3dinteractor::OnKeyDown()
{
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::Rotate()
{
    vtkInteractorStyleTrackballCamera::Rotate();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::Spin()
{
    vtkInteractorStyleTrackballCamera::Spin();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::Pan()
{
    vtkInteractorStyleTrackballCamera::Pan();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::Dolly()
{
    vtkInteractorStyleTrackballCamera::Dolly();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::OnMouseWheelForward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::OnMouseWheelBackward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
    RefreshText();
    m_disp3dmodel->FreshKeyPoint();
}

void disp3dinteractor::OnKeyPress()
{
    return;
}

void disp3dinteractor::OnKeyRelease()
{
    return;
}

void disp3dinteractor::UpdatePointIdOverlay(const double worldPos[3])
{
    if (!m_pointIdOverlay || !m_disp3dmodel) {
        return;
    }

    QWidget* viewport = m_pointIdOverlay->parentWidget();
    vtkRenderWindow* renderWindow = m_disp3dmodel->GetVTKRenderWindow();
    vtkRenderer* renderer = m_disp3dmodel->GetVTKRenderer();
    if (!viewport || !renderWindow || !renderer) {
        m_pointIdOverlay->hide();
        return;
    }

    const int* renderSize = renderWindow->GetSize();
    if (renderSize[0] <= 0 || renderSize[1] <= 0) {
        m_pointIdOverlay->hide();
        return;
    }

    renderer->SetWorldPoint(worldPos[0], worldPos[1], worldPos[2], 1.0);
    renderer->WorldToDisplay();
    double displayPos[3];
    renderer->GetDisplayPoint(displayPos);
    if (!std::isfinite(displayPos[0]) || !std::isfinite(displayPos[1]) ||
        displayPos[0] < 0 || displayPos[0] > renderSize[0] ||
        displayPos[1] < 0 || displayPos[1] > renderSize[1]) {
        m_pointIdOverlay->hide();
        return;
    }

    m_pointIdOverlay->adjustSize();
    const double scaleX = static_cast<double>(viewport->width()) / renderSize[0];
    const double scaleY = static_cast<double>(viewport->height()) / renderSize[1];
    const int x = qRound(displayPos[0] * scaleX) + 8;
    const int y = viewport->height() - qRound(displayPos[1] * scaleY)
                  - m_pointIdOverlay->height() - 4;
    m_pointIdOverlay->move(std::max(0, std::min(x, viewport->width() - m_pointIdOverlay->width())),
                           std::max(0, std::min(y, viewport->height() - m_pointIdOverlay->height())));
    m_pointIdOverlay->raise();
    m_pointIdOverlay->show();
}

void disp3dinteractor::RefreshText()
{
    if(Flag_Pick){
        QVector<double> NewPos = m_disp3dmodel->pointpos.value(OldID);
        if (NewPos.size() < 3) {
            if (m_pointIdOverlay) m_pointIdOverlay->hide();
            return;
        }
        UpdatePointIdOverlay(NewPos.constData());
        m_SphereActor->SetPosition(NewPos[0],NewPos[1],NewPos[2]);

        m_disp3dmodel->GetVTKRenderWindow()->Render();
    }
}

void disp3dinteractor::ClearPicker()
{
    if(Flag_Pick == true){
        if (m_pointIdOverlay) m_pointIdOverlay->hide();
        m_disp3dmodel->GetVTKRenderer()->RemoveActor(m_SphereActor);

        QString str0 = "###";
        QString str1 = "(#,#,#)";

        lab_KeyPointID->setText(str0);
        lab_KeyPointRawPos->setText(str1);
        lab_KeyPointDisplace->setText(str1);
        lab_KeyPointNewPos->setText(str1);

        m_disp3dmodel->GetVTKRenderWindow()->Render();

        Flag_Pick = false;
    }
}

void disp3dinteractor::RefreshSelectedPoints()
{
    RefreshText();
}
