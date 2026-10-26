#include "CalculationRepository.h"
#include <utility>

namespace fosd {
namespace core {
void CalculationRepository::clear() {
    QWriteLocker lock(&m_lock);
    m_latest.reset();
}

CalculationResultPtr CalculationRepository::latest() const {
    QReadLocker lock(&m_lock);
    return m_latest;
}

CalculationResultPtr CalculationRepository::publish(CalculationResult result) {
    QWriteLocker lock(&m_lock);
    result.revision = ++m_revision;
    m_latest = std::make_shared<const CalculationResult>(std::move(result));
    return m_latest;
}

} // namespace core
} // namespace fosd
