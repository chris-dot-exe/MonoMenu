/**
 * @file MenuBackItem.h
 * 
 * @brief Defines the MenuBackItem class for representing a back item in a menu.
*/

#pragma once

#include "MenuItem.h"
#include "../Menu.h"

/**
 * @class MenuBackItem
 * @brief Represents a back item in a menu, which allows navigation to the previous menu level.
*/
class MenuBackItem: public MenuItem {
public:
    /**
     * @brief Constructs a MenuBackItem object with the specified label and menu instance.
     *
     * @param labelInst The label for the back item.
     * @param menuInst Pointer to the menu instance.
    */
    explicit MenuBackItem(const char *labelInst, Menu *menuInst);

    /**
     * @brief Constructs a MenuBackItem object with the specified label, icon, and menu instance.
     *
     * @param labelInst The label for the back item.
     * @param iconInst The icon for the back item.
     * @param menuInst Pointer to the menu instance.
    */
    MenuBackItem(const char *labelInst, const unsigned char *iconInst, Menu *menuInst);

    /**
     * @brief Performs the action associated with the back item, which typically navigates to the previous menu level.
    */
    void Action() override;

    /**
     * @brief Draws the back item on the display.
     *
     * @param display Reference to the U8G2 display.
     * @param offsetY The vertical offset for drawing the item.
     * @param active Flag indicating if the item is active.
    */
    void Draw(U8G2 &display, int offsetY, bool active) override;

private:
    Menu *menu; ///< Pointer to the menu instance.
};  