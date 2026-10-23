#include "FOSDWidgets/views/WorkbenchView.h"
#include "FOSDWidgets/presenters/WorkbenchPresenter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    fosd::widgets::WorkbenchView window;
    fosd::widgets::WorkbenchPresenter presenter(&window);
    QObject::connect(&window, &fosd::widgets::WorkbenchView::actionTriggered,
                     &presenter, &fosd::widgets::WorkbenchPresenter::execute);
    window.show();
    return app.exec();
}
