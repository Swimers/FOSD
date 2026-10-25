#include "FormBinding.h"
#include "../controls/switchcontrol.h"
#include "../controls/qcustomplot.h"
#include <QAbstractButton>
#include <QAbstractSlider>
#include <QAction>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QTableView>
#include <QTableWidget>
#include <cmath>

namespace fosd {
namespace ui {
namespace classic {
namespace {
bool named(const QObject *object) {
    return !object->objectName().isEmpty() && !object->objectName().startsWith("qt_");
}
} // namespace
FormBinding::FormBinding(PanelId panel, QWidget *widget, QObject *parent)
    : ViewEvents(parent), m_panel(panel), m_widget(widget) {
    widget->setProperty("fosdPanelRoot", true);
    for (auto *button : widget->findChildren<QAbstractButton *>())
        if (named(button))
            connect(button, &QAbstractButton::clicked, this, &FormBinding::relay);
    for (auto *action : widget->findChildren<QAction *>())
        if (named(action))
            connect(action, &QAction::triggered, this, &FormBinding::relay);
    for (auto *combo : widget->findChildren<QComboBox *>())
        if (named(combo))
            connect(combo, QOverload<int>::of(&QComboBox::activated), this, &FormBinding::relay);
    for (auto *spin : widget->findChildren<QAbstractSpinBox *>())
        if (named(spin))
            connect(spin, &QAbstractSpinBox::editingFinished, this, &FormBinding::relay);
    for (auto *slider : widget->findChildren<QAbstractSlider *>())
        if (named(slider))
            connect(slider, &QAbstractSlider::valueChanged, this, &FormBinding::relay);
    for (auto *toggle : widget->findChildren<SwitchControl *>())
        connect(toggle, &SwitchControl::toggled, this, &FormBinding::relay);
    for (auto *box : widget->findChildren<QDialogButtonBox *>()) {
        connect(box, &QDialogButtonBox::accepted, this, &FormBinding::accepted);
        connect(box, &QDialogButtonBox::rejected, this, &FormBinding::rejected);
    }
}

QWidget *FormBinding::widget() const {
    return m_widget.data();
}

bool FormBinding::owns(const QObject *object) const {
    for (const QObject *parent = object; parent; parent = parent->parent()) {
        if (parent == m_widget.data())
            return true;
        if (parent->property("fosdPanelRoot").toBool())
            return false;
    }
    return false;
}

void FormBinding::relay() {
    if (sender() && named(sender()) && owns(sender()))
        emit actionTriggered(m_panel, sender()->objectName());
}

void FormBinding::accepted() {
    emit actionTriggered(m_panel, QStringLiteral("accepted"));
}

void FormBinding::rejected() {
    emit actionTriggered(m_panel, QStringLiteral("rejected"));
}

QVariantMap FormBinding::values() const {
    QVariantMap fields;
    if (!m_widget)
        return fields;
    for (auto *edit : m_widget->findChildren<QLineEdit *>())
        if (owns(edit) && named(edit) && !edit->isReadOnly())
            fields[edit->objectName()] = edit->text();
    for (auto *edit : m_widget->findChildren<QPlainTextEdit *>())
        if (owns(edit) && named(edit) && !edit->isReadOnly())
            fields[edit->objectName()] = edit->toPlainText();
    for (auto *combo : m_widget->findChildren<QComboBox *>()) {
        if (!owns(combo) || !named(combo))
            continue;
        QVariantMap value;
        value["index"] = combo->currentIndex();
        value["text"] = combo->currentText();
        fields[combo->objectName()] = value;
    }
    for (auto *button : m_widget->findChildren<QAbstractButton *>())
        if (owns(button) && named(button) && button->isCheckable())
            fields[button->objectName()] = button->isChecked();
    for (auto *spin : m_widget->findChildren<QSpinBox *>())
        if (owns(spin) && named(spin))
            fields[spin->objectName()] = spin->value();
    for (auto *spin : m_widget->findChildren<QDoubleSpinBox *>())
        if (owns(spin) && named(spin))
            fields[spin->objectName()] = spin->value();
    for (auto *toggle : m_widget->findChildren<SwitchControl *>())
        if (owns(toggle))
            fields[toggle->objectName()] = toggle->isToggled();
    for (auto *slider : m_widget->findChildren<QAbstractSlider *>())
        if (owns(slider) && named(slider))
            fields[slider->objectName()] = slider->value();
    for (auto *table : m_widget->findChildren<QTableView *>()) {
        if (!owns(table) || !named(table) || !table->selectionModel())
            continue;
        QVariantList selected;
        for (const auto &index : table->selectionModel()->selectedRows())
            selected.append(index.row());
        fields[table->objectName() + ".selectedRows"] = selected;
    }
    return fields;
}

void FormBinding::applyFields(const QVariantMap &fields) {
    if (!m_widget)
        return;
    for (auto it = fields.constBegin(); it != fields.constEnd(); ++it) {
        QObject *object = nullptr;
        for (auto *candidate : m_widget->findChildren<QObject *>(it.key()))
            if (owns(candidate)) {
                object = candidate;
                break;
            }
        if (!object)
            continue;
        QSignalBlocker block(object);
        const auto value = it.value();
        if (auto *combo = qobject_cast<QComboBox *>(object)) {
            const auto data = value.toMap();
            if (data.contains("items")) {
                combo->clear();
                combo->addItems(data["items"].toStringList());
            }
            if (data.contains("index"))
                combo->setCurrentIndex(data["index"].toInt());
            else if (data.contains("text"))
                combo->setCurrentText(data["text"].toString());
        } else if (auto *edit = qobject_cast<QLineEdit *>(object))
            edit->setText(value.toString());
        else if (auto *edit = qobject_cast<QPlainTextEdit *>(object))
            edit->setPlainText(value.toString());
        else if (auto *label = qobject_cast<QLabel *>(object))
            label->setText(value.toString());
        else if (auto *button = qobject_cast<QAbstractButton *>(object)) {
            if (value.type() == QVariant::Bool && button->isCheckable())
                button->setChecked(value.toBool());
            else
                button->setText(value.toString());
        } else if (auto *spin = qobject_cast<QSpinBox *>(object))
            spin->setValue(value.toInt());
        else if (auto *spin = qobject_cast<QDoubleSpinBox *>(object))
            spin->setValue(value.toDouble());
        else if (auto *toggle = qobject_cast<SwitchControl *>(object))
            toggle->setToggle(value.toBool());
        else if (auto *slider = qobject_cast<QAbstractSlider *>(object))
            slider->setValue(value.toInt());
        else if (auto *plot = qobject_cast<QCustomPlot *>(object)) {
            const auto data = value.toMap();
            const auto xs = data["x"].toList(), ys = data["y"].toList();
            if (xs.size() != ys.size())
                continue;
            QVector<double> x, y;
            for (int index = 0; index < xs.size(); ++index) {
                bool validX = false, validY = false;
                const double xx = xs[index].toDouble(&validX), yy = ys[index].toDouble(&validY);
                if (validX && validY && std::isfinite(xx) && std::isfinite(yy)) {
                    x.append(xx);
                    y.append(yy);
                }
            }
            if (!plot->graphCount())
                plot->addGraph();
            plot->graph(0)->setData(x, y);
            plot->rescaleAxes();
            plot->replot();
        } else if (auto *table = qobject_cast<QTableView *>(object)) {
            const auto data = value.toMap();
            const auto rows = data["rows"].toList();
            const auto headers = data["headers"].toStringList();
            if (auto *items = qobject_cast<QTableWidget *>(table)) {
                items->setColumnCount(headers.size());
                items->setHorizontalHeaderLabels(headers);
                items->setRowCount(rows.size());
                for (int row = 0; row < rows.size(); ++row) {
                    const auto cells = rows[row].toList();
                    for (int column = 0; column < cells.size() && column < headers.size(); ++column)
                        items->setItem(row, column, new QTableWidgetItem(cells[column].toString()));
                }
            } else {
                auto *model = qobject_cast<QStandardItemModel *>(table->model());
                if (!model) {
                    model = new QStandardItemModel(table);
                    table->setModel(model);
                }
                model->clear();
                model->setHorizontalHeaderLabels(headers);
                for (const auto &row : rows) {
                    QList<QStandardItem *> cells;
                    for (const auto &cell : row.toList())
                        cells.append(new QStandardItem(cell.toString()));
                    model->appendRow(cells);
                }
            }
            table->setEditTriggers(QAbstractItemView::NoEditTriggers);
            table->horizontalHeader()->setStretchLastSection(true);
        }
    }
}

void FormBinding::clearSensitiveFields() {
    if (!m_widget)
        return;
    for (auto *edit : m_widget->findChildren<QLineEdit *>())
        if (owns(edit) && edit->echoMode() != QLineEdit::Normal) {
            QSignalBlocker block(edit);
            edit->clear();
        }
}

void FormBinding::clearInputs() {
    if (!m_widget)
        return;
    for (auto *edit : m_widget->findChildren<QLineEdit *>())
        if (owns(edit) && !edit->isReadOnly()) {
            QSignalBlocker block(edit);
            edit->clear();
        }
    for (auto *edit : m_widget->findChildren<QPlainTextEdit *>())
        if (owns(edit) && !edit->isReadOnly()) {
            QSignalBlocker block(edit);
            edit->clear();
        }
}

void FormBinding::showStatus(const QString &message) {
    if (!m_widget)
        return;
    m_widget->setProperty("presenterStatus", message);
    if (auto *window = qobject_cast<QMainWindow *>(m_widget.data()))
        window->statusBar()->showMessage(message);
}
} // namespace classic
} // namespace ui
} // namespace fosd
