#pragma once

#include "MenuItem.h"
#include "Logger.h"
#include "../types.h"

/**
     * @enum MenuToggleItem
     * @brief Type of menu item that renders a toggle right to the label
    */
class MenuToggleItem : public MenuItem {
public:
/**
     * @brief Constructor
     *
     * @param label Label of the menu item
     * @param value Value of the toggle
     * @param icon Icon shown in front of the menu item
     * @param visible Visibility of the menu item

    */
    MenuToggleItem(const char *labelInst, bool &valueInst, bool visibleInst = true);
    MenuToggleItem(const char *labelInst, bool &valueInst, const unsigned char *iconInst, bool visibleInst = true);
    MenuToggleItem(const char *labelInst, bool &valueInst, std::function<void()> onSaveInst, bool visibleInst = true);
    MenuToggleItem(const char *labelInst, bool &valueInst, std::function<void()> onSaveInst, const unsigned char *iconInst, bool visibleInst = true);

    void Draw(U8G2 &display, int offsetY, bool active) override;
    void Action() override;
    MenuType GetType() override {return ITEM_TOGGLE; };
private:
    bool  &value;
};