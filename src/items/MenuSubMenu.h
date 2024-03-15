#pragma once

#include "../Menu.h"

class MenuSubMenu: public MenuItem {
public:
    MenuSubMenu(const char *labelInst, Menu &menu, bool visibleInst);
    MenuSubMenu(const char *labelInst, const unsigned char *iconInst, Menu &menu, bool visibleInst);

    void Draw(U8G2 &display, int offsetY, bool active) override;
    void Event(EventType type, EventState state) override;
    void Loop() override;
    MenuType GetType() override {return ITEM_SUBMENU; };
    void Action() override;
    void Abort() override;

private:

    Menu &varMenu;
};
