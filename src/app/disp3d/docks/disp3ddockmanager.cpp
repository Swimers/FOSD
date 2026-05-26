#include "disp3ddockmanager.h"

#include "disp3ddockpanel.h"

#include <QAction>
#include <QDockWidget>
#include <QMainWindow>

Disp3DDockManager::Disp3DDockManager(QMainWindow *host, QObject *parent)
    : QObject(parent),
      m_host(host)
{
}

void Disp3DDockManager::registerDock(const QString &id,
                                     QDockWidget *dock,
                                     QAction *showAction,
                                     bool initiallyVisible,
                                     Qt::DockWidgetArea area)
{
    if (!m_host || !dock || id.isEmpty()) {
        return;
    }

    m_docks.insert(id, dock);

    dock->setParent(m_host);
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFloating(false);
    m_host->addDockWidget(area, dock);

    dock->setVisible(initiallyVisible);

    if (showAction) {
        QObject::connect(showAction, &QAction::triggered, dock, [dock]() {
            dock->setFloating(false);
            dock->setVisible(true);
            dock->raise();
        });
    }
}

void Disp3DDockManager::registerPanel(Disp3DDockPanel *panel, QAction *showAction)
{
    if (!panel) {
        return;
    }

    registerDock(panel->dockId(),
                 panel->dockWidget(),
                 showAction,
                 panel->defaultVisible(),
                 panel->defaultArea());
}

QDockWidget *Disp3DDockManager::dock(const QString &id) const
{
    return m_docks.value(id, nullptr);
}

void Disp3DDockManager::setDockVisible(const QString &id, bool visible)
{
    QDockWidget *target = dock(id);
    if (!target) {
        return;
    }

    target->setVisible(visible);
    if (visible) {
        target->raise();
    }
}

void Disp3DDockManager::showDock(const QString &id)
{
    setDockVisible(id, true);
}

void Disp3DDockManager::hideDock(const QString &id)
{
    setDockVisible(id, false);
}
