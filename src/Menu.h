/**
 * @file Menu.h
 * 
 * @brief Display menu
*/
#pragma once

#include <cstdint>
#include "clib/u8g2.h"
#include "Logger.h"
#include "items/MenuItem.h"
#include <U8g2lib.h>
#include <memory> // For std::unique_ptr
#include <utility>
#include "icons/menuIcons.h"
#include "types.h"


namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

/**
 * @class Menu
 * @brief Provides functionality for displaying and interacting with a menu.
*/
class Menu {
public:
    /**
     * @brief Constructs a Menu object.
     *
     * @param displayInst The reference to the U8G2 display instance.
    */
    Menu(U8G2 &displayInst);

    /**
     * @brief Initializes the menu.
    */
    void Init();

    /**
     * @brief Opens the menu.
    */
    void Open();

    /**
     * @brief Checks if the menu is open.
     *
     * @return true if the menu is open, false otherwise.
    */
    bool IsOpen();

    /**
     * @brief Closes the menu.
    */
    void Close();

    /**
     * @brief Handles menu navigation and input events.
    */
    void Loop();

    /**
     * @brief Adds an input item to the menu.
     *
     * @param label The label for the input item.
     * @param title The title for the input item.
     * @param pre The prefix for the input item value display.
     * @param post The postfix for the input item value display.
     * @param min The minimum value for the input item.
     * @param max The maximum value for the input item.
     * @param value Reference to the variable storing the input value.
     * @param step The step size for adjusting the input value.
     * @param stepFast The step size for fast adjustment of the input value.
     * @param isInt Flag indicating if the input is an integer.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, double &value, float step=0.1, float stepFast=0.5, bool isInt=false, bool visible = true);

    /**
     * @brief Adds an input item with an icon to the menu.
     *
     * @param label The label for the input item.
     * @param title The title for the input item.
     * @param pre The prefix for the input item value display.
     * @param post The postfix for the input item value display.
     * @param min The minimum value for the input item.
     * @param max The maximum value for the input item.
     * @param value Reference to the variable storing the input value.
     * @param icon The icon for the input item.
     * @param step The step size for adjusting the input value.
     * @param stepFast The step size for fast adjustment of the input value.
     * @param isInt Flag indicating if the input is an integer.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, double &value, const unsigned char *icon, float step=0.1, float stepFast=0.5, bool isInt=false,
                      bool visible = true);

    /**
     * @brief Adds an input item with an icon to the menu.
     *
     * @param label The label for the input item.
     * @param title The title for the input item.
     * @param pre The prefix for the input item value display.
     * @param post The postfix for the input item value display.
     * @param min The minimum value for the input item.
     * @param max The maximum value for the input item.
     * @param onSave The callback function to be executed when the item is activated.
     * @param value Reference to the variable storing the input value.
     * @param step The step size for adjusting the input value.
     * @param stepFast The step size for fast adjustment of the input value.
     * @param isInt Flag indicating if the input is an integer.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, OnSaveFunction &onSave, double &value,  float step=0.1, float stepFast=0.5, bool isInt=false, bool visible = true);

    /**
     * @brief Adds an input item with an icon to the menu.
     *
     * @param label The label for the input item.
     * @param title The title for the input item.
     * @param pre The prefix for the input item value display.
     * @param post The postfix for the input item value display.
     * @param min The minimum value for the input item.
     * @param max The maximum value for the input item.
     * @param onSave The callback function to be executed when the item is activated.
     * @param value Reference to the variable storing the input value.
     * @param icon The icon for the input item.
     * @param step The step size for adjusting the input value.
     * @param stepFast The step size for fast adjustment of the input value.
     * @param isInt Flag indicating if the input is an integer.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, OnSaveFunction &onSave, double &value, const unsigned char *icon, float step=0.1, float stepFast=0.5, bool isInt=false,
                      bool visible = true);

    /**
     * @brief Adds a toggle item to the menu.
     *
     * @param label The label for the toggle item.
     * @param value Reference to the boolean variable representing the toggle state.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddToggleItem(const char *label, bool &value, bool visible = true);


    /**
     * @brief Adds a toggle item to the menu.
     *
     * @param label The label for the toggle item.
     * @param value Reference to the boolean variable representing the toggle state.
     * @param icon The icon for the input item.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddToggleItem(const char *label, bool &value, const unsigned char *icon, bool visible = true);

    /**
     * @brief Adds a toggle item to the menu.
     *
     * @param label The label for the toggle item.
     * @param onSave The callback function to be executed when the item is activated.
     * @param value Reference to the boolean variable representing the toggle state.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddToggleItem(const char *label, OnSaveFunction &onSave, bool &value, bool visible = true);

    /**
     * @brief Adds a toggle item to the menu.
     *
     * @param label The label for the toggle item.
     * @param onSave The callback function to be executed when the item is activated.
     * @param value Reference to the boolean variable representing the toggle state.
     * @param icon The icon for the input item.
     * @param visible Flag indicating if the item is initially visible.
    */
    void AddToggleItem(const char *label, OnSaveFunction &onSave, bool &value, const unsigned char *icon, bool visible = true);

    /**
     * @brief Adds a submenu to the menu.
     *
     * @param label The label for the submenu.
     * @param menu Reference to the submenu to be added.
     * @param visible Flag indicating if the submenu is initially visible.
    */
    void AddSubMenu(const char *label, Menu &menu, bool visible = true);

    /**
     * @brief Adds a submenu to the menu.
     *
     * @param label The label for the submenu.
     * @param menu Reference to the submenu to be added.
     * @param icon The icon for the input item.
     * @param visible Flag indicating if the submenu is initially visible.
    */
    void AddSubMenu(const char *label, Menu &menu, const unsigned char *icon, bool visible = true);

    /**
     * @brief Adds a back item to the menu.
     *
     * @param label The label for the back item.
    */
    void AddBackItem(const char *label);

    /**
     * @brief Adds a back item to the menu.
     *
     * @param label The label for the back item.
     * @param icon The icon for the input item.
    */
    void AddBackItem(const char *label, const unsigned char *icon);

    /**
     * @brief Sets the event handler for menu events.
     *
     * @param function The event handler function.
    */
    void SetEventHandler(std::function<void()> function);

    /**
     * @brief Handles menu events.
    */
    void EventHandler();

    /**
     * @brief Processes an event for the menu.
     *
     * @param type The type of event.
     * @param state The state of the event.
    */
    void Event(EventType type, EventState state);
private:


    int width;
    int height;
    U8G2 &display;
    bool isOpen = false;
    long lastMenuAbort = 0;
    long const abortInterval = 2500;
    int capacity = 100;
    int itemCount = 0;
    int visibleItemCount = 0;
    std::vector<std::shared_ptr<MenuItem>> items;
    std::vector<std::shared_ptr<MenuItem>> visibleItems;
    uint8_t offset = 0;
    uint8_t activeItem = 0;
    static const uint8_t itemsPerPage = 3;
    int itemHeight = this->display.getHeight() / itemsPerPage;
    int yOffset[itemsPerPage];

    EventType buttonHold = EventType::EVENT_NONE;
    unsigned short buttonHoldThreshold = 500;
    unsigned short buttonHoldThresholdFast = 1500;
    unsigned long buttonHoldStartTime = 0;
    unsigned char enterState = 0;

    std::function<void()> handler;
    void holdEvent();

    template<typename T, typename... Args>
    void addItem(Args&&... args);


    void checkVisibleItems();
};