/**
 * @file MenuInfoItem.h
 *
 * @brief Defines the MenuInfoItem class for representing a read-only list of info.
 */
#pragma once

#include "MenuItem.h"
#include "../types.h"
#include <vector>

// MenuInfoEntry is now defined in types.h

/**
 * @class MenuInfoItem
 * @brief Represents a menu item that opens a sub-screen to display a list of information.
 */
class MenuInfoItem : public MenuItem {
public:
    /**
     * @brief Constructs a MenuInfoItem object.
     *
     * @param labelInst The label of the menu item.
     * @param entriesInst The list of label/value entries to display.
     * @param visibleInst Flag indicating if the menu item is initially visible.
     */
    MenuInfoItem(const char *labelInst, const std::vector<MenuInfoEntry>& entriesInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuInfoItem object with an icon.
     */
    MenuInfoItem(const char *labelInst, const std::vector<MenuInfoEntry>& entriesInst, const unsigned char *iconInst, bool visibleInst = true);

    /**
     * @brief Draws the item on the display.
     */
    void Draw(U8G2 &display, int offsetY, bool active) override;

    /**
     * @brief Performs the action associated with the item (opens the info screen).
     */
    void Action() override;

    /**
     * @brief Handles the 'Up' action for scrolling up.
     */
    void Up() override;

    /**
     * @brief Handles the 'Down' action for scrolling down.
     */
    void Down() override;

    /**
     * @brief Handles the 'Enter' action for closing the screen.
     */
    void Enter() override;

    /**
     * @brief Handles the 'Abort' action for closing the screen.
     */
    void Abort() override;

    /**
     * @brief Gets the type of the menu item.
     */
    MenuType GetType() override { return ITEM_INFO; };

private:
    std::vector<MenuInfoEntry> entries; ///< List of entries to display.
    uint8_t scrollOffset = 0; ///< The index of the first visible entry.
    
    /**
     * @brief Draws the sub-screen for the info list.
     *
     * @param display Reference to the U8G2 display.
     */
    void drawInfoScreen(U8G2 &display);
};
