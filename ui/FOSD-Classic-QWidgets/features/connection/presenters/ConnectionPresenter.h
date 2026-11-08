#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class ConnectionPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    ConnectionPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr,
                        QObject *parent = nullptr);

protected:
    bool prepareRequest(UiRequest &request) override;
    bool localAction(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
