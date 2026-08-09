#ifndef STANDARDMENUS_H
#define STANDARDMENUS_H

#include <Kanoop/gui/libkanoopgui.h>

#include <QMenu>

/**
 * @brief The conventional top-level menus of a main window, resolved by title.
 *
 * An MDI shell owns one set of these and hands them to whichever sub-window is being
 * activated, so a sub-window contributes its commands into the shell's menus rather
 * than carrying a menu bar of its own. Menus are matched on their title text with
 * accelerators and case ignored, so a shell declaring "&File" resolves as File.
 */
class LIBKANOOPGUI_EXPORT StandardMenus
{
public:
    /** @brief The conventional top-level menus. */
    enum Menu
    {
        File,
        Edit,
        Window,
        Help,
    };

    /** @brief Constructs an empty set. */
    StandardMenus() {}

    /**
     * @brief Constructs a set by resolving each menu from its title.
     * @param menus Top-level menus of the owning window; unrecognized titles are ignored
     */
    StandardMenus(const QList<QMenu*>& menus);

    /**
     * @brief Returns the menu of the given type.
     * @param menuType Which conventional menu to return
     * @return The menu, or nullptr when the owning window has no such menu
     */
    QMenu* menu(Menu menuType) const { return _menus.value(menuType); }

    /**
     * @brief Returns the action following a separator, for use as an insertion point.
     * @param menuType Which conventional menu to search
     * @param number Zero-based index of the separator to search from
     * @return The action after that separator, or nullptr
     */
    QAction* firstAfterSeperator(Menu menuType, int number = 0) const;

    /**
     * @brief Returns the action following the one with the given text.
     * @param menuType Which conventional menu to search
     * @param text Text of the action to search from, accelerators and case ignored
     * @param number Zero-based index of the match to search from
     * @return The action after the match, or nullptr
     */
    QAction* firstAfterText(Menu menuType, const QString& text, int number = 0) const;

    /** @brief Returns the action following a separator in an arbitrary menu. */
    static QAction* firstAfterSeperator(const QMenu* menu, int number = 0);

    /** @brief Returns the action following the one with the given text in an arbitrary menu. */
    static QAction* firstAfterText(const QMenu* menu, const QString& text, int number = 0);

private:
    QMap<Menu, QMenu*> _menus;

    static QString simplified(const QString& text);
};

#endif // STANDARDMENUS_H
