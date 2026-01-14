#ifndef DISP3DKEYPOINTSELECT_H
#define DISP3DKEYPOINTSELECT_H

#include <QWidget>

#include <disp3dmodel.h>

namespace Ui {
class disp3dkeypointselect;
}

class disp3dkeypointselect : public QWidget
{
    Q_OBJECT

public:
    static disp3dkeypointselect* instance(QWidget *parent = nullptr);     // 获取单例实例（静态方法）
    disp3dkeypointselect(const disp3dkeypointselect&) = delete;             // 禁止拷贝构造和赋值
    disp3dkeypointselect& operator=(const disp3dkeypointselect&) = delete;
    ~disp3dkeypointselect();

public:
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}

    void InitWindow();
    void UpDataKeyInfor();
    void UpDataKeyPosition(vtkIdType id);

private slots:
    void on_pbtn_Exit_clicked();
    void on_cbox_KeyPointID_currentIndexChanged(const QString &arg1);
    void on_pbtn_AddPoint_clicked();
    void on_pbtn_DeletePoint_clicked();
    void on_pbtn_EditPoint_clicked();

public:
    int rowCount;
    int rowEdit;
    bool Flag_Edit;

    vtkIdType editingOldID;
    int editingRow;

private:
    explicit disp3dkeypointselect(QWidget *parent = nullptr);             // 静态实例指针
    static disp3dkeypointselect* m_instance;

    Ui::disp3dkeypointselect *ui;

    disp3dmodel* m_disp3dmodel = nullptr;
};

#endif // DISP3DKEYPOINTSELECT_H
