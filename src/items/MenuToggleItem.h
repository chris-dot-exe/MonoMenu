/**
 * @file MenuToggleItem.h
 *
 * @brief Defines the MenuToggleItem class for representing a toggle item in a menu.
*/
#pragma once

#include "MenuItem.h"
#include "../types.h"

/**
 * @class MenuToggleItem
 * @brief Represents a toggle item in a menu.
*/
class MenuToggleItem : public MenuItem {
public:
    /**
     * @brief Constructs a MenuToggleItem object with the specified label, value, and visibility.
     *
     * @param labelInst The label for the toggle item.
     * @param valueInst Reference to the boolean variable representing the toggle state.
     * @param visibleInst Flag indicating the initial visibility of the toggle item.
    */
    MenuToggleItem(const char *labelInst, bool &valueInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuToggleItem object with the specified label, value, icon, and visibility.
     *
     * @param labelInst The label for the toggle item.
     * @param valueInst Reference to the boolean variable representing the toggle state.
     * @param iconInst The icon for the toggle item.
     * @param visibleInst Flag indicating the initial visibility of the toggle item.
    */
    MenuToggleItem(const char *labelInst, bool &valueInst, const unsigned char *iconInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuToggleItem object with the specified label, value, callback function, and visibility.
     *
     * @param labelInst The label for the toggle item.
     * @param valueInst Reference to the boolean variable representing the toggle state.
     * @param onSaveInst The callback function to be executed when the toggle item state changes.
     * @param visibleInst Flag indicating the initial visibility of the toggle item.
    */
    MenuToggleItem(const char *labelInst, bool &valueInst, std::function<void()> onSaveInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuToggleItem object with the specified label, value, callback function, icon, and visibility.
     *
     * @param labelInst The label for the toggle item.
     * @param valueInst Reference to the boolean variable representing the toggle state.
     * @param onSaveInst The callback function to be executed when the toggle item state changes.
     * @param iconInst The icon for the toggle item.
     * @param visibleInst Flag indicating the initial visibility of the toggle item.
    */
    MenuToggleItem(const char *labelInst, bool &valueInst, std::function<void()> onSaveInst, const unsigned char *iconInst, bool visibleInst = true);

    // Overridden method for drawing the toggle item.

    /**
     * @brief Draws the toggle item on the display.
     *
     * @param display Reference to the U8G2 display.
     * @param offsetY The vertical offset for drawing the item.
     * @param active Flag indicating if the item is active.
    */
    void Draw(U8G2 &display, int offsetY, bool active) override;

    /**
     * @brief Performs the action associated with the toggle item.
    */
    void Action() override;

    /**
     * @brief Gets the type of the toggle item.
     *
     * @return The type of the toggle item.
    */
    MenuType GetType() override {return ITEM_TOGGLE; };

private:
    bool  &value; ///< Reference to the boolean variable representing the toggle state.
};