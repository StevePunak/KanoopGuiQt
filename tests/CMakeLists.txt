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
add_kanoop_gui_test(tst_palette)
add_kanoop_gui_test(tst_stylesheets)
add_kanoop_gui_test(tst_resources)
add_kanoop_gui_test(tst_abstractmodelitem)
