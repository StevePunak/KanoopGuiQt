#ifndef PALETTE_H
#define PALETTE_H
#include <QPalette>

class Palette : public QPalette
{
public:
    Palette() :
        QPalette() {}
    Palette(const QColor &button) :
        QPalette(button) {}
    Palette(Qt::GlobalColor button) :
        QPalette(button) {}
    Palette(const QColor &button, const QColor &window) :
        QPalette(button, window) {}
    Palette(const QBrush &windowText, const QBrush &button, const QBrush &light,
             const QBrush &dark, const QBrush &mid, const QBrush &text,
             const QBrush &bright_text, const QBrush &base, const QBrush &window) :
        QPalette(windowText, button, light, dark, mid, text, bright_text, base, window) {}
    Palette(const QColor &windowText, const QColor &window, const QColor &light,
             const QColor &dark, const QColor &mid, const QColor &text, const QColor &base) :
        QPalette(windowText, window, light, dark, mid, text, base) {}
    Palette(const QPalette &palette) :
        QPalette(palette) {}

    static Palette fusionLight();
    static Palette fusionDark();

private:
};

#endif // PALETTE_H
