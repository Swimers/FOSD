#pragma once
#include "ModelData.h"
#include <QReadWriteLock>

namespace fosd {
namespace Calculation {
namespace detail {
class CalculationWorker;
}
} // namespace Calculation

namespace core {
class CalculationRepository final {
public:
    CalculationResultPtr latest() const;

private:
    friend class fosd::Calculation::detail::CalculationWorker;
    CalculationResultPtr publish(CalculationResult result);
    mutable QReadWriteLock m_lock;
    CalculationResultPtr m_latest;
    quint64 m_revision = 0;
};
using CalculationRepositoryPtr = std::shared_ptr<const CalculationRepository>;

} // namespace core
} // namespace fosd
