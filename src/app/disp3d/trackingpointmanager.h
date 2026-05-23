#ifndef TRACKINGPOINTMANAGER_H
#define TRACKINGPOINTMANAGER_H

#include <QWidget>

#include <disp3dmodel.h>
#include <trackingpoint.h>

namespace Ui {
class trackingpointmanager;
}

class trackingpointmanager : public QWidget
{
    Q_OBJECT

public:
    static trackingpointmanager* instance(QWidget *parent = nullptr);         // 获取单例实例（静态方法）
    trackingpointmanager(const trackingpointmanager&) = delete;             // 禁止拷贝构造和赋值
    trackingpointmanager& operator=(const trackingpointmanager&) = delete;
    ~trackingpointmanager();

public:
    void SetDisplayModel(disp3dmodel *ren_disp3dmodel){m_disp3dmodel = ren_disp3dmodel;}
    void SetTrackingPoint(trackingpoint *ren_trackingpoint){m_trackingpoint = ren_trackingpoint;}

    void InitWindow();
    void RefreshTable();
    void UpdataTableRow(int row, TrackingPoint* tp);
    void OnVisibilityChanged(int row, int state);


private slots:
    void on_pbtn_LoadFile_clicked();
    void on_pbtn_ClearAll_clicked();
    void on_pbtn_SelectAll_clicked();
    void on_pbtn_SelectNone_clicked();
    void on_pbtn_Refresh_clicked();

private:
    explicit trackingpointmanager(QWidget *parent = nullptr);             // 静态实例指针
    static trackingpointmanager* m_instance;

    disp3dmodel *m_disp3dmodel = nullptr;
    trackingpoint *m_trackingpoint = nullptr;

    Ui::trackingpointmanager *ui;
};

#endif // TRACKINGPOINTMANAGER_H
