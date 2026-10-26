#include "FOSD-Classic-QWidgets/apps/WorkbenchPresenter.h"
#include "FOSD-Classic-QWidgets/apps/WorkbenchView.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    fosd::ui::classic::WorkbenchView view;
    fosd::ui::classic::WorkbenchBackend backends;
    fosd::ui::classic::WorkbenchPresenter presenter(&view);

    presenter.setBackends(&backends);

    view.show();
    return app.exec();
}
