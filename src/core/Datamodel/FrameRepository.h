#pragma once
#include "FrameData.h"
#include <QReadWriteLock>

namespace fosd {
namespace protocol { namespace detail { class ProtocolWorker; } }
namespace core {

// Consumers receive shared_ptr<const FrameRepository>; no public write API.
class FrameRepository final {
public:
    FrameSnapshotPtr latest() const;
private:
    friend class fosd::protocol::detail::ProtocolWorker;
    FrameSnapshotPtr publish(FrameSnapshot snapshot);
    mutable QReadWriteLock m_lock;
    FrameSnapshotPtr m_latest;
    quint64 m_revision = 0;
};
using FrameRepositoryPtr = std::shared_ptr<const FrameRepository>;

} } // namespace fosd::core
