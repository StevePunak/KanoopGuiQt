#ifndef ACCORDIONWIDGETPRIVATE_H
#define ACCORDIONWIDGETPRIVATE_H

#include <QPushButton>
#include <QWidget>


class DropDownButton;
class AccordionItem : public QWidget
{
    Q_OBJECT
public:
    explicit AccordionItem(const QString& title, QWidget *parent = nullptr);

    QWidget* centralWidget() const { return _centralWidget; }
    void setCentralWidget(QWidget* value) { _centralWidget = value; }
    void setExpanded(bool expanded);
    bool isExpanded() const;
    bool isCollapsed() const;
    void setTitle(const QString& title);

private:
    QWidget* _centralWidget = nullptr;
    DropDownButton* _button = nullptr;

private slots:
    void showHideCentralWidget(bool checked);

};

class DropDownButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DropDownButton(const QString& text, QWidget* parent = nullptr);

    void setExpanded(bool value);

private:
    virtual void paintEvent(QPaintEvent* event) override;

    QPixmap _down;
    QPixmap _up;
    bool _expanded = false;
};

#endif // ACCORDIONWIDGETPRIVATE_H
