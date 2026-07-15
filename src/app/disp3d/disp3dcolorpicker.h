#ifndef DISP3DCOLORPICKER_H
#define DISP3DCOLORPICKER_H

#include <QDialog>
#include <QVector>
#include <QPoint>
#include <QPointer>

class QSlider;
class QLabel;
class QPushButton;
class QSpinBox;

namespace Ui {
class Disp3DColorPicker;
}

class Disp3DColorPicker : public QDialog
{
    Q_OBJECT

public:
    ~Disp3DColorPicker();

    static Disp3DColorPicker *instance(QWidget *parent = nullptr);

    QVector<float> selectedColor() const;

signals:
    void colorSelected(const QVector<float> &rgb);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onSliderChanged();
    void onSpinBoxChanged(int value);
    void onPickColorToggled(bool checked);
    void onAccepted();

private:
    explicit Disp3DColorPicker(QWidget *parent = nullptr);

    void updatePreview();
    void updateSlidersFromColor(const QColor &color);
    void startScreenPick();
    void stopScreenPick();
    bool eventFilter(QObject *obj, QEvent *event) override;

    Ui::Disp3DColorPicker *ui = nullptr;

    static QPointer<Disp3DColorPicker> s_instance;

    bool m_picking = false;
    QVector<float> m_result; // RGB float [0.0, 1.0]
};

#endif // DISP3DCOLORPICKER_H
