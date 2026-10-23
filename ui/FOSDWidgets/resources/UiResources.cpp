#include "UiResources.h"
#include <QResource>
void initializeFosdUiResources() {
    // Forms are constructed only on the GUI thread.
    static bool initialized = false;
    if (!initialized) { Q_INIT_RESOURCE(legacy_ui); initialized = true; }
}
