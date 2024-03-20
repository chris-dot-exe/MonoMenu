/**
 * @file Menu.h
 * 
 * @brief Display menu
*/
#pragma once

#include <cstdint>
#include "userConfig.h"
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
 * @class Menu control class
 * @brief This class provides a display menu
*/

class Menu {
public:

/**
     * @enum InputType
     * @brief Type of input for the menu
     * @details The menu can either be controlled by a rotary encoder with a button or three buttons for up, down and enter
    */


    /**
     * @brief Constructor
     *
     * @param display u8g2 instance of selected display
    */

    Menu(U8G2 &displayInst);

    void Init();
    void Open();
    bool IsOpen();

    void Close();

    void Loop();

    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, double &value, float step=0.1, float stepFast=0.5, bool isInt=false, bool visible = true);

    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, double &value, const unsigned char *icon, float step=0.1, float stepFast=0.5, bool isInt=false,
                      bool visible = true);

    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, const std::function<void()>& onSave, double &value,  float step=0.1, float stepFast=0.5, bool isInt=false, bool visible = true);

    void AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min,
                      float max, const std::function<void()>& onSave, double &value, const unsigned char *icon, float step=0.1, float stepFast=0.5, bool isInt=false,
                      bool visible = true);

    void AddToggleItem(const char *label, bool &value, bool visible = true);
    void AddToggleItem(const char *label, bool &value, const unsigned char *icon, bool visible = true);

    void AddToggleItem(const char *label, const std::function<void()>& onSave, bool &value, bool visible = true);
    void AddToggleItem(const char *label, const std::function<void()>& onSave, bool &value, const unsigned char *icon, bool visible = true);

    void AddSubMenu(const char *label, Menu &menu, bool visible = true);
    void AddSubMenu(const char *label, Menu &menu, const unsigned char *icon, bool visible = true);

    void AddBackItem(const char *label);
    void AddBackItem(const char *label, const unsigned char *icon);

    void SetEventHandler(std::function<void()> function);
    void EventHandler();
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