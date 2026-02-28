#include "disp3dkeypointselect.h"
#include "ui_disp3dkeypointselect.h"

#include <QMessageBox>
#include <QDebug>

#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>

#include <disp3dglobal.h>

disp3dkeypointselect* disp3dkeypointselect::m_instance = nullptr;

disp3dkeypointselect::disp3dkeypointselect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disp3dkeypointselect)
{
    ui->setupUi(this);

    Flag_Edit = false;
    //setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

disp3dkeypointselect::~disp3dkeypointselect()
{
    delete ui;
    m_instance = nullptr;  // 销毁时清空单例指针
}

disp3dkeypointselect *disp3dkeypointselect::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new disp3dkeypointselect(parent);
    }
    return m_instance;
}

void disp3dkeypointselect::InitWindow()
{
    rowCount = 0;
    ui->tablew_PointPara->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablew_PointPara->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablew_PointPara->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tablew_PointPara->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablew_PointPara->clearContents();
    ui->tablew_PointPara->setRowCount(1);

    if (!m_disp3dmodel->SelectedPointMap.isEmpty()) {
        QMap<vtkIdType, SelectedPointEntity>::const_iterator it;
        for (it = m_disp3dmodel->SelectedPointMap.constBegin(); it != m_disp3dmodel->SelectedPointMap.constEnd(); ++it) {
            vtkIdType id = it.key();  // 从 QMap 取出点 ID
            QVector<double> coord = m_disp3dmodel->rawPointMap.value(id);

            ui->lab_XPos->setText(QString::number(coord[0]));
            ui->lab_YPos->setText(QString::number(coord[1]));
            ui->lab_ZPos->setText(QString::number(coord[2]));

            ui->tablew_PointPara->insertRow(rowCount);
            QTableWidgetItem* item0 = new QTableWidgetItem(QString::number(id));
            QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(coord[0]));
            QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(coord[1]));
            QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(coord[2]));

            ui->tablew_PointPara->setItem(rowCount, 0, item0);
            ui->tablew_PointPara->setItem(rowCount, 1, item1);
            ui->tablew_PointPara->setItem(rowCount, 2, item2);
            ui->tablew_PointPara->setItem(rowCount, 3, item3);
        }
    }
    else {
        rowCount = 0;
    }

    ui->lab_XPos->clear();
    ui->lab_YPos->clear();
    ui->lab_ZPos->clear();
}

void disp3dkeypointselect::UpDataKeyInfor()
{
    ui->lab_NumPoints->setText(QString::number(m_disp3dmodel->ModelPointsNum));
    ui->lab_NumKeyPoints->setText(QString::number(m_disp3dmodel->ModelKeyPointsNum));

    if(!m_disp3dmodel->keyPointSet.isEmpty()){
        QList<vtkIdType> idList = m_disp3dmodel->keyPointSet.values();
        std::sort(idList.begin(), idList.end());
        for (vtkIdType id : idList){
            QString idStr = QString::number(id);
            ui->cbox_KeyPointID->addItem(idStr);
            ui->cbox_KeyPointID->currentIndexChanged(0);
        }
    }
    else {
        ui->cbox_KeyPointID->clear();
    }

}

void disp3dkeypointselect::UpDataKeyPosition(vtkIdType id)
{
    QVector<double> coord = m_disp3dmodel->rawPointMap.value(id);
    ui->lab_XPos->setText(QString::number(coord[0]));
    ui->lab_YPos->setText(QString::number(coord[1]));
    ui->lab_ZPos->setText(QString::number(coord[2]));
}

void disp3dkeypointselect::on_pbtn_Exit_clicked()
{
    this->close();
}

void disp3dkeypointselect::on_cbox_KeyPointID_currentIndexChanged(const QString &arg1)
{
    vtkIdType id = arg1.toInt();
    UpDataKeyPosition(id);
}

void disp3dkeypointselect::on_pbtn_AddPoint_clicked()
{
    QString IDstr = ui->cbox_KeyPointID->currentText();
    vtkIdType ID = IDstr.toLongLong();

    if (m_disp3dmodel->keyPointSet.isEmpty()) {
        return;
    } else {
        if (m_disp3dmodel->SelectedPointMap.contains(ID)) {
            QMessageBox::critical(this, "错误", "此点已经选中");
            return;
        }
        QVector<double> RawPos = m_disp3dmodel->rawPointMap.value(ID);
        QVector<double> Displace = m_disp3dmodel->pointDisplace.value(ID);
        QVector<double> NewPos = m_disp3dmodel->pointpos.value(ID);

        double DisplayCord[3];
        m_disp3dmodel->GetVTKRenderer()->SetWorldPoint(NewPos[0], NewPos[1], NewPos[2], 1.0);
        m_disp3dmodel->GetVTKRenderer()->WorldToDisplay();
        m_disp3dmodel->GetVTKRenderer()->GetDisplayPoint(DisplayCord);

        double bounds[6];
        m_disp3dmodel->GetVTKActor()->GetBounds(bounds);
        double dx = bounds[1] - bounds[0];
        double dy = bounds[3] - bounds[2];
        double dz = bounds[5] - bounds[4];
        double diag = std::sqrt(dx*dx + dy*dy + dz*dz);
        double radius = std::max(1e-6, diag * 0.008);
        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetRadius(radius);
        sphereSource->SetPhiResolution(6);
        sphereSource->SetThetaResolution(6);
        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());
        vtkActor* SphereActor = vtkActor::New();
        SphereActor->SetMapper(mapper);
        SphereActor->SetPosition(NewPos[0], NewPos[1], NewPos[2]);
        m_disp3dmodel->GetVTKRenderer()->AddActor(SphereActor);

        vtkTextActor* TextActor = vtkTextActor::New();
        char textbuf[64] = { '\0' };
        sprintf(textbuf, "(%0.2f,%0.2f,%0.2f)", NewPos[0], NewPos[1], NewPos[2]);
        TextActor->SetDisplayPosition(static_cast<int>(DisplayCord[0]), static_cast<int>(DisplayCord[1]));
        TextActor->SetInput(textbuf);
        TextActor->GetTextProperty()->SetColor(1, 0, 0);
        TextActor->GetTextProperty()->SetFontSize(20);
        m_disp3dmodel->GetVTKRenderer()->AddActor2D(TextActor);
        m_disp3dmodel->GetVTKRenderWindow()->Render();

        SelectedPointEntity ent;
        ent.keyPointEntity = SphereActor;
        ent.keyPointText = TextActor;
        m_disp3dmodel->SelectedPointMap.insert(ID, ent);

        ui->lab_XPos->setText(QString::number(NewPos[0]));
        ui->lab_YPos->setText(QString::number(NewPos[1]));
        ui->lab_ZPos->setText(QString::number(NewPos[2]));
        int rowCount = ui->tablew_PointPara->rowCount();
        ui->tablew_PointPara->insertRow(rowCount);
        QTableWidgetItem* item0 = new QTableWidgetItem(IDstr);
        QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(NewPos[0]));
        QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(NewPos[1]));
        QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(NewPos[2]));

        ui->tablew_PointPara->setItem(rowCount-1, 0, item0);
        ui->tablew_PointPara->setItem(rowCount-1, 1, item1);
        ui->tablew_PointPara->setItem(rowCount-1, 2, item2);
        ui->tablew_PointPara->setItem(rowCount-1, 3, item3);
    }
}

void disp3dkeypointselect::on_pbtn_DeletePoint_clicked()
{
    QString IDstr = ui->cbox_KeyPointID->currentText();
    int currentRow = ui->tablew_PointPara->currentRow();

    if (m_disp3dmodel->SelectedPointMap.isEmpty()) {
        QMessageBox::critical(this, "错误", "未选择任何关键点");
        return;
    }
    if (currentRow < 0) {
        QMessageBox::critical(this, "错误", "未选择正确的表格行");
        return;
    }
    QTableWidgetItem *item = ui->tablew_PointPara->item(currentRow, 0);
    if (!item) {
        QMessageBox::critical(this, "错误", "选中行无效");
        return;
    }
    vtkIdType deleteID = item->data(Qt::DisplayRole).toLongLong();
    if (!m_disp3dmodel->SelectedPointMap.contains(deleteID)) {
        QMessageBox::critical(this, "错误", "此点不在选中列表中");
        return;
    }

    SelectedPointEntity entity = m_disp3dmodel->SelectedPointMap.value(deleteID);
    if (entity.keyPointEntity) {
        m_disp3dmodel->GetVTKRenderer()->RemoveActor(entity.keyPointEntity);
        entity.keyPointEntity->Delete();
        entity.keyPointEntity = nullptr;
    }
    if (entity.keyPointText) {
        m_disp3dmodel->GetVTKRenderer()->RemoveActor2D(entity.keyPointText);
        entity.keyPointText->Delete();
        entity.keyPointText = nullptr;
    }
    m_disp3dmodel->SelectedPointMap.remove(deleteID);
    ui->tablew_PointPara->removeRow(currentRow);
    m_disp3dmodel->GetVTKRenderWindow()->Render();
    QMessageBox::information(this, "提示", QString("关键点 %1 已删除").arg(deleteID));
}

void disp3dkeypointselect::on_pbtn_EditPoint_clicked()
{
    if(Flag_Edit){
        QString IDstr = ui->cbox_KeyPointID->currentText();
        vtkIdType ID = IDstr.toLongLong();



        if(ID == editingOldID){
            ui->pbtn_AddPoint->setEnabled(true);
            ui->pbtn_DeletePoint->setEnabled(true);
            ui->pbtn_EditPoint->setText("编辑节点");
            Flag_Edit = false;
        }
        else {
            QVector<double> RawPos = m_disp3dmodel->rawPointMap.value(ID);
            QVector<double> Displace = m_disp3dmodel->pointDisplace.value(ID);
            QVector<double> NewPos = m_disp3dmodel->pointpos.value(ID);

            double DisplayCord[3];
            m_disp3dmodel->GetVTKRenderer()->SetWorldPoint(NewPos[0], NewPos[1], NewPos[2], 1.0);
            m_disp3dmodel->GetVTKRenderer()->WorldToDisplay();
            m_disp3dmodel->GetVTKRenderer()->GetDisplayPoint(DisplayCord);

            // 计算自适应半径
            double bounds[6];
            m_disp3dmodel->GetVTKRenderer()->ComputeVisiblePropBounds(bounds);
            double dx = bounds[1] - bounds[0];
            double dy = bounds[3] - bounds[2];
            double dz = bounds[5] - bounds[4];
            double diag = std::sqrt(dx*dx + dy*dy + dz*dz);
            double radius = std::max(1e-6, diag * 0.008);

            // Sphere actor
            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetRadius(radius);
            sphereSource->SetPhiResolution(24);
            sphereSource->SetThetaResolution(24);

            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());

            vtkActor* SphereActor = vtkActor::New();
            SphereActor->SetMapper(mapper);
            SphereActor->SetPosition(NewPos[0], NewPos[1], NewPos[2]);
            m_disp3dmodel->GetVTKRenderer()->AddActor(SphereActor);

            // Text actor
            vtkTextActor* TextActor = vtkTextActor::New();
            char textbuf[64] = { '\0' };
            sprintf(textbuf, "(%0.2f,%0.2f,%0.2f)", NewPos[0], NewPos[1], NewPos[2]);
            TextActor->SetDisplayPosition(static_cast<int>(DisplayCord[0]), static_cast<int>(DisplayCord[1]));
            TextActor->SetInput(textbuf);
            TextActor->GetTextProperty()->SetColor(1, 0, 0);
            TextActor->GetTextProperty()->SetFontSize(20);
            m_disp3dmodel->GetVTKRenderer()->AddActor2D(TextActor);

            // ---- 删除旧实体并从 SelectedPointMap 中移除 old ID ----
            if (m_disp3dmodel->SelectedPointMap.contains(editingOldID)) {
                SelectedPointEntity oldEnt = m_disp3dmodel->SelectedPointMap.value(editingOldID);

                if (oldEnt.keyPointEntity) {
                    m_disp3dmodel->GetVTKRenderer()->RemoveActor(oldEnt.keyPointEntity);
                    oldEnt.keyPointEntity->Delete();
                }
                if (oldEnt.keyPointText) {
                    m_disp3dmodel->GetVTKRenderer()->RemoveActor2D(oldEnt.keyPointText);
                    oldEnt.keyPointText->Delete();
                }
                m_disp3dmodel->SelectedPointMap.remove(editingOldID);
            }

            // ---- 插入新记录到 SelectedPointMap ----
            SelectedPointEntity newEnt;
            newEnt.keyPointEntity = SphereActor;
            newEnt.keyPointText = TextActor;
            m_disp3dmodel->SelectedPointMap.insert(ID, newEnt);

            ui->tablew_PointPara->item(editingRow, 0)->setText(QString::number(ID));
            ui->tablew_PointPara->item(editingRow, 1)->setText(QString::number(NewPos[0]));
            ui->tablew_PointPara->item(editingRow, 2)->setText(QString::number(NewPos[1]));
            ui->tablew_PointPara->item(editingRow, 3)->setText(QString::number(NewPos[2]));

            // 触发渲染刷新
            m_disp3dmodel->GetVTKRenderWindow()->Render();

            // ---- 恢复 UI 状态 ----
            ui->pbtn_AddPoint->setEnabled(true);
            ui->pbtn_DeletePoint->setEnabled(true);
            ui->pbtn_EditPoint->setText("编辑节点");
            Flag_Edit = false;
            editingRow = -1;
            editingOldID = -1;

            QMessageBox::information(this, "提示", QString("已将关键点 %1 编辑为 %2").arg(editingOldID).arg(ID));
        }
    }
    else {
        int currentRow = ui->tablew_PointPara->currentRow();
        if (m_disp3dmodel->SelectedPointMap.isEmpty()) {
            QMessageBox::critical(this, "错误", "未选择任何关键点");
            return;
        }
        if (currentRow < 0) {
            QMessageBox::critical(this, "错误", "未选择正确的表格行");
            return;
        }
        QTableWidgetItem *item = ui->tablew_PointPara->item(currentRow, 0);
        if (!item) {
            QMessageBox::critical(this, "错误", "选中行无效");
            return;
        }
        vtkIdType EditID = item->data(Qt::DisplayRole).toLongLong();
        if (!m_disp3dmodel->SelectedPointMap.contains(EditID)) {
            QMessageBox::critical(this, "错误", "此点不在选中列表中");
            return;
        }

        ui->cbox_KeyPointID->setCurrentText(QString::number(EditID));
        editingOldID = EditID;
        editingRow = currentRow;

        ui->pbtn_AddPoint->setEnabled(false);
        ui->pbtn_DeletePoint->setEnabled(false);
        ui->pbtn_EditPoint->setText("确认编辑");
        Flag_Edit = true;
    }
}

