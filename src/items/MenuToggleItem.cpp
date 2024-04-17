#include "MenuToggleItem.h"

MenuToggleItem::MenuToggleItem(const char *labelInst, bool  &valueInst, bool visibleInst)
        : MenuItem(labelInst, visibleInst), value(valueInst) {

}

MenuToggleItem::MenuToggleItem(const char *labelInst, bool  &valueInst, const unsigned char *iconInst, bool visibleInst)
        : MenuItem(labelInst, iconInst, visibleInst), value(valueInst)   {
}

MenuToggleItem::MenuToggleItem(const char *labelInst, bool  &valueInst, OnSaveFunction onSaveInst, bool visibleInst)
        : MenuItem(labelInst, onSaveInst, visibleInst), value(valueInst) {

}

MenuToggleItem::MenuToggleItem(const char *labelInst, bool  &valueInst, OnSaveFunction onSaveInst, const unsigned char *iconInst, bool visibleInst)
        : MenuItem(labelInst, onSaveInst, iconInst, visibleInst), value(valueInst)   {
}

void MenuToggleItem::Draw(U8G2 &display, int offsetY, bool active) {
    u8g2_uint_t x;
    if (active) {
        display.setFont(u8g_font_7x14B);
        display.drawRBox(2, 1 + offsetY, display.getWidth() - 4, 20, 0);
        display.setDrawColor(0);
        display.setFontMode(1);
    } else {
        display.setFont(u8g_font_7x14);
    }
    this->width = display.getUTF8Width(this->GetLabel())+ 7;
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

    display.drawEllipse(display.getWidth() - 11, 11 + offsetY, 5, 5);
    if (this->value == 1) {
        display.drawFilledEllipse(display.getWidth() - 11, 11 + offsetY, 3, 3);
    } else {
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

void MenuToggleItem::Action() {
    this->value = !this->value;
    this->onSave();
}
