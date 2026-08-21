#include <Kanoop/gui/standardmenus.h>

StandardMenus::StandardMenus(const QList<QMenu*>& menus)
{
    for(QMenu* menu : menus) {
        QString text = simplified(menu->title());
        if(text == "file") {
            _menus.insert(File, menu);
        }
        else if(text == "edit") {
            _menus.insert(Edit, menu);
        }
        else if(text == "help") {
            _menus.insert(Help, menu);
        }
        else if(text == "window") {
            _menus.insert(Window, menu);
        }
    }
}

QAction* StandardMenus::firstAfterSeparator(Menu menuType, int number) const
{
    QMenu* menu = _menus.value(menuType);
    return menu == nullptr ? nullptr : firstAfterSeparator(menu, number);
}

QAction* StandardMenus::firstAfterText(Menu menuType, const QString& text, int number) const
{
    QMenu* menu = _menus.value(menuType);
    return menu == nullptr ? nullptr : firstAfterText(menu, text, number);
}

QAction* StandardMenus::firstAfterSeparator(const QMenu* menu, int number)
{
    // ⚠ The member overloads answer nullptr for a menu the window does not have, so a caller
    // chaining menu() straight into one of these hands it a null menu as a matter of course.
    if(menu == nullptr) {
        return nullptr;
    }

    int count = -1;
    for(int i = 0;i < menu->actions().count();i++) {
        QAction* action = menu->actions().at(i);
        if(action->isSeparator() && ++count == number) {
            return i < menu->actions().count() - 1 ? menu->actions().at(i + 1) : nullptr;
        }
    }
    return nullptr;
}

QAction* StandardMenus::firstAfterText(const QMenu* menu, const QString& text, int number)
{
    if(menu == nullptr) {
        return nullptr;
    }

    const QString wanted = simplified(text);
    int count = -1;
    for(int i = 0;i < menu->actions().count();i++) {
        QAction* action = menu->actions().at(i);
        if(wanted == simplified(action->text()) && ++count == number) {
            return i < menu->actions().count() - 1 ? menu->actions().at(i + 1) : nullptr;
        }
    }
    return nullptr;
}

QString StandardMenus::simplified(const QString& text)
{
    QString result = text;
    result.remove('&');
    result = result.toLower();
    return result;
}
