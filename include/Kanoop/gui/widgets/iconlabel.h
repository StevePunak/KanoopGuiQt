#ifndef ICONLABEL_H
#define ICONLABEL_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QLabel;

/**
 * @brief Composite widget showing a text label alongside an icon.
 *
 * IconLabel places a QLabel for text and a QLabel for a QIcon side by side.
 * The icon alignment within the layout can be controlled independently of the
 * text alignment.
 */
class LIBKANOOPGUI_EXPORT IconLabel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct an empty IconLabel.
     * @param parent Optional QWidget parent
     */
    explicit IconLabel(QWidget *parent = nullptr);

    /**
     * @brief Construct with text only.
     * @param text Initial label text
     * @param parent Optional QWidget parent
     */
    explicit IconLabel(const QString& text, QWidget *parent = nullptr);

    /**
     * @brief Construct with text and an icon.
     * @param text Initial label text
     * @param icon Icon to display alongside the text
     * @param parent Optional QWidget parent
     */
    explicit IconLabel(const QString& text, const QIcon& icon, QWidget *parent = nullptr);

    /**
     * @brief Return the current label text.
     * @return Text string
     */
    QString text() const;

    /**
     * @brief Set the label text.
     * @param text New text string
     */
    void setText(const QString& text);

    /**
     * @brief Return the current icon.
     * @return Icon displayed in the widget
     */
    QIcon icon() const;

    /**
     * @brief Set the icon.
     * @param icon New icon to display
     */
    void setIcon(const QIcon& icon);

    /**
     * @brief Set the alignment of the icon within the layout.
     * @param alignment Qt alignment flags
     */
    void setIconAlignment(Qt::Alignment alignment);

private:
    void createLayout(const QString& text, const QIcon& icon);

    QLabel* _label = nullptr;
    QLabel* _iconLabel = nullptr;

signals:

private slots:

};

#endif // ICONLABEL_H
