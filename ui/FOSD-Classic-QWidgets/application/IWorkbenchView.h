#pragma once
#include "FOSD-Classic-QWidgets/common/IView.h"
#include "FOSD-Classic-QWidgets/common/UiTypes.h"
namespace fosd {
namespace ui {
namespace classic {
class IWorkbenchView {
public:
    virtual ~IWorkbenchView() = default;
    virtual IView *panelView(PanelId panel) const = 0;
    virtual void selectPage(int index) = 0;
    virtual void showPanel(PanelId panel) = 0;
    virtual void closePanel(PanelId panel) = 0;
    virtual void windowAction(PanelId panel, WindowAction action) = 0;
    virtual void applyTheme(bool dark) = 0;
    virtual void setModelAvailable(bool) {}
    virtual QString chooseFile(bool save, const QString &title, const QString &filter) = 0;
};
} // namespace classic
} // namespace ui
} // namespace fosd
