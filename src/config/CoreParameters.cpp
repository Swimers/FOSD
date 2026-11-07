#include "CoreParameters.h"

namespace fosd {
namespace communication {
bool Endpoint::isValid() const {
    return !address.isNull() && port != 0;
}
} // namespace communication
namespace visualization {
SelectionStyleSettings::SelectionStyleSettings() {
    radiusMm = 7;
    color = {{0.0, 1.0, 1.0}};
    labelColor = {{0.0, 1.0, 1.0}};
}
} // namespace visualization
} // namespace fosd
