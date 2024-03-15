#include "MenuSubMenu.h"

MenuSubMenu::MenuSubMenu(const char *labelInst, const unsigned char *iconInst, Menu &menu, bool visibleInst) :
                         MenuItem(labelInst, iconInst, visibleInst),  varMenu(menu) {
    this->varMenu.Init();
}

MenuSubMenu::MenuSubMenu(const char *labelInst, Menu &menu, bool visibleInst) :
        MenuItem(labelInst, visibleInst),  varMenu(menu) {
        this->varMenu.Init();
}

void MenuSubMenu::Draw(U8G2 &display, int offsetY, bool active) {
    u8g2_uint_t x;
    if (active) {
        display.setFont(u8g_font_7x14B);
        display.drawRBox(2, 1 + offsetY, display.getWidth() - 4, 20, 0);
        display.setDrawColor(0);
        display.setFontMode(1);
    } else {
        display.setFont(u8g_font_7x14);
    }
    this->width = display.getUTF8Width(this->GetLabel());
    x = this->offset;

    display.setClipWindow(25, 1, display.getWidth() - 11 - 5 - 3,display.getHeight());
    const bool scroll = this->width > display.getWidth() - 25 -5 -3;
    if (scroll) {
        char buffer[strlen(this->GetLabel())+3];
        sprintf(buffer, "%s - ", this->GetLabel());
        this->width = display.getUTF8Width(buffer);
        do {
            display.drawUTF8(x + 25, 4 + offsetY, buffer);
            x += this->width;
        } while (x < display.getDisplayWidth());
    } else {
        display.drawUTF8(25, 4 + offsetY, this->GetLabel());
    }
    display.setMaxClipWindow();


    const unsigned char *icon1 = this->GetIcon();
    if (icon1 != nullptr) {
        display.drawXBMP(4, 3 + offsetY, 16, 16, icon1);
    }

    display.setDrawColor(1);
    display.setFontMode(0);


    if (scroll) {
        this->offset -= 1;
        if ( (u8g2_uint_t)offset < (u8g2_uint_t)-width) {
            this->offset = 0;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void MenuSubMenu::Event(EventType evType, EventState evState) {
    LOG(DEBUG, "Submenu Events");
    this->varMenu.Event(evType, evState);
}

void MenuSubMenu::Loop() {
    if(!this->varMenu.IsOpen()) {
        this->Abort();
    }
    this->varMenu.Loop();
}

void MenuSubMenu::Action() {
    this->activeScreen = true;
    this->varMenu.Open();
}

void MenuSubMenu::Abort() {
    this->activeScreen = false;
}