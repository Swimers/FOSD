#ifndef DISP3DDOCKPANEL_H
#define DISP3DDOCKPANEL_H

#include <QString>
#include <Qt>

class QDockWidget;

namespace Disp3DDockIds {
const char Model[] = "model";
const char View[] = "view";
const char Information[] = "information";
}

class Disp3DDockPanel
{
public:
    virtual ~Disp3DDockPanel() {}

    virtual QString dockId() const = 0;
    virtual QDockWidget *dockWidget() const = 0;
    virtual Qt::DockWidgetArea defaultArea() const { return Qt::RightDockWidgetArea; }
    virtual bool defaultVisible() const { return true; }
};

#endif // DISP3DDOCKPANEL_H
