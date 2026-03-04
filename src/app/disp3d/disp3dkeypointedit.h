#ifndef DISP3DKEYPOINTEDIT_H
#define DISP3DKEYPOINTEDIT_H

#include <QWidget>
#include <disp3dmodel.h>

namespace Ui {
class disp3dkeypointedit;
}

class disp3dkeypointedit : public QWidget
{
    Q_OBJECT

public:
    static disp3dkeypointedit* instance(QWidget *parent = nullptr);     // 获取单例实例（静态方法）
    disp3dkeypointedit(const disp3dkeypointedit&) = delete;             // 禁止拷贝构造和赋值
    disp3dkeypointedit& operator=(const disp3dkeypointedit&) = delete;
    ~disp3dkeypointedit();

public:
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}

    void EditInit();

    void UpDataKeyInfor();
    void UpDataKeyPosition(vtkIdType id);

    void ClearDisp();

private slots:
    void on_cbox_IDKeyPoint_currentIndexChanged(const QString &arg1);
    void on_pbtn_FindKeyPoint_clicked();
    void on_pbtn_SelectAll_clicked();
    void on_pbtn_ClearAll_clicked();
    void on_pbtn_DispCube_clicked();
    void on_pbtn_SelectCube_clicked();
    void on_pbtn_ClearCube_clicked();
    void on_pushButton_clicked();

public:
    void SelectAll();

signals:
    void SelectSuccess();

private:
    double CubePoint[6];
    vtkSmartPointer<vtkActor> cubeActor = nullptr;

private:
    explicit disp3dkeypointedit(QWidget *parent = nullptr);             // 静态实例指针
    static disp3dkeypointedit* m_instance;

    disp3dmodel* m_disp3dmodel = nullptr;

    Ui::disp3dkeypointedit *ui;
};

#endif // DISP3DKEYPOINTEDIT_H
