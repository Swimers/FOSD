#pragma once
#include "../../../common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class VisualizationPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    VisualizationPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr,
                           QObject *parent = nullptr);

protected:
    bool localAction(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
