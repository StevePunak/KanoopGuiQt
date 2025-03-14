#ifndef ICONLABEL_H
#define ICONLABEL_H

#include <QWidget>
#include <Kanoop/gui/libkanoopgui.h>

class QLabel;
class LIBKANOOPGUI_EXPORT IconLabel : public QWidget
{
    Q_OBJECT
public:
    explicit IconLabel(QWidget *parent = nullptr);
    explicit IconLabel(const QString& text, QWidget *parent = nullptr);
    explicit IconLabel(const QString& text, const QIcon& icon, QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString& text);

    QIcon icon() const;
    void setIcon(const QIcon& icon);

    void setIconAlignment(Qt::Alignment alignment);

private:
    void createLayout(const QString& text, const QIcon& icon);

    QLabel* _label = nullptr;
    QLabel* _iconLabel = nullptr;

signals:

private slots:

};

#endif // ICONLABEL_H
