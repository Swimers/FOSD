#include "FOSD-Classic-QWidgets/features/data/presenters/HardwareDataPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
void HardwareDataPresenter::initialize() {
    presentOwn(m_panel, {{"checkBox", true}});
    poll();
}
bool HardwareDataPresenter::handleAction(const QString &action, const QVariantMap &fields,
                                         QString &message, QVariantMap &) {
    if (action != "checkBox")
        return false;
    m_refresh = fields.value(action).toBool();
    m_presented.reset();
    message = tr("已更新数据刷新状态");
    poll();
    return true;
}
void HardwareDataPresenter::poll() {
    if (!m_model || !m_refresh)
        return;
    auto data = m_model->hardware();
    if (!data || !data->sourceFrame || data == m_presented)
        return;
    QVariantList rows;
    for (int i = 0; i < data->selectedStrains.size(); ++i)
        rows.append(QVariant(
            QVariantList{i + 1, data->sourceIndices.value(i) + 1, data->selectedStrains[i]}));
    presentOwn(
        PanelId::HardwareData,
        {{"tableWidget",
          QVariantMap{{"headers", QStringList{tr("IFE 输入"), tr("波长源序号"), tr("标定应变")}},
                      {"rows", rows}}}});
    m_presented = data;
}
} // namespace classic
} // namespace ui
} // namespace fosd
