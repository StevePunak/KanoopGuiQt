# KanoopGuiQt {#mainpage}

Qt GUI utility library providing base classes for MDI windows, tree/list/table views, model items, dialogs, settings persistence, and custom widgets.

## Key Classes

| Class | Description |
|-------|-------------|
| `MainWindowBase` | QMainWindow base with position/size persistence and font management |
| `MdiWindow` | MDI container with sub-window lifecycle management |
| `MdiSubWindow` | QMdiSubWindow wrapper with type tagging and geometry persistence |
| `TreeViewBase` | QTreeView with header state persistence, column settings, and search |
| `ListView` | QListView with proxy model support and UUID-based selection |
| `AbstractItemModel` | QAbstractItemModel base with UUID lookup and column header management |
| `AbstractModelItem` | Tree item base with entity metadata, icons, and parent/child hierarchy |
| `ComplexWidget` | QWidget base with validation signal wiring and settings persistence |
| `GuiSettings` | Singleton for persisting window geometry, header state, and preferences |
| `StatusBar` | Custom status bar with animated progress messages |
| `PushButton` | QPushButton with stylesheet-based color and font customization |

## Links

- [GitHub Repository](https://github.com/StevePunak/KanoopGuiQt)
- [Class List](annotated.html)
- [Class Hierarchy](hierarchy.html)
- [File List](files.html)
