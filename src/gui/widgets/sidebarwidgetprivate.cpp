#include "sidebarwidgetprivate.h"

#include <QPainter>

#include <Kanoop/geometry/rectangle.h>
#include <Kanoop/log.h>

void SidebarPaintDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);
    const QRect &contentRect(rect.adjusted(_contentsMargins.left(),
                                           _contentsMargins.top(),
                                           -_contentsMargins.right(),
                                           -_contentsMargins.bottom()));
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    const bool hasIcon = !opt.icon.isNull();
    const int bottomEdge = rect.bottom();

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                                palette.highlight().color() :
                                palette.light().color());

    // Draw bottom line
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    painter->drawLine(lastIndex ? rect.left() : _contentsMargins.left(),
                      bottomEdge, rect.right(), bottomEdge);

    int centerX = Rectangle(rect).topEdge().midpoint().x();

    // Draw icon
    if (hasIcon) {
        int iconX = centerX - _iconSize.width() / 2;
        QPoint iconPoint(iconX, contentRect.top());
        painter->drawPixmap(iconPoint,
                            opt.icon.pixmap(_iconSize));
    }

    // Draw Text
    QFont f(opt.font);
    f.setPointSize(.85 * f.pointSize());

    QRect textRect(textRectangle(opt));
    int messageX = centerX - textRect.width() / 2;
    textRect.moveTo(messageX, contentRect.top() + _iconSize.height() + 2);

    painter->setFont(f);
    painter->setPen(palette.windowText().color());
    painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, opt.text);

    painter->restore();
}

QSize SidebarPaintDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = _verticalSpacing + textRectangle(opt).height();
    int iconHeight = _iconSize.height();
    int h = textHeight + iconHeight;

    QSize result = QSize(opt.rect.width(), _contentsMargins.top() + h
                                               + _contentsMargins.bottom());
    return result;
}

QFont SidebarPaintDelegate::textFont(const QStyleOptionViewItem &option) const
{
    QFont font = option.font;
    font.setPointSize(.85 * font.pointSizeF());
    return font;
}

QRect SidebarPaintDelegate::textRectangle(const QStyleOptionViewItem &option) const
{
    QFontMetrics metrics(textFont(option));
    return metrics.boundingRect(option.rect, Qt::AlignCenter | Qt::TextWordWrap, option.text);
}
