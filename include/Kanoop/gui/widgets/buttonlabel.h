#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QHBoxLayout;
class QToolButton;
class QLabel;

/**
 * @brief Composite widget combining a QLabel with a QToolButton.
 *
 * ButtonLabel places a text label and a tool button side-by-side.  The button
 * can carry separate active and inactive icons and fires a clicked() signal.
 * Colors and alignment are customizable.
 */
class LIBKANOOPGUI_EXPORT ButtonLabel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct an empty ButtonLabel.
     * @param parent Optional QWidget parent
     */
    explicit ButtonLabel(QWidget *parent = nullptr);

    /**
     * @brief Construct with initial label text.
     * @param text Initial label text
     * @param parent Optional QWidget parent
     */
    explicit ButtonLabel(const QString& text, QWidget *parent = nullptr);

    /**
     * @brief Return the current label text.
     * @return Label text string
     */
    QString text() const { return _text; }

    /**
     * @brief Set the label text.
     * @param text New label text
     */
    void setText(const QString& text);

    /**
     * @brief Set the button icons for active and inactive states.
     * @param activeIcon Icon shown when isActive() is true
     * @param inactiveIcon Icon shown when isActive() is false (optional)
     */
    void setIcon(const QIcon& activeIcon, const QIcon& inactiveIcon = QIcon());

    /**
     * @brief Set the horizontal alignment of the button within the layout.
     * @param alignment Qt alignment flags
     */
    void setButtonAlignment(Qt::Alignment alignment);

    /**
     * @brief Set the active state and update the displayed icon.
     * @param active true to show the active icon
     */
    void setActive(bool active);

    /**
     * @brief Return whether the button is in the active state.
     * @return true if active
     */
    bool isActive() const { return _active; }

    /**
     * @brief Set the foreground (text) color.
     * @param color Foreground color
     */
    void setForegroundColor(const QColor& color);

    /**
     * @brief Set the background color.
     * @param color Background color
     */
    void setBackgroundColor(const QColor& color);

private:
    void commonInit();
    void makeStyleSheet();
    void relayout();

    QHBoxLayout* _layout = nullptr;
    QLabel* _label = nullptr;
    QToolButton* _button = nullptr;
    QIcon _activeIcon;
    QIcon _inactiveIcon;
    bool _active = false;
    Qt::Alignment _buttonAlignment = Qt::AlignRight;
    QString _text;
    QColor _backgroundColor;
    QColor _foregroundColor;

signals:
    /** @brief Emitted when the tool button is clicked. */
    void clicked();
    /** @brief Emitted when the active state changes. */
    void activeChanged();

private slots:
    void onButtonClicked();
};

#endif // BUTTONLABEL_H
