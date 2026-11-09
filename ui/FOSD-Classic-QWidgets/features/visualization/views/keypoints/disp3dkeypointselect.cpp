#include "FOSD-Classic-QWidgets/features/visualization/views/keypoints/disp3dkeypointselect.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_disp3dkeypointselect.h"
#include <QItemSelectionModel>
#include <QSet>
namespace fosd {
namespace ui {
namespace classic {
KeyPointSelectView::KeyPointSelectView(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::disp3dkeypointselect) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_binding = new FormBinding(PanelId::KeyPointSelect, this, this);
}

KeyPointSelectView::~KeyPointSelectView() = default;
ViewEvents *KeyPointSelectView::events() const {
    return m_binding;
}

QVariantMap KeyPointSelectView::values() const {
    auto fields = m_binding->values();
    QVariantList ids;
    const auto *table = m_ui->tablew_PointPara;
    for (const auto &row : table->selectionModel()->selectedRows()) {
        const auto *item = table->item(row.row(), 0);
        if (item)
            ids.append(item->text().toLongLong());
    }
    // Capture IDs at click time: live coordinate refreshes must not retarget a deletion.
    fields["tablew_PointPara.selectedIds"] = ids;
    return fields;
}

void KeyPointSelectView::applyFields(const QVariantMap &fields) {
    QSet<qlonglong> selected;
    auto *table = m_ui->tablew_PointPara;
    const QString candidate = m_ui->cbox_KeyPointID->currentText();
    if (fields.contains("tablew_PointPara"))
        for (const auto &row : table->selectionModel()->selectedRows()) {
            const auto *item = table->item(row.row(), 0);
            if (item)
                selected.insert(item->text().toLongLong());
        }
    m_binding->applyFields(fields);
    if (fields.contains("cbox_KeyPointID")) {
        const int index = m_ui->cbox_KeyPointID->findText(candidate);
        if (index >= 0)
            m_ui->cbox_KeyPointID->setCurrentIndex(index);
    }
    if (fields.contains("tablew_PointPara")) {
        table->clearSelection();
        for (int row = 0; row < table->rowCount(); ++row)
            if (table->item(row, 0) && selected.contains(table->item(row, 0)->text().toLongLong()))
                table->selectionModel()->select(table->model()->index(row, 0),
                                                QItemSelectionModel::Select |
                                                    QItemSelectionModel::Rows);
    }
}

void KeyPointSelectView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void KeyPointSelectView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

} // namespace classic
} // namespace ui
} // namespace fosd
