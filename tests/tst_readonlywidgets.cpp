#include <QTest>
#include <QKeyEvent>
#include <QList>
#include <QPoint>
#include <QPointF>
#include <QWheelEvent>
#include <QWidget>

#include <Kanoop/gui/widgets/checkbox.h>
#include <Kanoop/gui/widgets/slider.h>

/*
 * Known gap, headless.
 *
 * A read-only CheckBox must not swallow a dialog's default-button key. The keys
 * that activate a button come from the platform theme's ButtonPressKeys hint; the
 * offscreen plugin reports {Space, Select}, while a desktop session such as GNOME
 * adds Return and Enter. The Return case therefore cannot be exercised in this
 * environment at all. What is asserted here is the mechanism the fix relies on:
 * the key event is ignored and propagates to the parent widget. On a platform whose
 * hint includes Return, that is what stops the checkbox eating it.
 */

namespace
{

/**
 * @brief Parent widget that records the key presses that reach it.
 */
class KeyRecorder : public QWidget
{
public:
    QList<int> keys;

protected:
    virtual void keyPressEvent(QKeyEvent* event) override
    {
        keys.append(event->key());
        QWidget::keyPressEvent(event);
    }
};

const int SliderStart = 50;
const int SliderMinimum = 0;
const int SliderMaximum = 100;

void configure(Slider* slider)
{
    slider->setRange(SliderMinimum, SliderMaximum);
    slider->setPageStep(10);
    slider->setSingleStep(1);
    slider->setValue(SliderStart);
}

/**
 * @brief Send a wheel notch to a widget.
 *
 * QTest has no wheel helper, and the real delivery path is by cursor position, so
 * the event is constructed and sent directly to the widget under test.
 */
void sendWheel(QWidget* widget, int degrees)
{
    QWheelEvent event(QPointF(widget->width() / 2, widget->height() / 2),
                      widget->mapToGlobal(QPointF(widget->width() / 2, widget->height() / 2)),
                      QPoint(0, 0), QPoint(0, degrees),
                      Qt::NoButton, Qt::NoModifier, Qt::NoScrollPhase, false);
    QCoreApplication::sendEvent(widget, &event);
}

// A key of NoKey means the wheel rather than a key press.
const int NoKey = 0;

/**
 * @brief Populate the shared input table used by the slider cases.
 *
 * writableExpected is the value a slider that is not read-only lands on;
 * AnyOtherValue means only that it moved.
 */
const int AnyOtherValue = -1;

void addSliderInputRows()
{
    QTest::addColumn<int>("key");
    QTest::addColumn<int>("writableExpected");

    QTest::newRow("wheel")    << NoKey             << AnyOtherValue;
    QTest::newRow("Right")    << (int)Qt::Key_Right    << SliderStart + 1;
    QTest::newRow("Left")     << (int)Qt::Key_Left     << SliderStart - 1;
    QTest::newRow("PageUp")   << (int)Qt::Key_PageUp   << SliderStart + 10;
    QTest::newRow("PageDown") << (int)Qt::Key_PageDown << SliderStart - 10;
    QTest::newRow("Home")     << (int)Qt::Key_Home     << SliderMinimum;
    QTest::newRow("End")      << (int)Qt::Key_End      << SliderMaximum;
}

void applyInput(Slider* slider, int key)
{
    if(key == NoKey) {
        sendWheel(slider, 120);
    }
    else {
        QTest::keyClick(slider, (Qt::Key)key);
    }
}

}

class TstReadOnlyWidgets : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Space toggles a checkbox that is not read-only.
     *
     * Control for the read-only case. Every "did not change" assertion below is a
     * negative property and is equally true of a harness whose key never lands.
     */
    void checkBox_writable_spaceToggles()
    {
        CheckBox box;
        box.setChecked(false);
        QTest::keyClick(&box, Qt::Key_Space);
        QCOMPARE(box.isChecked(), true);
    }

    /**
     * @brief Space leaves a read-only checkbox alone.
     */
    void checkBox_readOnly_spaceDoesNotToggle()
    {
        CheckBox box;
        box.setChecked(false);
        box.setReadOnly(true);
        QTest::keyClick(&box, Qt::Key_Space);
        QCOMPARE(box.isChecked(), false);
    }

    /**
     * @brief The suppressed key still reaches the parent.
     *
     * ignore() is the chosen mechanism precisely so a read-only checkbox does not
     * eat a key its dialog wants. Asserting only that the box did not toggle would
     * also pass on an implementation that accepted and discarded the event.
     */
    void checkBox_readOnly_keyReachesTheParent()
    {
        KeyRecorder parent;
        CheckBox* box = new CheckBox(&parent);
        box->setReadOnly(true);
        parent.show();

        QTest::keyClick(box, Qt::Key_Space);

        QVERIFY2(parent.keys.contains(Qt::Key_Space),
                 qPrintable(QString("parent saw %1 key presses").arg(parent.keys.count())));
        QCOMPARE(box->isChecked(), false);
    }

    /**
     * @brief Control: each input moves a slider that is not read-only.
     *
     * Every "did not move" assertion below is a negative property and is equally
     * true of a harness whose input never lands. These rows are the same inputs on
     * the same widget class with the flag off.
     */
    void slider_writable_everyInputMoves_data() { addSliderInputRows(); }

    void slider_writable_everyInputMoves()
    {
        QFETCH(int, key);
        QFETCH(int, writableExpected);

        Slider slider(Qt::Horizontal);
        configure(&slider);
        slider.show();

        applyInput(&slider, key);

        if(writableExpected == AnyOtherValue) {
            QVERIFY2(slider.value() != SliderStart,
                     qPrintable(QString("input did not reach the writable slider; value %1")
                                .arg(slider.value())));
        }
        else {
            QCOMPARE(slider.value(), writableExpected);
        }
    }

    /**
     * @brief No input moves a read-only slider.
     *
     * One row per input, so a partial fix names the input that is still live
     * instead of stopping at the first one. Each row builds its own slider, so no
     * leaked write from an earlier input can make a later number nonsense.
     */
    void slider_readOnly_noInputMoves_data() { addSliderInputRows(); }

    void slider_readOnly_noInputMoves()
    {
        QFETCH(int, key);

        Slider slider(Qt::Horizontal);
        configure(&slider);
        slider.setReadOnly(true);
        slider.show();

        applyInput(&slider, key);

        QCOMPARE(slider.value(), SliderStart);
    }

    /**
     * @brief Clearing the flag restores ordinary behaviour.
     */
    void slider_readOnlyCleared_movesAgain()
    {
        Slider slider(Qt::Horizontal);
        configure(&slider);
        slider.setReadOnly(true);
        slider.show();

        QTest::keyClick(&slider, Qt::Key_Right);
        QCOMPARE(slider.value(), SliderStart);

        slider.setReadOnly(false);
        QTest::keyClick(&slider, Qt::Key_Right);
        QCOMPARE(slider.value(), SliderStart + 1);
    }

    /**
     * @brief Neither class disables itself or drops focus to become read-only.
     *
     * A disabled widget receives no key events, and its text stops being copyable.
     */
    void readOnlyWidgets_stayEnabledAndFocusable()
    {
        CheckBox box;
        box.setReadOnly(true);
        QVERIFY(box.isEnabled());
        QVERIFY(box.focusPolicy() != Qt::NoFocus);

        Slider slider(Qt::Horizontal);
        slider.setReadOnly(true);
        QVERIFY(slider.isEnabled());
        QVERIFY(slider.focusPolicy() != Qt::NoFocus);
    }
};

QTEST_MAIN(TstReadOnlyWidgets)
#include "tst_readonlywidgets.moc"
