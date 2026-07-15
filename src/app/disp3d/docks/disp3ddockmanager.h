#ifndef DISP3DDOCKMANAGER_H
#define DISP3DDOCKMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <Qt>

class QAction;
class QDockWidget;
class QMainWindow;
class Disp3DDockPanel;

class Disp3DDockManager : public QObject
{
    Q_OBJECT

public:
    explicit Disp3DDockManager(QMainWindow *host, QObject *parent = nullptr);

    void registerDock(const QString &id,
                      QDockWidget *dock,
                      QAction *showAction = nullptr,
                      bool initiallyVisible = true,
                      Qt::DockWidgetArea area = Qt::RightDockWidgetArea);
    void registerPanel(Disp3DDockPanel *panel, QAction *showAction = nullptr);

    QDockWidget *dock(const QString &id) const;
    void setDockVisible(const QString &id, bool visible);
    void showDock(const QString &id);
    void hideDock(const QString &id);

private:
    QMainWindow *m_host = nullptr;
    QMap<QString, QDockWidget *> m_docks;
};

#endif // DISP3DDOCKMANAGER_H
