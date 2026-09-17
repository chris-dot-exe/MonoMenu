/**
 * @file MenuEnumItem.h
 *
 * @brief Defines the MenuEnumItem class for representing an enumeration selection menu.
 */
#pragma once

#include "MenuItem.h"
#include "../types.h"
#include <vector>

/**
 * @class MenuEnumItem
 * @brief Represents a menu item that opens a sub-screen to select one option from an array.
 */
class MenuEnumItem : public MenuItem {
public:
    /**
     * @brief Constructs a MenuEnumItem object.
     *
     * @param labelInst The label of the menu item.
     * @param titleInst The title of the enum screen.
     * @param optionsInst Pointer to the array of strings for the options.
     * @param numOptionsInst The number of options in the array.
     * @param valueInst Reference to the variable storing the selected option index.
     * @param visibleInst Flag indicating if the menu item is initially visible.
     */
    MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuEnumItem object with an icon.
     */
    MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, const unsigned char *iconInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuEnumItem object with an OnSaveFunction.
     */
    MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, OnSaveFunction onSaveInst, bool visibleInst = true);

    /**
     * @brief Constructs a MenuEnumItem object with an icon and OnSaveFunction.
     */
    MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, OnSaveFunction onSaveInst, const unsigned char *iconInst, bool visibleInst = true);

    /**
     * @brief Draws the item on the display.
     */
    void Draw(U8G2 &display, int offsetY, bool active) override;

    /**
     * @brief Performs the action associated with the item (opens the enum screen).
     */
    void Action() override;

    /**
     * @brief Handles the 'Up' action for navigating options.
     */
    void Up() override;

    /**
     * @brief Handles the 'Down' action for navigating options.
     */
    void Down() override;

    /**
     * @brief Handles the 'Enter' action for confirming the selected option.
     */
    void Enter() override;

    /**
     * @brief Handles the 'Abort' action for cancelling.
     */
    void Abort() override;

    /**
     * @brief Gets the type of the menu item.
     */
    MenuType GetType() override { return ITEM_ENUM; };

private:
    uint8_t &value; ///< Reference to the selected option index.
    uint8_t workingVal; ///< Temporary index while editing.
    const char *title; ///< Title for the sub-screen.
    std::vector<const char*> options; ///< Vector of strings for the options.
    bool editing = false; ///< Flag indicating if the enum screen is active.

    /**
     * @brief Draws the sub-screen for enum selection.
     *
     * @param display Reference to the U8G2 display.
     */
    void drawEnumScreen(U8G2 &display);
};
