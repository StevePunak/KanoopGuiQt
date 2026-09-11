find_package(Qt6 REQUIRED COMPONENTS Test Gui Widgets)

function(add_kanoop_gui_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE
        Qt6::Test
        Qt6::Gui
        Qt6::Widgets
        KanoopGuiQt
        KanoopCommonQt
    )
    target_include_directories(${name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_kanoop_gui_test(tst_vector3d)
add_kanoop_gui_test(tst_quaternion)
add_kanoop_gui_test(tst_tableheader)
add_kanoop_gui_test(tst_headerstate)
add_kanoop_gui_test(tst_headerreorder)
add_kanoop_gui_test(tst_palette)
add_kanoop_gui_test(tst_stylesheets)
add_kanoop_gui_test(tst_resources)
add_kanoop_gui_test(tst_abstractmodelitem)
add_kanoop_gui_test(tst_geometry_persistence)
add_kanoop_gui_test(tst_standardmenus)

# SC-6236 Tier 7
add_kanoop_gui_test(tst_abstractitemmodel_rootitems)
add_kanoop_gui_test(tst_abstractitemmodel_columncolor)
add_kanoop_gui_test(tst_viewbase_proxycast)
add_kanoop_gui_test(tst_toastmanager)
add_kanoop_gui_test(tst_iconlabel)
add_kanoop_gui_test(tst_layoutitemownership)
add_kanoop_gui_test(tst_accordionwidget)
add_kanoop_gui_test(tst_widgetcolors)
add_kanoop_gui_test(tst_readonlywidgets)
add_kanoop_gui_test(tst_htmlutil)
