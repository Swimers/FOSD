#pragma once
#include "../../../common/PanelPresenter.h"
namespace fosd { namespace ui { namespace classic {
class PlotPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    PlotPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
protected:
    bool localAction(const UiRequest &request) override;
};
} } }
