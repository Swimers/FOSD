#ifndef TRACKINGPOINTDATATABLE_H
#define TRACKINGPOINTDATATABLE_H

#include <QWidget>
#include <QTime>

#include <disp3dmodel.h>
#include <trackingpoint.h>

namespace Ui {
class trackingpointdatatable;
}

class trackingpointdatatable : public QWidget
{
    Q_OBJECT

public: 
    static trackingpointdatatable* instance(QWidget *parent = nullptr);         // 获取单例实例（静态方法）
    trackingpointdatatable(const trackingpointdatatable&) = delete;             // 禁止拷贝构造和赋值
    trackingpointdatatable& operator=(const trackingpointdatatable&) = delete;
    ~trackingpointdatatable();

public:
    void SetDisplayModel(disp3dmodel *ren_disp3dmodel){m_disp3dmodel = ren_disp3dmodel;}
    void SetTrackingPoint(trackingpoint *ren_trackingpoint){m_trackingpoint = ren_trackingpoint; RefreshTable();}

    void InitWindow();
    void InitializeTableRows();

    void StartAutoRefresh(int intervalMs = 100);  // 启动自动刷新（默认100ms）
    void StopAutoRefresh();                       // 停止自动刷新

public slots:
    void RefreshTable();



private slots:
    void on_cbox_AutoRefresh_toggled(bool checked);

    void on_pbtn_ShowAll_clicked();

    void on_pbtn_HideAll_clicked();

    void on_pbtn_ShowSelected_clicked();

    void on_pbtn_ShowAllRows_clicked();

    void on_pbtn_Refresh_clicked();

private:
    explicit trackingpointdatatable(QWidget *parent = nullptr);             // 静态实例指针
    static trackingpointdatatable* m_instance;

    disp3dmodel *m_disp3dmodel = nullptr;
    trackingpoint *m_trackingpoint = nullptr;

    QTimer* m_refreshTimer = nullptr;  // 自动刷新定时器

    QMap<int, bool> rowVisibility;     // 记录每行的显示状态（行号 -> 是否显示）

    Ui::trackingpointdatatable *ui;
};

#endif // TRACKINGPOINTDATATABLE_H
