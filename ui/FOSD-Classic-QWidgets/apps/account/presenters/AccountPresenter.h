#pragma once
#include "../../../common/PanelPresenter.h"
namespace fosd { namespace ui { namespace classic {
class AccountPresenter final : public PanelPresenter {
    Q_OBJECT
public:
    AccountPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
protected:
    bool localAction(const UiRequest &request) override;
    bool isSensitiveAction(const QString &action) const override;
};
} } }
