#pragma once
#include "oldmainwindow.h"
#include "../contracts/IWorkbenchView.h"
#include <QMap>
class QDockWidget;
class QStackedWidget;
namespace fosd {
namespace ui {
namespace classic {
class RenderHostWidget;
// Composition/layout only. Every form has its own view binding and presenter.
class WorkbenchView : public MainWindowView, public IWorkbenchView {
    Q_OBJECT
public:
    explicit WorkbenchView(QWidget *parent = nullptr);
    ~WorkbenchView() override;
    IView *panelView(PanelId panel) const override;
    QWidget *panelWidget(PanelId panel) const;
    RenderHostWidget *renderHost() const;
    void selectPage(int index) override;
    void showPanel(PanelId panel) override;
    void closePanel(PanelId panel) override;
    void windowAction(PanelId panel, WindowAction action) override;
    void applyTheme(bool dark) override;
    QString chooseFile(bool save, const QString &title, const QString &filter) override;

private:
    QMap<int, QWidget *> m_panels;
    QMap<int, QDockWidget *> m_docks;
    QStackedWidget *m_pages = nullptr;
};
} // namespace classic
} // namespace ui
} // namespace fosd
