#ifndef STYLESHEETTYPES_H
#define STYLESHEETTYPES_H

#include <Kanoop/kanoopcommon.h>

/**
 * @brief Qt stylesheet property identifiers.
 *
 * Each value corresponds to a CSS property supported by Qt's stylesheet engine.
 * Pass these to StyleSheet::setProperty() or StyleSheetStrings::getPropertyString().
 */
enum StyleSheetProperty {
    SP_AccentColor,
    SP_AlternateBackgroundColor,
    SP_Background,
    SP_BackgroundColor,
    SP_BackgroundImage,
    SP_BackgroundRepeat,
    SP_BackgroundPosition,
    SP_BackgroundAttachment,
    SP_BackgroundClip,
    SP_BackgroundOrigin,
    SP_Border,
    SP_BorderTop,
    SP_BorderRight,
    SP_BorderBottom,
    SP_BorderLeft,
    SP_BorderColor,
    SP_BorderTopColor,
    SP_BorderRightColor,
    SP_BorderBottomColor,
    SP_BorderLeftColor,
    SP_BorderImage,
    SP_BorderRadius,
    SP_BorderTopLeftRadius,
    SP_BorderTopRightRadius,
    SP_BorderBottomRightRadius,
    SP_BorderBottomLeftRadius,
    SP_BorderStyle,
    SP_BorderTopStyle,
    SP_BorderRightStyle,
    SP_BorderBottomStyle,
    SP_BorderLeftStyle,
    SP_BorderWidth,
    SP_BorderTopWidth,
    SP_BorderRightWidth,
    SP_BorderBottomWidth,
    SP_BorderLeftWidth,
    SP_Bottom,
    SP_ButtonLayout,
    SP_Color,
    SP_DialogbuttonboxButtonsHaveIcons,
    SP_Font,
    SP_FontFamily,
    SP_FontSize,
    SP_FontStyle,
    SP_FontWeight,
    SP_GridlineColor,
    SP_Height,
    SP_Icon,
    SP_IconSize,
    SP_Image,
    SP_ImagePosition,
    SP_Left,
    SP_LineeditPasswordCharacter,
    SP_LineeditPasswordMaskDelay,
    SP_Margin,
    SP_MarginTop,
    SP_MarginRight,
    SP_MarginBottom,
    SP_MarginLeft,
    SP_MaxHeight,
    SP_MaxWidth,
    SP_MessageboxTextInteractionFlags,
    SP_MinHeight,
    SP_MinWidth,
    SP_Opacity,
    SP_Outline,
    SP_OutlineColor,
    SP_OutlineOffset,
    SP_OutlineStyle,
    SP_OutlineRadius,
    SP_OutlineBottomLeftRadius,
    SP_OutlineBottomRightRadius,
    SP_OutlineTopLeftRadius,
    SP_OutlineTopRightRadius,
    SP_Padding,
    SP_PaddingTop,
    SP_PaddingRight,
    SP_PaddingBottom,
    SP_PaddingLeft,
    SP_PaintAlternatingRowColorsForEmptyArea,
    SP_PlaceholderTextColor,
    SP_Position,
    SP_Right,
    SP_SelectionBackgroundColor,
    SP_SelectionColor,
    SP_ShowDecorationSelected,
    SP_Spacing,
    SP_SubcontrolOrigin,
    SP_SubcontrolPosition,
    SP_TitlebarShowTooltipsOnButtons,
    SP_WidgetAnimationDuration,
    SP_TextAlign,
    SP_TextDecoration,
    SP_Top,
    SP_Width,
    SP_QtBackgroundRole,
    SP_QtStyleFeatures,
};

/**
 * @brief Qt stylesheet pseudo-state identifiers.
 *
 * Each value corresponds to a CSS pseudo-state supported by Qt's stylesheet engine.
 * Pass these to StyleSheet::setPseudoState() or StyleSheetStrings::getPseudoStateString().
 */
enum StyleSheetPseudoState {
    PS_Invalid = 0,

    PS_Active,                  // This state is set when the widget resides in an active window.
    PS_AdjoinsItem,             // This state is set when the ::branch of a QTreeView is adjacent to an item.
    PS_Alternate,               // This state is set for every alternate row whe painting the row of a QAbstractItemView when QAbstractItemView::alternatingRowColors() is set to true.
    PS_Bottom,                  // The item is positioned at the bottom. For example, a QTabBar that has its tabs positioned at the bottom.
    PS_Checked,                 // The item is checked. For example, the checked state of QAbstractButton.
    PS_Closable,                // The items can be closed. For example, the QDockWidget has the QDockWidget::DockWidgetClosable feature turned on.
    PS_Closed,                  // The item is in the closed state. For example, an non-expanded item in a QTreeView
    PS_Default,                 // The item is the default. For example, a default QPushButton or a default action in a QMenu.
    PS_Disabled,                // The item is disabled.
    PS_Editable,                // The QComboBox is editable.
    PS_EditFocus,               // The item has edit focus (See QStyle::State_HasEditFocus). This state is available only for Qt Extended applications.
    PS_Enabled,                 // The item is enabled.
    PS_Exclusive,               // The item is part of an exclusive item group. For example, a menu item in a exclusive QActionGroup.
    PS_First,                   // The item is the first (in a list). For example, the first tab in a QTabBar.
    PS_Flat,                    // The item is flat. For example, a flat QPushButton.
    PS_Floatable,               // The items can be floated. For example, the QDockWidget has the QDockWidget::DockWidgetFloatable feature turned on.
    PS_Focus,                   // The item has input focus.
    PS_HasChildren,             // The item has children. For example, an item in a QTreeView that has child items.
    PS_HasSiblings,             // The item has siblings. For example, an item in a QTreeView that siblings.
    PS_Horizontal,              // The item has horizontal orientation
    PS_Hover,                   // The mouse is hovering over the item.
    PS_Indeterminate,           // The item has indeterminate state. For example, a QCheckBox or QRadioButton is partially checked.
    PS_Last,                    // The item is the last (in a list). For example, the last tab in a QTabBar.
    PS_Left,                    // The item is positioned at the left. For example, a QTabBar that has its tabs positioned at the left.
    PS_Maximized,               // The item is maximized. For example, a maximized QMdiSubWindow.
    PS_Middle,                  // The item is in the middle (in a list). For example, a tab that is not in the beginning or the end in a QTabBar.
    PS_Minimized,               // The item is minimized. For example, a minimized QMdiSubWindow.
    PS_Movable,                 // The item can be moved around. For example, the QDockWidget has the QDockWidget::DockWidgetMovable feature turned on.
    PS_NoFrame,                 // The item has no frame. For example, a frameless QSpinBox or QLineEdit.
    PS_NonExclusive,            // The item is part of a non-exclusive item group. For example, a menu item in a non-exclusive QActionGroup.
    PS_Off,                     // For items that can be toggled, this applies to items in the "off" state.
    PS_On,                      // For items that can be toggled, this applies to widgets in the "on" state.
    PS_OnlyOne,                 // The item is the only one (in a list). For example, a lone tab in a QTabBar.
    PS_Open,                    // The item is in the open state. For example, an expanded item in a QTreeView, or a QComboBox or QPushButton with an open menu.
    PS_NextSelected,            // The next item (in a list) is selected. For example, the selected tab of a QTabBar is next to this item.
    PS_Pressed,                 // The item is being pressed using the mouse.
    PS_PreviousSelected,        // The previous item (in a list) is selected. For example, a tab in a QTabBar that is next to the selected tab.
    PS_ReadOnly,                // The item is marked read only or non-editable. For example, a read only QLineEdit or a non-editable QComboBox.
    PS_Right,                   // The item is positioned at the right. For example, a QTabBar that has its tabs positioned at the right.
    PS_Selected,                // The item is selected. For example, the selected tab in a QTabBar or the selected item in a QMenu.
    PS_Top,                     // The item is positioned at the top. For example, a QTabBar that has its tabs positioned at the top.
    PS_Unchecked,               // The item is unchecked.
    PS_Vertical,                // The item has vertical orientation.
    PS_Window,                  // The widget is a window (i.e top level widget)
};

/**
 * @brief Utility class for converting stylesheet enums to their CSS string equivalents.
 */
class StyleSheetStrings
{
public:
    /**
     * @brief Return the CSS property name for a StyleSheetProperty value.
     * @param property Property enum value
     * @return CSS property name string (e.g., "background-color")
     */
    static QString getPropertyString(StyleSheetProperty property) { return _PropertyToStringMap.getString(property); }

    /**
     * @brief Return the CSS pseudo-state name for a StyleSheetPseudoState value.
     * @param pseudoState Pseudo-state enum value
     * @return CSS pseudo-state string (e.g., "hover")
     */
    static QString getPseudoStateString(StyleSheetPseudoState pseudoState) { return _PseudoStateToStringMap.getString(pseudoState); }

private:
    class PropertyToStringMap : public KANOOP::EnumToStringMap<StyleSheetProperty>
    {
    public:
        PropertyToStringMap()
        {
            insert(SP_AccentColor,                             "accent-color");
            insert(SP_AlternateBackgroundColor,                "alternate-background-color");
            insert(SP_Background,                              "background");
            insert(SP_BackgroundColor,                         "background-color");
            insert(SP_BackgroundImage,                         "background-image");
            insert(SP_BackgroundRepeat,                        "background-repeat");
            insert(SP_BackgroundPosition,                      "background-position");
            insert(SP_BackgroundAttachment,                    "background-attachment");
            insert(SP_BackgroundClip,                          "background-clip");
            insert(SP_BackgroundOrigin,                        "background-origin");
            insert(SP_Border,                                  "border");
            insert(SP_BorderTop,                               "border-top");
            insert(SP_BorderRight,                             "border-right");
            insert(SP_BorderBottom,                            "border-bottom");
            insert(SP_BorderLeft,                              "border-left");
            insert(SP_BorderColor,                             "border-color");
            insert(SP_BorderTopColor,                          "border-top-color");
            insert(SP_BorderRightColor,                        "border-right-color");
            insert(SP_BorderBottomColor,                       "border-bottom-color");
            insert(SP_BorderLeftColor,                         "border-left-color");
            insert(SP_BorderImage,                             "border-image");
            insert(SP_BorderRadius,                            "border-radius");
            insert(SP_BorderTopLeftRadius,                     "border-top-left-radius");
            insert(SP_BorderTopRightRadius,                    "border-top-right-radius");
            insert(SP_BorderBottomRightRadius,                 "border-bottom-right-radius");
            insert(SP_BorderBottomLeftRadius,                  "border-bottom-left-radius");
            insert(SP_BorderStyle,                             "border-style");
            insert(SP_BorderTopStyle,                          "border-top-style");
            insert(SP_BorderRightStyle,                        "border-right-style");
            insert(SP_BorderBottomStyle,                       "border-bottom-style");
            insert(SP_BorderLeftStyle,                         "border-left-style");
            insert(SP_BorderWidth,                             "border-width");
            insert(SP_BorderTopWidth,                          "border-top-width");
            insert(SP_BorderRightWidth,                        "border-right-width");
            insert(SP_BorderBottomWidth,                       "border-bottom-width");
            insert(SP_BorderLeftWidth,                         "border-left-width");
            insert(SP_Bottom,                                  "bottom");
            insert(SP_ButtonLayout,                            "button-layout");
            insert(SP_Color,                                   "color");
            insert(SP_DialogbuttonboxButtonsHaveIcons,         "dialogbuttonbox-buttons-have-icons");
            insert(SP_Font,                                    "font");
            insert(SP_FontFamily,                              "font-family");
            insert(SP_FontSize,                                "font-size");
            insert(SP_FontStyle,                               "font-style");
            insert(SP_FontWeight,                              "font-weight");
            insert(SP_GridlineColor,                           "gridline-color");
            insert(SP_Height,                                  "height");
            insert(SP_Icon,                                    "icon");
            insert(SP_IconSize,                                "icon-size");
            insert(SP_Image,                                   "image");
            insert(SP_ImagePosition,                           "image-position");
            insert(SP_Left,                                    "left");
            insert(SP_LineeditPasswordCharacter,               "lineedit-password-character");
            insert(SP_LineeditPasswordMaskDelay,               "lineedit-password-mask-delay");
            insert(SP_Margin,                                  "margin");
            insert(SP_MarginTop,                               "margin-top");
            insert(SP_MarginRight,                             "margin-right");
            insert(SP_MarginBottom,                            "margin-bottom");
            insert(SP_MarginLeft,                              "margin-left");
            insert(SP_MaxHeight,                               "max-height");
            insert(SP_MaxWidth,                                "max-width");
            insert(SP_MessageboxTextInteractionFlags,          "messagebox-text-interaction-flags");
            insert(SP_MinHeight,                               "min-height");
            insert(SP_MinWidth,                                "min-width");
            insert(SP_Opacity,                                 "opacity");
            insert(SP_Outline,                                 "outline");
            insert(SP_OutlineColor,                            "outline-color");
            insert(SP_OutlineOffset,                           "outline-offset");
            insert(SP_OutlineStyle,                            "outline-style");
            insert(SP_OutlineRadius,                           "outline-radius");
            insert(SP_OutlineBottomLeftRadius,                 "outline-bottom-left-radius");
            insert(SP_OutlineBottomRightRadius,                "outline-bottom-right-radius");
            insert(SP_OutlineTopLeftRadius,                    "outline-top-left-radius");
            insert(SP_OutlineTopRightRadius,                   "outline-top-right-radius");
            insert(SP_Padding,                                 "padding");
            insert(SP_PaddingTop,                              "padding-top");
            insert(SP_PaddingRight,                            "padding-right");
            insert(SP_PaddingBottom,                           "padding-bottom");
            insert(SP_PaddingLeft,                             "padding-left");
            insert(SP_PaintAlternatingRowColorsForEmptyArea,   "paint-alternating-row-colors-for-empty-area");
            insert(SP_PlaceholderTextColor,                    "placeholder-text-color");
            insert(SP_Position,                                "position");
            insert(SP_Right,                                   "right");
            insert(SP_SelectionBackgroundColor,                "selection-background-color");
            insert(SP_SelectionColor,                          "selection-color");
            insert(SP_ShowDecorationSelected,                  "show-decoration-selected");
            insert(SP_Spacing,                                 "spacing");
            insert(SP_SubcontrolOrigin,                        "subcontrol-origin");
            insert(SP_SubcontrolPosition,                      "subcontrol-position");
            insert(SP_TitlebarShowTooltipsOnButtons,           "titlebar-show-tooltips-on-buttons");
            insert(SP_WidgetAnimationDuration,                 "widget-animation-duration");
            insert(SP_TextAlign,                               "text-align");
            insert(SP_TextDecoration,                          "text-decoration");
            insert(SP_Top,                                     "top");
            insert(SP_Width,                                   "width");
            insert(SP_QtBackgroundRole,                        "-qt-background-role");
            insert(SP_QtStyleFeatures,                         "-qt-style-features");
        }
    };

    class PseudoStateToStringMap : public KANOOP::EnumToStringMap<StyleSheetPseudoState>
    {
    public:
        PseudoStateToStringMap()
        {
            insert(PS_Active,               "active");
            insert(PS_AdjoinsItem,          "adjoins-item");
            insert(PS_Alternate,            "alternate");
            insert(PS_Bottom,               "bottom");
            insert(PS_Checked,              "checked");
            insert(PS_Closable,             "closable");
            insert(PS_Closed,               "closed");
            insert(PS_Default,              "default");
            insert(PS_Disabled,             "disabled");
            insert(PS_Editable,             "editable");
            insert(PS_EditFocus,            "edit-focus");
            insert(PS_Enabled,              "enabled");
            insert(PS_Exclusive,            "exclusive");
            insert(PS_First,                "first");
            insert(PS_Flat,                 "flat");
            insert(PS_Floatable,            "floatable");
            insert(PS_Focus,                "focus");
            insert(PS_HasChildren,          "has-children");
            insert(PS_HasSiblings,          "has-siblings");
            insert(PS_Horizontal,           "horizontal");
            insert(PS_Hover,                "hover");
            insert(PS_Indeterminate,        "indeterminate");
            insert(PS_Last,                 "last");
            insert(PS_Left,                 "left");
            insert(PS_Maximized,            "maximized");
            insert(PS_Middle,               "middle");
            insert(PS_Minimized,            "minimized");
            insert(PS_Movable,              "movable");
            insert(PS_NoFrame,              "no-frame");
            insert(PS_NonExclusive,         "non-exclusive");
            insert(PS_Off,                  "off");
            insert(PS_On,                   "on");
            insert(PS_OnlyOne,              "only-one");
            insert(PS_Open,                 "open");
            insert(PS_NextSelected,         "next-selected");
            insert(PS_Pressed,              "pressed");
            insert(PS_PreviousSelected,     "previous-selected");
            insert(PS_ReadOnly,             "read-only");
            insert(PS_Right,                "right");
            insert(PS_Selected,             "selected");
            insert(PS_Top,                  "top");
            insert(PS_Unchecked,            "unchecked");
            insert(PS_Vertical,             "vertical");
            insert(PS_Window,               "window");
        }
    };

    static const PropertyToStringMap _PropertyToStringMap;
    static const PseudoStateToStringMap _PseudoStateToStringMap;
};

#endif // STYLESHEETTYPES_H
