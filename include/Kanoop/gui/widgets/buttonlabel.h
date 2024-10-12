#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QHBoxLayout;
class QToolButton;
class QLabel;
class LIBKANOOPGUI_EXPORT ButtonLabel : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonLabel(QWidget *parent = nullptr);
    explicit ButtonLabel(const QString& text, QWidget *parent = nullptr);

    QString text() const { return _text; }
    void setText(const QString& text);

    void setIcon(const QIcon& activeIcon, const QIcon& inactiveIcon = QIcon());

    void setButtonAlignment(Qt::Alignment alignment);

    void setActive(bool active);
    bool isActive() const { return _active; }

private:
    void commonInit();
    void relayout();

    QHBoxLayout* _layout = nullptr;
    QLabel* _label = nullptr;
    QToolButton* _button = nullptr;
    QIcon _activeIcon;
    QIcon _inactiveIcon;
    bool _active = false;
    Qt::Alignment _buttonAlignment = Qt::AlignRight;
    QString _text;

signals:
    void clicked();
    void activeChanged();

private slots:
    void onButtonClicked();
};

#endif // BUTTONLABEL_H
