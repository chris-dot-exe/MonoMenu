#pragma once

#include "MenuItem.h"
#include "../Menu.h"

class MenuBackItem: public MenuItem {
public:
    explicit MenuBackItem(const char *labelInst, Menu *menuInst);
    MenuBackItem(const char *labelInst, const unsigned char *iconInst, Menu *menuInst);

    void Action() override;

    void Draw(U8G2 &display, int offsetY, bool active) override;
private:
    Menu *menu;
};