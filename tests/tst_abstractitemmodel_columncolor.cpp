#include <QTest>
#include <QColor>
#include <QUuid>

#include <Kanoop/entitymetadata.h>
#include <Kanoop/gui/abstractitemmodel.h>
#include <Kanoop/gui/abstractmodelitem.h>

namespace Colors = QColorConstants::Svg;

namespace
{

enum ColumnType
{
    ColFirst = 10,
    ColSecond = 11,
    ColThird = 12,
    ColUnknown = 99
};

class Item : public AbstractModelItem
{
public:
    Item(const QString& name, AbstractItemModel* model) :
        AbstractModelItem(EntityMetadata(1, name, QUuid::createUuid()), model), _name(name) {}

    QVariant data(const QModelIndex& index, int role) const override
    {
        if(role == Qt::DisplayRole) {
            return _name;
        }
        return AbstractModelItem::data(index, role);
    }

private:
    QString _name;
};

class Model : public AbstractItemModel
{
public:
    Model() : AbstractItemModel("tst-columncolor") {}

    using AbstractItemModel::appendColumnHeader;
    using AbstractItemModel::appendRootItem;
    using AbstractItemModel::setColumnTextColor;

    /**
     * @brief Three columns and a three-deep tree: 2 roots, 2 children each, 1 grandchild each.
     */
    void buildThreeDeepTree()
    {
        appendColumnHeader(ColFirst, QString("First"));
        appendColumnHeader(ColSecond, QString("Second"));
        appendColumnHeader(ColThird, QString("Third"));

        for(int r = 0;r < 2;r++) {
            Item* root = new Item(QString("root%1").arg(r), this);
            for(int c = 0;c < 2;c++) {
                Item* child = new Item(QString("child%1-%2").arg(r).arg(c), this);
                child->appendChild(new Item(QString("grandchild%1-%2").arg(r).arg(c), this));
                root->appendChild(child);
            }
            appendRootItem(root);
        }
    }
};

/**
 * @brief Every index in the model at the given column, root level downwards.
 */
QModelIndexList allIndexesInColumn(const QAbstractItemModel& model, int column, const QModelIndex& parent = QModelIndex())
{
    QModelIndexList result;
    const int rows = model.rowCount(parent);
    for(int row = 0;row < rows;row++) {
        result.append(model.index(row, column, parent));
        result.append(allIndexesInColumn(model, column, model.index(row, 0, parent)));
    }
    return result;
}

struct ColumnChange
{
    int column = -1;
    bool carriedForegroundRole = false;
};

}

/**
 * @brief Pins AbstractItemModel::setColumnTextColor (a9fd7f5).
 *
 * ⚠ The mutator was declared with no definition anywhere, so before the fix this file does not
 * link at all - that is its red-first evidence, not a failed assertion.
 *
 * ⚠ setColumnTextColor_reachesEveryTreeDepth still passes when emitColumnChanged does not recurse,
 * because data() reads the header live at any depth.  Only
 * setColumnTextColor_announcesTheColumnAtEveryDepth pins the recursion, and the repaint of cells
 * already on screen depends on it.
 */
class TstAbstractItemModelColumnColor : public QObject
{
    Q_OBJECT

private slots:
    void setColumnTextColor_reachesEveryTreeDepth()
    {
        Model model;
        model.buildThreeDeepTree();

        const QModelIndexList targets = allIndexesInColumn(model, 1);
        QCOMPARE(targets.count(), 10);          // 2 roots, 4 children, 4 grandchildren

        // Control: nothing is coloured yet, so the assertions after the call cannot be satisfied by
        // a foreground that was always there.
        for(const QModelIndex& index : targets) {
            QVERIFY(index.isValid());
            QVERIFY(index.data(Qt::ForegroundRole).isValid() == false);
        }

        model.setColumnTextColor(ColSecond, Colors::red);

        for(const QModelIndex& index : targets) {
            const QVariant foreground = index.data(Qt::ForegroundRole);
            QVERIFY2(foreground.isValid(),
                     qPrintable(QString("no foreground at row %1 depth-parent valid %2")
                                    .arg(index.row()).arg(index.parent().isValid())));
            QCOMPARE(foreground.value<QColor>(), QColor(Colors::red));
        }
    }

    void setColumnTextColor_leavesOtherColumnsAlone()
    {
        Model model;
        model.buildThreeDeepTree();
        model.setColumnTextColor(ColSecond, Colors::red);

        // Positive half: the column that was set did move.
        QCOMPARE(model.index(0, 1, QModelIndex()).data(Qt::ForegroundRole).value<QColor>(), QColor(Colors::red));

        // Negative half: its neighbours did not.
        for(const int column : { 0, 2 }) {
            const QModelIndexList others = allIndexesInColumn(model, column);
            QVERIFY(others.isEmpty() == false);
            for(const QModelIndex& index : others) {
                QVERIFY(index.data(Qt::ForegroundRole).isValid() == false);
            }
        }
    }

    void setColumnTextColor_announcesTheColumnAtEveryDepth()
    {
        Model model;
        model.buildThreeDeepTree();

        QList<ColumnChange> changes;
        connect(&model, &QAbstractItemModel::dataChanged, &model,
                [&changes](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles) {
            ColumnChange change;
            change.column = topLeft.column();
            change.carriedForegroundRole = roles.contains(Qt::ForegroundRole);
            QCOMPARE(bottomRight.column(), topLeft.column());
            changes.append(change);
        });

        model.setColumnTextColor(ColSecond, Colors::dodgerblue);

        // One announcement per parent that has rows: the root level, then each of the 2 roots, then
        // each of the 4 children.  The 4 grandchildren have no rows and announce nothing.
        QCOMPARE(changes.count(), 7);
        for(const ColumnChange& change : changes) {
            QCOMPARE(change.column, 1);
            QVERIFY(change.carriedForegroundRole);
        }
    }

    void setColumnTextColor_unknownTypeIsANoOp()
    {
        Model model;
        model.buildThreeDeepTree();

        int announcements = 0;
        connect(&model, &QAbstractItemModel::dataChanged, &model,
                [&announcements](const QModelIndex&, const QModelIndex&, const QList<int>&) {
            ++announcements;
        });

        model.setColumnTextColor(ColUnknown, Colors::red);

        QCOMPARE(announcements, 0);
        const QModelIndexList everything = allIndexesInColumn(model, 1);
        for(const QModelIndex& index : everything) {
            QVERIFY(index.data(Qt::ForegroundRole).isValid() == false);
        }

        // Control: the same model and the same connection do fire for a type that exists, so the
        // zero above is not a model that cannot announce anything at all.
        model.setColumnTextColor(ColSecond, Colors::red);
        QCOMPARE(announcements, 7);
    }

    void setColumnTextColor_overwritesAColourSeededAtConstruction()
    {
        Model model;
        model.appendColumnHeader(ColFirst, Colors::green, QString("First"));
        model.appendRootItem(new Item("root", &model));

        QCOMPARE(model.index(0, 0, QModelIndex()).data(Qt::ForegroundRole).value<QColor>(), QColor(Colors::green));

        model.setColumnTextColor(ColFirst, Colors::gold);

        QCOMPARE(model.index(0, 0, QModelIndex()).data(Qt::ForegroundRole).value<QColor>(), QColor(Colors::gold));
    }
};

QTEST_MAIN(TstAbstractItemModelColumnColor)
#include "tst_abstractitemmodel_columncolor.moc"
