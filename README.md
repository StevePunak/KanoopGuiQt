# KanoopGuiQt

Comprehensive Qt GUI library providing extended model/view classes, custom widgets, graphics items, MDI window management, and utility helpers. Used across EPC and Kanoop Qt projects.

**[API Documentation](https://StevePunak.github.io/KanoopGuiQt/)** | [Class List](https://StevePunak.github.io/KanoopGuiQt/annotated.html) | [Class Hierarchy](https://StevePunak.github.io/KanoopGuiQt/hierarchy.html) | [Files](https://StevePunak.github.io/KanoopGuiQt/files.html)

## Requirements

- C++11
- Qt 6.7.0+ (Gui, Widgets, Network, Designer)
- CMake 3.16+
- [KanoopCommonQt](https://github.com/StevePunak/KanoopCommonQt)

## Building

KanoopGuiQt is typically built as part of the [meta-qt-mains](https://github.com/epcpower/meta-qt-mains) superproject. To build standalone, clone [KanoopCommonQt](https://github.com/StevePunak/KanoopCommonQt) as a sibling and use a workspace CMakeLists.txt:

```bash
# Clone both repos side-by-side
git clone https://github.com/StevePunak/KanoopCommonQt.git
git clone https://github.com/StevePunak/KanoopGuiQt.git

# Create a workspace CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.16)
project(KanoopGuiQt-workspace)
add_subdirectory(KanoopCommonQt)
add_subdirectory(KanoopGuiQt)
EOF

# Build
cmake -S . -B build -G Ninja \
  -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
cmake --build build --parallel
```

## Modules

| Module | Headers | Description |
|--------|---------|-------------|
| **model/view** | 8 | [AbstractItemModel](https://StevePunak.github.io/KanoopGuiQt/classAbstractItemModel.html), [AbstractModelItem](https://StevePunak.github.io/KanoopGuiQt/classAbstractModelItem.html), list/table/tree model specializations, [TableHeader](https://StevePunak.github.io/KanoopGuiQt/classTableHeader.html), [HeaderState](https://StevePunak.github.io/KanoopGuiQt/classHeaderState.html) |
| **views** | 4 | [TableViewBase](https://StevePunak.github.io/KanoopGuiQt/classTableViewBase.html), [TreeViewBase](https://StevePunak.github.io/KanoopGuiQt/classTreeViewBase.html), [ListView](https://StevePunak.github.io/KanoopGuiQt/classListView.html), [TreeSelectionModel](https://StevePunak.github.io/KanoopGuiQt/classTreeSelectionModel.html) |
| **windows** | 6 | [MainWindowBase](https://StevePunak.github.io/KanoopGuiQt/classMainWindowBase.html), [Dialog](https://StevePunak.github.io/KanoopGuiQt/classDialog.html), [MdiWindow](https://StevePunak.github.io/KanoopGuiQt/classMdiWindow.html), [MdiSubWindow](https://StevePunak.github.io/KanoopGuiQt/classMdiSubWindow.html), [MdiArea](https://StevePunak.github.io/KanoopGuiQt/classMdiArea.html), [ComplexWidget](https://StevePunak.github.io/KanoopGuiQt/classComplexWidget.html) |
| **widgets** | 20 | Accordion, button label, checkbox, combobox, date/time edit, frame, group box, icon label, label, line edit, plain text edit, play/pause button, push button, sidebar, slider, spinner, status bar, tab widget, toast manager, Designer plugin collection |
| **graphics** | 7 | [GraphicsView](https://StevePunak.github.io/KanoopGuiQt/classGraphicsView.html), [GraphicsScene](https://StevePunak.github.io/KanoopGuiQt/classGraphicsScene.html), typed line/rectangle/ellipse/pixmap items, [QObjectGraphicsItem](https://StevePunak.github.io/KanoopGuiQt/classQObjectGraphicsItem.html) mixin |
| **utility** | 4 | [HtmlBuilder](https://StevePunak.github.io/KanoopGuiQt/classHtmlBuilder.html), [HtmlUtil](https://StevePunak.github.io/KanoopGuiQt/classHtmlUtil.html), [StyleSheet\<T\>](https://StevePunak.github.io/KanoopGuiQt/classStyleSheet.html) template builder, stylesheet property/pseudo-state enums |
| **core** | 9 | [Application](https://StevePunak.github.io/KanoopGuiQt/classApplication.html), [GuiSettings](https://StevePunak.github.io/KanoopGuiQt/classGuiSettings.html), [Palette](https://StevePunak.github.io/KanoopGuiQt/classPalette.html), [Resources](https://StevePunak.github.io/KanoopGuiQt/classResources.html), [StyleSheets](https://StevePunak.github.io/KanoopGuiQt/classStyleSheets.html), [Vector3D](https://StevePunak.github.io/KanoopGuiQt/classVector3D.html), [Quaternion](https://StevePunak.github.io/KanoopGuiQt/classQuaternion.html), [TabBar](https://StevePunak.github.io/KanoopGuiQt/classTabBar.html), GUI types/enums |

## Testing

Unit tests use Qt6::Test and cover non-GUI logic across 8 test suites:

```bash
# Build and run tests
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

| Test | Description |
|------|-------------|
| `tst_vector3d` | Constructors, toString/fromString round-trip, edge cases |
| `tst_quaternion` | Constructors, toString/fromString round-trip, edge cases |
| `tst_tableheader` | Constructors, properties, List and IntMap container helpers |
| `tst_headerstate` | Section add/get, JSON serialize/deserialize round-trip |
| `tst_palette` | Fusion presets, QVariant round-trip, ColorRole string lookups |
| `tst_stylesheets` | CSS fragment generation (background, foreground, border-radius) |
| `tst_resources` | Image registration, pixmap/icon retrieval |
| `tst_abstractmodelitem` | Tree hierarchy operations, child counting, List search helpers |

## CI

[![CI](https://github.com/StevePunak/KanoopGuiQt/actions/workflows/ci.yaml/badge.svg)](https://github.com/StevePunak/KanoopGuiQt/actions/workflows/ci.yaml)

Builds and runs tests on every push using GitHub Actions (`ubuntu-latest`, Qt 6.10.1, Ninja). Can also be triggered manually from the Actions tab.

## API Documentation

Full Doxygen documentation is generated automatically on every CI build and published to GitHub Pages:

**https://StevePunak.github.io/KanoopGuiQt/**

To generate locally:

```bash
# Via CMake (requires Doxygen + Graphviz)
cmake --build build --target doc_KanoopGuiQt

# Or directly
doxygen Doxyfile
```

Open `docs/html/index.html` in a browser to browse the local copy.

## Project Structure

```
KanoopGuiQt/
  include/Kanoop/gui/            Model/view, windows, core headers
  include/Kanoop/gui/graphics/   Graphics scene/view/item headers
  include/Kanoop/gui/widgets/    Custom widget headers
  include/Kanoop/gui/utility/    HTML builder, stylesheet helpers
  src/gui/                       Implementation files
  tests/                         Unit tests (Qt6::Test)
  docs/                          Generated documentation (not committed)
  Doxyfile                       Doxygen configuration
  CMakeLists.txt                 Build configuration
```

## License

[MIT](LICENSE)
