#pragma once
#include "../../../common/PanelPresenter.h"
namespace fosd { namespace ui { namespace classic {
class MainWindowPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    MainWindowPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
protected:
    bool localAction(const UiRequest &request) override;
};
} } }
