#include "FrameRepository.h"
#include <utility>
namespace fosd { namespace core {
FrameSnapshotPtr FrameRepository::latest() const {
    QReadLocker locker(&m_lock);
    return m_latest;
}
FrameSnapshotPtr FrameRepository::publish(FrameSnapshot snapshot) {
    QWriteLocker locker(&m_lock);
    snapshot.revision = ++m_revision;
    m_latest = std::make_shared<const FrameSnapshot>(std::move(snapshot));
    return m_latest;
}
} } // namespace fosd::core
