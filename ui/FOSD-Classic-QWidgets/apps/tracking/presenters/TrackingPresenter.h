#pragma once
#include "../../../common/PanelPresenter.h"
namespace fosd { namespace ui { namespace classic {
class TrackingPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    TrackingPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
protected:
    bool localAction(const UiRequest &request) override;
    bool prepareRequest(UiRequest &request) override;
};
} } }
