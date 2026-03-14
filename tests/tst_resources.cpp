#include <QTest>
#include <Kanoop/gui/resources.h>

class TstResources : public QObject
{
    Q_OBJECT

private slots:
    void firstUserResource_constant()
    {
        QCOMPARE(Resources::FirstUserResource, 100000);
    }

    void registerImage_getPixmap_roundTrip()
    {
        // Register a custom image ID (use high number to avoid conflicts)
        int testId = Resources::FirstUserResource + 9999;
        Resources::registerImage(testId, ":/nonexistent/path.png");

        // getPixmap should not crash even with non-existent resource
        QPixmap pm = Resources::getPixmap(testId);
        Q_UNUSED(pm)
    }

    void getIcon_unregisteredId_returnsNull()
    {
        QIcon icon = Resources::getIcon(Resources::FirstUserResource + 99998);
        QVERIFY(icon.isNull());
    }

    void getPixmap_unregisteredId_returnsNull()
    {
        QPixmap pm = Resources::getPixmap(Resources::FirstUserResource + 99997);
        QVERIFY(pm.isNull());
    }

    void standardImages_areRegistered()
    {
        // Standard images should have been auto-registered via static init.
        // We can't verify they load (no actual resource files in test context),
        // but getPixmap should not crash for any standard image ID.
        QPixmap pm;
        pm = Resources::getPixmap(Resources::Play);
        pm = Resources::getPixmap(Resources::Pause);
        pm = Resources::getPixmap(Resources::Stop);
        pm = Resources::getPixmap(Resources::Refresh);
        pm = Resources::getPixmap(Resources::Search);
        Q_UNUSED(pm)
    }

    void registerImage_replaceExisting_nocrash()
    {
        int testId = Resources::FirstUserResource + 9990;
        Resources::registerImage(testId, ":/path/a.png");
        Resources::registerImage(testId, ":/path/b.png");
        // Should not crash; second call replaces first
    }
};

QTEST_MAIN(TstResources)
#include "tst_resources.moc"
