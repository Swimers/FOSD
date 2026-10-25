#pragma once
#include <QWidget>
#include <QPointer>

namespace fosd {
namespace ui {
namespace classic {
// Frontend-owned host; an application adapter can mount QVTK's widget here.
// Deliberately does not construct a renderer or depend on VTK runtime DLLs.
class RenderHostWidget final : public QWidget {
    Q_OBJECT
public:
    explicit RenderHostWidget(QWidget *parent = nullptr);
    void setSurface(QWidget *surface);
    QWidget *surface() const {
        return m_surface.data();
    }

private:
    QPointer<QWidget> m_surface;
};
} // namespace classic
} // namespace ui
} // namespace fosd
// Qt Designer promotion uses this unqualified name.
using RenderHostWidget = fosd::ui::classic::RenderHostWidget;
