#include <Kanoop/gui/standardmenus.h>

StandardMenus::StandardMenus(const QList<QMenu*>& menus)
{
    for(QMenu* menu : menus) {
        QString text = simplified(menu->title());
        if(text.toLower() == "file") {
            _menus.insert(File, menu);
        }
        else if(text.toLower() == "edit") {
            _menus.insert(Edit, menu);
        }
        else if(text.toLower() == "help") {
            _menus.insert(Help, menu);
        }
        else if(text.toLower() == "window") {
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
    int count = -1;
    for(int i = 0;i < menu->actions().count();i++) {
        QAction* action = menu->actions().at(i);
        if(simplified(text) == simplified(action->text()) && ++count == number) {
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
