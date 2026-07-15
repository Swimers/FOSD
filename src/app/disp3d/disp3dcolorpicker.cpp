#include "disp3dcolorpicker.h"
#include "ui_disp3dcolorpicker.h"

#include <QCloseEvent>
#include <QColor>
#include <QCursor>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QScreen>

QPointer<Disp3DColorPicker> Disp3DColorPicker::s_instance;

Disp3DColorPicker *Disp3DColorPicker::instance(QWidget *parent)
{
    if (!s_instance) {
        s_instance = new Disp3DColorPicker(parent);
    }
    // 如果已存在则置顶激活
    s_instance->raise();
    s_instance->activateWindow();
    return s_instance;
}

Disp3DColorPicker::Disp3DColorPicker(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      ui(new Ui::Disp3DColorPicker)
{
    ui->setupUi(this);

    setWindowTitle(tr("调色板"));
    setFixedSize(sizeHint());

    // RGB 滑块范围 0-255
    ui->sliderR->setRange(0, 255);
    ui->sliderG->setRange(0, 255);
    ui->sliderB->setRange(0, 255);

    // SpinBox 范围 0-255
    ui->spinR->setRange(0, 255);
    ui->spinG->setRange(0, 255);
    ui->spinB->setRange(0, 255);

    // 默认颜色：灰色 (128, 128, 128)
    ui->sliderR->setValue(128);
    ui->sliderG->setValue(128);
    ui->sliderB->setValue(128);

    // 滑块 ↔ SpinBox 双向同步
    connect(ui->sliderR, &QSlider::valueChanged, this, &Disp3DColorPicker::onSliderChanged);
    connect(ui->sliderG, &QSlider::valueChanged, this, &Disp3DColorPicker::onSliderChanged);
    connect(ui->sliderB, &QSlider::valueChanged, this, &Disp3DColorPicker::onSliderChanged);
    connect(ui->spinR, QOverload<int>::of(&QSpinBox::valueChanged), this, &Disp3DColorPicker::onSpinBoxChanged);
    connect(ui->spinG, QOverload<int>::of(&QSpinBox::valueChanged), this, &Disp3DColorPicker::onSpinBoxChanged);
    connect(ui->spinB, QOverload<int>::of(&QSpinBox::valueChanged), this, &Disp3DColorPicker::onSpinBoxChanged);

    // 取色器按钮
    ui->btnPick->setCheckable(true);
    connect(ui->btnPick, &QPushButton::toggled, this, &Disp3DColorPicker::onPickColorToggled);

    // OK / Cancel
    connect(ui->btnBox, &QDialogButtonBox::accepted, this, &Disp3DColorPicker::onAccepted);
    connect(ui->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updatePreview();
}

Disp3DColorPicker::~Disp3DColorPicker()
{
    delete ui;
}

QVector<float> Disp3DColorPicker::selectedColor() const
{
    return m_result;
}

void Disp3DColorPicker::closeEvent(QCloseEvent *event)
{
    stopScreenPick();
    s_instance = nullptr;
    event->accept();
}

// ─── 滑块拖动 → 同步 SpinBox + 更新预览 ───

void Disp3DColorPicker::onSliderChanged()
{
    QSlider *slider = qobject_cast<QSlider *>(sender());
    if (!slider) return;

    if (slider == ui->sliderR)
        ui->spinR->blockSignals(true),  ui->spinR->setValue(slider->value()),  ui->spinR->blockSignals(false);
    else if (slider == ui->sliderG)
        ui->spinG->blockSignals(true),  ui->spinG->setValue(slider->value()),  ui->spinG->blockSignals(false);
    else if (slider == ui->sliderB)
        ui->spinB->blockSignals(true),  ui->spinB->setValue(slider->value()),  ui->spinB->blockSignals(false);

    updatePreview();
}

// ─── SpinBox 改变 → 同步滑块 + 更新预览 ───

void Disp3DColorPicker::onSpinBoxChanged(int value)
{
    QSpinBox *spin = qobject_cast<QSpinBox *>(sender());
    if (!spin) return;

    if (spin == ui->spinR)
        ui->sliderR->blockSignals(true),  ui->sliderR->setValue(value),  ui->sliderR->blockSignals(false);
    else if (spin == ui->spinG)
        ui->sliderG->blockSignals(true),  ui->sliderG->setValue(value),  ui->sliderG->blockSignals(false);
    else if (spin == ui->spinB)
        ui->sliderB->blockSignals(true),  ui->sliderB->setValue(value),  ui->sliderB->blockSignals(false);

    updatePreview();
}

// ─── 更新颜色预览 ───

void Disp3DColorPicker::updatePreview()
{
    int r = ui->sliderR->value();
    int g = ui->sliderG->value();
    int b = ui->sliderB->value();

    QPixmap pix(ui->labelPreview->size());
    pix.fill(QColor(r, g, b));
    ui->labelPreview->setPixmap(pix);

    // 同时更新 RGB 浮点标签
    ui->labelRgbFloat->setText(QString("RGB(%1, %2, %3)")
        .arg(r / 255.0, 0, 'f', 2)
        .arg(g / 255.0, 0, 'f', 2)
        .arg(b / 255.0, 0, 'f', 2));

    // 十六进制
    ui->labelHex->setText(QString("#%1%2%3")
        .arg(r, 2, 16, QChar('0'))
        .arg(g, 2, 16, QChar('0'))
        .arg(b, 2, 16, QChar('0'))
        .toUpper());
}

// ─── 取色器 ───

void Disp3DColorPicker::onPickColorToggled(bool checked)
{
    if (checked) {
        startScreenPick();
    } else {
        stopScreenPick();
    }
}

void Disp3DColorPicker::startScreenPick()
{
    m_picking = true;
    ui->btnPick->setText(tr("取色中..."));
    setCursor(Qt::CrossCursor);
    grabMouse();
    qApp->installEventFilter(this);
}

void Disp3DColorPicker::stopScreenPick()
{
    if (!m_picking) return;
    m_picking = false;
    ui->btnPick->setChecked(false);
    ui->btnPick->setText(tr("取色器"));
    setCursor(Qt::ArrowCursor);
    releaseMouse();
    qApp->removeEventFilter(this);
}

bool Disp3DColorPicker::eventFilter(QObject *obj, QEvent *event)
{
    if (!m_picking)
        return QDialog::eventFilter(obj, event);

    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        QPoint globalPos = me->globalPos();

        QScreen *screen = QGuiApplication::screenAt(globalPos);
        if (!screen)
            screen = QGuiApplication::primaryScreen();
        if (!screen)
            return QDialog::eventFilter(obj, event);

        // 截取鼠标所在像素
        QPixmap pix = screen->grabWindow(0, globalPos.x(), globalPos.y(), 1, 1);
        if (!pix.isNull()) {
            QImage img = pix.toImage();
            QColor color = img.pixel(0, 0);
            updateSlidersFromColor(color);
        }

        return true;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        if (me->button() == Qt::LeftButton) {
            QPoint globalPos = me->globalPos();

            QScreen *screen = QGuiApplication::screenAt(globalPos);
            if (!screen)
                screen = QGuiApplication::primaryScreen();
            if (screen) {
                QPixmap pix = screen->grabWindow(0, globalPos.x(), globalPos.y(), 1, 1);
                if (!pix.isNull()) {
                    QImage img = pix.toImage();
                    QColor color = img.pixel(0, 0);
                    updateSlidersFromColor(color);
                }
            }

            stopScreenPick();
            return true;
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape) {
            stopScreenPick();
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}

// ─── 从 QColor 反填滑块 ───

void Disp3DColorPicker::updateSlidersFromColor(const QColor &color)
{
    ui->sliderR->blockSignals(true);
    ui->sliderG->blockSignals(true);
    ui->sliderB->blockSignals(true);
    ui->spinR->blockSignals(true);
    ui->spinG->blockSignals(true);
    ui->spinB->blockSignals(true);

    ui->sliderR->setValue(color.red());
    ui->sliderG->setValue(color.green());
    ui->sliderB->setValue(color.blue());
    ui->spinR->setValue(color.red());
    ui->spinG->setValue(color.green());
    ui->spinB->setValue(color.blue());

    ui->sliderR->blockSignals(false);
    ui->sliderG->blockSignals(false);
    ui->sliderB->blockSignals(false);
    ui->spinR->blockSignals(false);
    ui->spinG->blockSignals(false);
    ui->spinB->blockSignals(false);

    updatePreview();
}

// ─── OK 按钮 ───

void Disp3DColorPicker::onAccepted()
{
    m_result.clear();
    m_result.append(ui->sliderR->value() / 255.0f);
    m_result.append(ui->sliderG->value() / 255.0f);
    m_result.append(ui->sliderB->value() / 255.0f);

    emit colorSelected(m_result);
    accept();
}
