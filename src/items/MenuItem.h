/**
 * @file MenuItem.h
 *
 * @brief Defines the MenuItem class for representing items in a menu.
*/
#pragma once

#include <U8g2lib.h>
#include "../types.h"
#include <functional>

/**
 * @class MenuItem
 * @brief Represents an item in a menu.
*/
class MenuItem {
public:

    /**
     * @brief Constructs a MenuItem object with the specified label and visibility.
     *
     * @param labelInst The label for the menu item.
     * @param visibleInst Flag indicating the initial visibility of the menu item.
    */
    MenuItem(const char* labelInst, bool visibleInst= true);

    /**
     * @brief Constructs a MenuItem object with the specified label, icon, and visibility.
     *
     * @param labelInst The label for the menu item.
     * @param iconInst The icon for the menu item.
     * @param visibleInst Flag indicating the initial visibility of the menu item.
    */
    MenuItem(const char* labelInst, const unsigned char *iconInst, bool visibleInst= true);

    /**
     * @brief Constructs a MenuItem object with the specified label, callback function, icon, and visibility.
     *
     * @param labelInst The label for the menu item.
     * @param onSaveInst The callback function to be executed when the item is activated.
     * @param iconInst The icon for the menu item.
     * @param visibleInst Flag indicating the initial visibility of the menu item.
    */
    MenuItem(const char* labelInst, std::function<void()> onSaveInst, const unsigned char *iconInst, bool visibleInst= true);

    /**
     * @brief Constructs a MenuItem object with the specified label, callback function, and visibility.
     *
     * @param labelInst The label for the menu item.
     * @param onSaveInst The callback function to be executed when the item is activated.
     * @param visibleInst Flag indicating the initial visibility of the menu item.
    */
    MenuItem(const char *labelInst, std::function<void()> onSaveInst, bool visibleInst);

    /**
     * @brief Checks if the menu item is visible.
     *
     * @return true if the menu item is visible, false otherwise.
    */
    virtual bool IsVisible() const;

    /**
     * @brief Gets the label of the menu item.
     *
     * @return The label of the menu item.
    */
    const char* GetLabel();

    /**
     * @brief Gets the icon of the menu item.
     *
     * @return The icon of the menu item.
    */
    const unsigned char* GetIcon();

    /**
     * @brief Draws the menu item on the display.
     *
     * @param display Reference to the U8G2 display.
     * @param offsetY The vertical offset for drawing the item.
     * @param active Flag indicating if the item is active.
    */
    virtual void Draw(U8G2 &display, int offsetY, bool active) {};

    // Methods for handling user interaction with the menu item.

    /**
     * @brief Performs the action associated with the menu item.
    */
    virtual void Action() {};

    /**
     * @brief Checks if the screen associated with the menu item is active.
     *
     * @return true if the screen is active, false otherwise.
    */
    virtual bool ScreenActive() {return activeScreen; };

    /**
     * @brief Handles the "Up" input action.
    */
    virtual void Up(){};

    /**
     * @brief Handles the "Fast Up" input action.
    */
    virtual void UpFast(){};

    /**
     * @brief Handles the "Down" input action.
    */
    virtual void Down(){};

    /**
     * @brief Handles the "Fast Down" input action.
    */
    virtual void DownFast(){};

    /**
     * @brief Handles the "Enter" input action.
    */
    virtual void Enter() {};

    /**
     * @brief Handles the "Abort" input action.
    */
    virtual void Abort() {};

    /**
     * @brief Performs the actions required during the loop.
    */
    virtual void Loop() {};

    /**
     * @brief Handles an event associated with the menu item.
     *
     * @param evType The type of event.
     * @param evState The state of the event.
    */
    virtual void Event(EventType evType, EventState evState) {};

    /**
     * @brief Gets the type of the menu item.
     *
     * @return The type of the menu item.
    */
    virtual MenuType GetType() {return ITEM_NONE;};

    u8g2_uint_t offset{}; ///< Offset of the menu item.
    u8g2_uint_t width{}; ///< Width of the menu item.
    bool activeScreen = false; ///< Flag indicating if the screen is active.

private:
    const char* label; ///< The label of the menu item.
    char* workingLabel; ///< Working label of the menu item.
    const unsigned char *icon; ///< The icon of the menu item.
    bool visible; ///< Visibility of the menu item.
    std::function<void()> onSaveFunc; ///< Callback function to be executed when the item is activated.

protected:
    /**
     * @brief Executes the onSave function associated with the menu item.
    */
    void onSave();
};