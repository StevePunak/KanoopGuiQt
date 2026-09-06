#include <QMenu>
#include <QTest>
#include <Kanoop/gui/standardmenus.h>

/**
 * @brief Resolving a window's conventional menus by title, and finding an insertion point in one.
 *
 * An MDI shell hands its menus to whichever sub-window is activating, so a sub-window inserts its
 * commands into the shell's menus rather than carrying a menu bar of its own. Both halves are
 * matched on text: the menu by its title, the insertion point by a separator index or by the text
 * of the action in front of it.
 *
 * ⚠ Matching folds accelerators and case, so a shell declaring "&File" must resolve as File. A
 * shell that renames a menu, or adds an accelerator to one, silently contributes nothing -- the
 * lookup returns nullptr and every caller treats that as "this window has no File menu".
 */
class TstStandardMenus : public QObject
{
    Q_OBJECT

private:
    QMenu* _file = nullptr;
    QMenu* _trailing = nullptr;

private slots:
    void init();
    void cleanup();

    void aTitleResolvesWithItsAcceleratorAndCaseFolded();
    void aTitleTheSetDoesNotKnowIsIgnored();
    void aMenuTheWindowDoesNotHaveResolvesToNothing();

    void theActionAfterASeparatorIsFound();
    void theSeparatorIndexSelectsWhichSeparator();
    void aSeparatorIndexPastTheLastOneFindsNothing();
    void aSeparatorWithNothingBehindItFindsNothing();

    void theStaticWalkersRefuseANullMenu();

    void theActionAfterANamedOneIsFound();
    void theNamedActionIsMatchedWithItsAcceleratorAndCaseFolded();
    void anActionWithNothingBehindItFindsNothing();
};

void TstStandardMenus::init()
{
    // New, &Open, ---, Save, ---, E&xit
    _file = new QMenu("&File");
    _file->addAction("New");
    _file->addAction("&Open");
    _file->addSeparator();
    _file->addAction("Save");
    _file->addSeparator();
    _file->addAction("E&xit");

    // ⚠ A menu whose LAST entry is a separator. The walkers return the action behind the one they
    // matched, so this is the case where there is nothing behind it -- and the off-by-one that
    // would read past the end lives exactly here.
    _trailing = new QMenu("Edit");
    _trailing->addAction("Cut");
    _trailing->addSeparator();
}

void TstStandardMenus::cleanup()
{
    delete _file;
    _file = nullptr;
    delete _trailing;
    _trailing = nullptr;
}

void TstStandardMenus::aTitleResolvesWithItsAcceleratorAndCaseFolded()
{
    QMenu help("HELP");
    QMenu window("&Window");
    const StandardMenus menus({ _file, _trailing, &help, &window });

    QCOMPARE(menus.menu(StandardMenus::File), _file);
    QCOMPARE(menus.menu(StandardMenus::Edit), _trailing);
    QCOMPARE(menus.menu(StandardMenus::Help), &help);
    QCOMPARE(menus.menu(StandardMenus::Window), &window);
}

void TstStandardMenus::aTitleTheSetDoesNotKnowIsIgnored()
{
    QMenu tools("&Tools");
    const StandardMenus menus({ &tools });

    QCOMPARE(menus.menu(StandardMenus::File), nullptr);
    QCOMPARE(menus.menu(StandardMenus::Edit), nullptr);
    QCOMPARE(menus.menu(StandardMenus::Help), nullptr);
    QCOMPARE(menus.menu(StandardMenus::Window), nullptr);
}

void TstStandardMenus::aMenuTheWindowDoesNotHaveResolvesToNothing()
{
    // ⚠ The instance walkers go through menu(), so a missing menu must answer nullptr rather
    // than walk one. A caller inserting at nullptr appends, which is the right fallback.
    const StandardMenus menus({ _file });

    QCOMPARE(menus.menu(StandardMenus::Help), nullptr);
    QCOMPARE(menus.firstAfterSeparator(StandardMenus::Help), nullptr);
    QCOMPARE(menus.firstAfterText(StandardMenus::Help, "anything"), nullptr);
}

void TstStandardMenus::theActionAfterASeparatorIsFound()
{
    const StandardMenus menus({ _file });

    QAction* found = menus.firstAfterSeparator(StandardMenus::File);
    QVERIFY(found != nullptr);
    QCOMPARE(found->text(), QString("Save"));

    // The static walker answers the same question about an arbitrary menu.
    QCOMPARE(StandardMenus::firstAfterSeparator(_file), found);
}

void TstStandardMenus::theSeparatorIndexSelectsWhichSeparator()
{
    // Zero-based, and it counts separators rather than actions.
    const StandardMenus menus({ _file });

    QCOMPARE(menus.firstAfterSeparator(StandardMenus::File, 0)->text(), QString("Save"));
    QCOMPARE(menus.firstAfterSeparator(StandardMenus::File, 1)->text(), QString("E&xit"));
    QCOMPARE(StandardMenus::firstAfterSeparator(_file, 1)->text(), QString("E&xit"));
}

void TstStandardMenus::aSeparatorIndexPastTheLastOneFindsNothing()
{
    const StandardMenus menus({ _file });

    QCOMPARE(menus.firstAfterSeparator(StandardMenus::File, 2), nullptr);
    QCOMPARE(StandardMenus::firstAfterSeparator(_file, 99), nullptr);
}

void TstStandardMenus::aSeparatorWithNothingBehindItFindsNothing()
{
    // ⚠ The boundary. The separator is the last entry, so there is no action behind it and the
    // walker must say so rather than index past the end.
    QCOMPARE(StandardMenus::firstAfterSeparator(_trailing), nullptr);

    const StandardMenus menus({ _trailing });
    QCOMPARE(menus.firstAfterSeparator(StandardMenus::Edit), nullptr);
}

void TstStandardMenus::theStaticWalkersRefuseANullMenu()
{
    // ⚠ menu() answers nullptr for a menu the window does not have, and chaining it straight
    // into a static walker is the obvious way to call one -- so a null menu arrives here as a
    // matter of course rather than as misuse. The member overloads already answer nullptr for
    // it; these must agree rather than dereference it.
    const StandardMenus menus({ _file });
    QCOMPARE(menus.menu(StandardMenus::Help), nullptr);

    QCOMPARE(StandardMenus::firstAfterSeparator(menus.menu(StandardMenus::Help)), nullptr);
    QCOMPARE(StandardMenus::firstAfterText(menus.menu(StandardMenus::Help), "anything"), nullptr);
}

void TstStandardMenus::theActionAfterANamedOneIsFound()
{
    const StandardMenus menus({ _file });

    QCOMPARE(menus.firstAfterText(StandardMenus::File, "New")->text(), QString("&Open"));

    // ⚠ A separator is a legitimate answer: it is what sits behind "&Open", and inserting in
    // front of a separator is the usual reason to ask.
    QAction* behindOpen = menus.firstAfterText(StandardMenus::File, "Open");
    QVERIFY(behindOpen != nullptr);
    QVERIFY(behindOpen->isSeparator());
}

void TstStandardMenus::theNamedActionIsMatchedWithItsAcceleratorAndCaseFolded()
{
    // "&Open" is found by "open", which is what a caller naming a menu item will actually pass.
    const StandardMenus menus({ _file });

    QCOMPARE(menus.firstAfterText(StandardMenus::File, "OPEN"), menus.firstAfterText(StandardMenus::File, "&Open"));
    QCOMPARE(StandardMenus::firstAfterText(_file, "new")->text(), QString("&Open"));
    QCOMPARE(menus.firstAfterText(StandardMenus::File, "nothing named this"), nullptr);
}

void TstStandardMenus::anActionWithNothingBehindItFindsNothing()
{
    const StandardMenus menus({ _file });

    // E&xit is the last entry.
    QCOMPARE(menus.firstAfterText(StandardMenus::File, "exit"), nullptr);
}

QTEST_MAIN(TstStandardMenus)
#include "tst_standardmenus.moc"
