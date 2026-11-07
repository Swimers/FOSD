#pragma once
#include "visualization/VisualizationTypes.h"

namespace fosd {
namespace visualization {

// Creates/configures props only. Renderer ownership belongs to the manager.
class KeyPointCreator final {
public:
    static bool validateStyle(const KeyPointStyle &style,
                              QString *error = nullptr); // 检查 KeyPointStyle 有效性

    static KeyPointActors create(vtkIdType id, const std::array<double, 3> &position,
                                 const QString &label, const KeyPointStyle &style);
    static void configure(KeyPointActors &actors, const KeyPointStyle &style);
    static void setPosition(KeyPointActors &actors, const std::array<double, 3> &position,
                            const KeyPointStyle &style);
};
} // namespace visualization
} // namespace fosd
