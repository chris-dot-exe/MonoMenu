/**
 * @file MenuSubMenu.h
 *
 * @brief Defines the MenuSubMenu class for representing a submenu item in a menu.
*/
#pragma once

#include "../Menu.h"

/**
 * @class MenuSubMenu
 * @brief Represents a submenu item in a menu.
*/
class MenuSubMenu: public MenuItem {
public:
    /**
     * @brief Constructs a MenuSubMenu object with the specified label, menu, and visibility.
     *
     * @param labelInst The label for the submenu item.
     * @param menu Reference to the submenu menu.
     * @param visibleInst Flag indicating the initial visibility of the submenu item.
    */
    MenuSubMenu(const char *labelInst, Menu &menu, bool visibleInst);

    /**
     * @brief Constructs a MenuSubMenu object with the specified label, icon, menu, and visibility.
     *
     * @param labelInst The label for the submenu item.
     * @param iconInst The icon for the submenu item.
     * @param menu Reference to the submenu menu.
     * @param visibleInst Flag indicating the initial visibility of the submenu item.
    */
    MenuSubMenu(const char *labelInst, const unsigned char *iconInst, Menu &menu, bool visibleInst);

    // Overridden methods for handling submenu item specific functionality.

    /**
     * @brief Draws the submenu item on the display.
     *
     * @param display Reference to the U8G2 display.
     * @param offsetY The vertical offset for drawing the item.
     * @param active Flag indicating if the item is active.
    */
    void Draw(U8G2 &display, int offsetY, bool active) override;

    /**
     * @brief Handles an event associated with the submenu item.
     *
     * @param type The type of event.
     * @param state The state of the event.
    */
    void Event(EventType type, EventState state) override;

    /**
     * @brief Performs actions during the loop for the submenu item.
    */
    void Loop() override;

    /**
     * @brief Gets the type of the submenu item.
     *
     * @return The type of the submenu item.
    */
    MenuType GetType() override {return ITEM_SUBMENU; };

    /**
     * @brief Performs the action associated with the submenu item.
    */
    void Action() override;

    /**
     * @brief Aborts the action associated with the submenu item.
    */
    void Abort() override;

private:
    Menu &varMenu; ///< Reference to the submenu menu.
};