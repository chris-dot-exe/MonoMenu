/**
 * @file MenuItem.h
 *
 * @brief MenuItem
*/
#pragma once


#include <U8g2lib.h>
#include "../types.h"


class MenuItem {
public:



    MenuItem(const char* labelInst, bool visibleInst= true);
    MenuItem(const char* labelInst, const unsigned char *iconInst, bool visibleInst= true);

    MenuItem(const char* labelInst, std::function<void()> onSaveInst, const unsigned char *iconInst, bool visibleInst= true);
    MenuItem(const char *labelInst, std::function<void()> onSaveInst, bool visibleInst);

    virtual bool IsVisible() const;
    const char* GetLabel();
    const unsigned char* GetIcon();
    virtual void Draw(U8G2 &display, int offsetY, bool active) {};
    virtual void Action() {};
    virtual bool ScreenActive() {return activeScreen; };
    virtual void Up(){};
    virtual void UpFast(){};
    virtual void Down(){};
    virtual void DownFast(){};
    virtual void Enter() {};
    virtual void Abort() {};
    virtual void Loop() {};
    virtual void Event(EventType evType, EventState evState) {};
    virtual MenuType GetType() {return ITEM_NONE;};

    u8g2_uint_t offset{};
    u8g2_uint_t width{};
    bool activeScreen = false;
private:
    const char* label;
    char* workingLabel;
    const unsigned char *icon{};
    bool visible;
    std::function<void()> onSaveFunc;
protected:
    void onSave();
};
