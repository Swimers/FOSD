#pragma once
#include "IPanelView.h"
#include "UiTypes.h"
namespace fosd { namespace widgets {
class IWorkbenchView {
public:
    virtual ~IWorkbenchView() = default;
    virtual IPanelView *panelView(PanelId panel) const = 0;
    virtual void selectPage(int index) = 0;
    virtual void showPanel(PanelId panel) = 0;
    virtual void closePanel(PanelId panel) = 0;
    virtual void windowAction(PanelId panel, WindowAction action) = 0;
    virtual void applyTheme(bool dark) = 0;
    virtual QString chooseFile(bool save, const QString &title, const QString &filter) = 0;
};
} }
