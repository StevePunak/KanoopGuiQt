# CLAUDE.md — KanoopGuiQt

Guidance for Claude Code sessions working with this library. Read this before touching code in this submodule.

## Overview

KanoopGuiQt provides Qt Widgets base classes for the GUI layer — model/view scaffolds (table/tree/list), dialog/window/widget bases with form validation and persistent geometry, the `EntityMetadata` ↔ `KANOOP::DataRole` model wiring, and common widget utilities. It does **not** contain application UI, business logic, or any non-GUI utilities (those are in [KanoopCommonQt](../KanoopCommonQt/CLAUDE.md)).

- Trunk branch: **`master`** (not `main`).
- Public include path: `<Kanoop/gui/...>` — for example `#include <Kanoop/gui/abstracttablemodel.h>`, `#include <Kanoop/gui/tableviewbase.h>`.
- Export macro: **`LIBKANOOPGUI_EXPORT`** (defined in `<Kanoop/gui/libkanoopgui.h>`). Every public class must be marked with it — Windows MinGW link fails without it.
- Depends on KanoopCommonQt (for `EntityMetadata`, `Log`, `TimeSpan`, etc.).
- Thread affinity: **GUI thread only.** Marshal updates from worker threads via signals or `QMetaObject::invokeMethod`.

## Library conventions

- **Model/view pair convention.** Always create a `<Foo>TableModel` (or `<Foo>TreeModel`) alongside a `<Foo>TableView` (or `<Foo>TreeView`). The view owns the model and exposes a typed API; widget `.ui` files declare the view as a custom widget.
- **EntityMetadata everywhere.** Every `AbstractModelItem` carries an `EntityMetadata` (type + UUID + role-value map). Build items in the model constructor, route data through the column-header enum, query items by UUID — never by raw `QModelIndex`.
- **Proxy-wrapped views.** `TableViewBase`/`TreeViewBase` wrap the source model in a `QSortFilterProxyModel` automatically. Always call `sourceModel()` for UUID lookups; the index returned by `view->currentIndex()` is a proxy index — convert with `sourceModel()->mapToSource()` if you need the source.
- **UUID-based ops, not QModelIndexLists.** Pass UUIDs into model methods (`deleteItem(uuid)`, `firstIndexOfEntityUuid(uuid)`); extract UUIDs from `EntityMetadata` in the view before calling into the model.
- **Form validation.** Subclasses of `Dialog`/`ComplexWidget`/`MainWindowBase` override `validate()` and call `initializeBase()` after the UI is built. The base auto-wires `textChanged`/`toggled`/`currentIndexChanged` etc. to invoke `validate()`.
- **Persistent geometry.** Toggle `setPersistPosition(true)` and `setPersistSize(true)` after construction — geometry persists in `QSettings` under the widget's object name.
- **Single-type vs multi-type models.** For models holding one entity type, use plain `KANOOP::DataRole` + `value<T>()` retrieval. For multi-type trees or delegates that render different editors per type, use `EntityMetadata` as the role payload.

## Code style

- **Class names**: PascalCase. **Methods**: camelCase. **Members**: underscore-prefixed camelCase (`_foo`). **Statics**: PascalCase, no prefix.
- Opening brace on same line for `if`/`for`/`while`/`try`; on new line for function/method definitions.
- No space before parens in control flow: `if(cond) {`, not `if (cond) {`.
- Explicit boolean comparison: `if(isReady() == false)` and `if(ok == true)`, not `if(!isReady())`.
- Includes ordered: same-library (`"..."`), then dependency libs (`<Kanoop/...>`), then Qt/system.
- `.ui` widget naming uses `<type><Description>` prefixes: `tree`, `table`, `list`, `text` (QLineEdit), `push`, `group`, `label`, `combo`.
- File-local helper **functions** used by one class → `static` private members. File-local **constants** → anonymous namespace is fine.
- Doxygen: single-line `/** @brief ... */`; multi-line with `@brief` on the line after `/**`. **Never** document member variables. `Q_PROPERTY` needs `/** @brief ... */` directly above.
- Single-exit functions preferred; avoid early returns.

## Commonly used classes

### `AbstractItemModel` — `<Kanoop/gui/abstractitemmodel.h>`
Base for all KanoopGuiQt models. Provides EntityMetadata routing, column-header registration, and UUID search.
```cpp
class MyModel : public AbstractItemModel {
    enum ColumnHeader { CH_Name, CH_Status };
public:
    MyModel(QObject* parent) : AbstractItemModel(parent) {
        appendColumnHeader(CH_Name, "Name");
        appendColumnHeader(CH_Status, "Status");
    }
};
QModelIndex idx = model->firstIndexOfEntityUuid(uuid);
```
Pitfalls: `index()`/`parent()` are final — only override `data()` or the update slots. Column-header types are application-defined integers (an enum in the model class). After mutating items, call `refresh(topLeft, bottomRight)` or emit `dataChanged`.

### `AbstractTableModel` — `<Kanoop/gui/abstracttablemodel.h>`
Flat row/column table specialization.
```cpp
class ProfileTableModel : public AbstractTableModel {
    enum CH { CH_Name, CH_Host };
public:
    ProfileTableModel(QObject* parent) : AbstractTableModel(parent) {
        appendColumnHeader(CH_Name, "Name");
        appendColumnHeader(CH_Host, "Host");
        for(const Profile& p : profiles) {
            appendRootItem(new ProfileItem(p, this));
        }
    }
};
```
Pitfalls: `columnCount()` is derived from registered headers — don't override. Map `index.column()` back to your `CH_` enum in `data()`.

### `AbstractTreeModel` — `<Kanoop/gui/abstracttreemodel.h>`
Hierarchical tree specialization. Items form a parent-child hierarchy via `appendChild`; root items via `appendRootItem`.
```cpp
class NodeTreeModel : public AbstractTreeModel {
    void populate(const Node& node, AbstractModelItem* parent) {
        auto item = new NodeItem(node, this);
        if(parent == nullptr) appendRootItem(item);
        else                  parent->appendChild(item);
        for(const Node& child : node.children()) populate(child, item);
    }
};
```

### `AbstractModelItem` — `<Kanoop/gui/abstractmodelitem.h>`
Base class for items inside a model. Holds `EntityMetadata`, child list, optional icon.
```cpp
class ProfileItem : public AbstractModelItem {
    Profile _profile;
public:
    ProfileItem(const Profile& p, AbstractItemModel* model)
        : AbstractModelItem(EntityMetadata(EntityTypes::Profile, p.toVariant(), p.uuid()), model)
        , _profile(p) {}
    QVariant data(const QModelIndex& idx, int role) const override {
        if(role == Qt::DisplayRole && idx.column() == ProfileTableModel::CH_Name)
            return _profile.name();
        return {};
    }
};
```
Pitfalls: items are owned by the model — never delete manually. Metadata must be set in the constructor.

### `TableViewBase` — `<Kanoop/gui/tableviewbase.h>`
`QTableView` subclass with proxy wrapping, UUID lookups, and metadata-at-position helpers.
```cpp
view->setModel(model);                                       // wraps in proxy
EntityMetadata md = view->metadataAtPos(event->pos());       // for context menus
QModelIndex idx = view->sourceModel()->firstIndexOfEntityUuid(uuid);
```

### `TreeViewBase` — `<Kanoop/gui/treeviewbase.h>`
`QTreeView` equivalent. Also provides `setCurrentUuid(uuid)`, recursive `collapseAll`, and case-insensitive text search.

### `Dialog` — `<Kanoop/gui/dialog.h>`
`QDialog` base with auto-wired validation, persistent geometry, status bar.
```cpp
class MyDialog : public Dialog {
public:
    MyDialog(QWidget* parent) : Dialog("MyDialog", parent) {
        // build UI
        initializeBase();                  // auto-wires input → validate()
        setPersistPosition(true);
        setPersistSize(true);
    }
protected:
    void validate() override {
        okButton()->setEnabled(_hostEdit->text().isEmpty() == false);
    }
};
```
Pitfalls: forgetting `initializeBase()` silently kills validation auto-wiring. The string passed to the base ctor is the QSettings key for geometry — keep it stable across versions.

### `ComplexWidget` — `<Kanoop/gui/complexwidget.h>`
Same auto-wired validation pattern as `Dialog`, but for embedded `QWidget`s.

### `MainWindowBase` — `<Kanoop/gui/mainwindowbase.h>`
`QMainWindow` base with logging, status bar, and persistent geometry. Same `initializeBase()` requirement.

### `EntityMetadata` — `<Kanoop/entitymetadata.h>` (from KanoopCommonQt)
The bridge object. See [KanoopCommonQt CLAUDE.md](../KanoopCommonQt/CLAUDE.md#entitymetadata--kanoopentitymetadatah).

## Common gotchas

- **Never pass raw `QModelIndexList` into models.** UUIDs are stable across sorts/filters/inserts; proxy indexes are not. Extract UUIDs in the view, then call the UUID-based model API.
- **`sourceModel()` for lookups, `model()` returns the proxy.** Forgetting this leads to "why does my UUID lookup return invalid index" bugs.
- **`refresh()` not allowed inside `data()`.** It triggers `dataChanged` which calls `data()` again — infinite recursion. Refresh after the model mutation that caused the change.
- **Don't connect item signals directly to the model.** Notifications flow through `AbstractItemModel`'s protected slots (`addEntity`, `updateEntity`, `deleteEntity`) — override those.
- **`initializeBase()` is not optional.** Dialog/ComplexWidget/MainWindowBase rely on it to wire validation and load persisted geometry.
- **Column header types are *types*, not indexes.** `appendColumnHeader(CH_Name, "Name")` registers the type → label mapping. `data()` receives `index.column()` (positional) — map it back to the enum via `columnHeader(index.column()).type()`.
- **Lambda-captured `this` in slots is a footgun.** Prefer named signal/slot connections — they auto-disconnect on receiver destruction.
- **Declspec on every new public class.** Missing `LIBKANOOPGUI_EXPORT` compiles on Linux and fails CI on Windows MinGW.

## Working in this repo

- When this library is consumed as a git submodule, the submodule will typically be in **detached HEAD** state pointing at a pinned commit. Check out a working branch before committing.
- Conventional-commits message format is used: `<type>(<scope>): <description>` with types `fix`, `feat`, `doc`, `refactor`, `test`, `chore`. Add a `Co-Authored-By` trailer if pair-programming.
- Submodule pointer changes belong in the consuming repo, not here.
