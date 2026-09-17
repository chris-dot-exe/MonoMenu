#include "MenuEnumItem.h"
#include "../Menu.h"

MenuEnumItem::MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, bool visibleInst)
    : MenuItem(labelInst, visibleInst), value(valueInst), title(titleInst), options(optionsInst, optionsInst + numOptionsInst) {
}

MenuEnumItem::MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, const unsigned char *iconInst, bool visibleInst)
    : MenuItem(labelInst, iconInst, visibleInst), value(valueInst), title(titleInst), options(optionsInst, optionsInst + numOptionsInst) {
}

MenuEnumItem::MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, OnSaveFunction onSaveInst, bool visibleInst)
    : MenuItem(labelInst, onSaveInst, visibleInst), value(valueInst), title(titleInst), options(optionsInst, optionsInst + numOptionsInst) {
}

MenuEnumItem::MenuEnumItem(const char *labelInst, const char *titleInst, const char* const* optionsInst, uint8_t numOptionsInst, uint8_t &valueInst, OnSaveFunction onSaveInst, const unsigned char *iconInst, bool visibleInst)
    : MenuItem(labelInst, onSaveInst, iconInst, visibleInst), value(valueInst), title(titleInst), options(optionsInst, optionsInst + numOptionsInst) {
}

void MenuEnumItem::Draw(U8G2 &display, int offsetY, bool active) {
    if (this->editing) {
        this->drawEnumScreen(display);
    } else {
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

        display.setClipWindow(25, 1, display.getWidth() - 11 - 5 - 3, display.getHeight());
        const bool scroll = this->width > display.getWidth() - 25 - 5 - 3;
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
            if ((u8g2_uint_t) offset < (u8g2_uint_t) -width) {
                this->offset = 0;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}

void MenuEnumItem::Action() {
    this->editing = !this->editing;
    this->workingVal = this->value;
    if (this->workingVal >= this->options.size()) {
        this->workingVal = 0;
    }
    
    this->activeScreen = true;
}

void MenuEnumItem::drawEnumScreen(U8G2 &display) {
    u8g2_uint_t textWidth;
    u8g2_uint_t height = display.getHeight();

    display.setCursor(0,0);
    display.setFontMode(1);
    display.setBitmapMode(1);

    // Draw Title (Top)
    display.setFont(u8g_font_7x14B);
    textWidth = display.getUTF8Width(this->title);
    display.drawUTF8(((display.getWidth() - textWidth) / 2), 0, this->title);
    display.drawLine(2, 14, 124, 14);

    // Draw Footer (Bottom)
    display.setFont(u8g_font_5x8); // small font
    const char* footerText = "^/v: Switch  Enter: Save";
    u8g2_uint_t footerWidth = display.getUTF8Width(footerText);
    int footerY = height - 8;
    display.drawLine(2, height - 10, 124, height - 10);
    display.drawUTF8(((display.getWidth() - footerWidth) / 2), footerY, footerText);

    if (this->options.empty()) return;

    // Draw current active option (Center)
    if (this->workingVal == this->value) {
        display.setFont(u8g_font_7x14B); // Bold if currently saved value
    } else {
        display.setFont(u8g_font_7x14); // Normal if just scrolling past
    }
    const char* currentOption = this->options[this->workingVal];
    
    // Draw left and right arrows to indicate scrolling is possible
    // < Option Name >
    char centerText[256];
    snprintf(centerText, sizeof(centerText), "< %s >", currentOption);
    
    u8g2_uint_t centerWidth = display.getUTF8Width(centerText);
    
    // Center it vertically between the two lines (y=14 and y=height-10)
    // Available space: (height - 10) - 14 = height - 24
    // Middle of this space is 14 + (height - 24) / 2 = 14 + height/2 - 12 = height/2 + 2
    // Font is top-aligned (from our previous testing), font height is ~11px
    int centerY = 14 + ((height - 24) / 2) - 6; 
    
    display.drawUTF8(((display.getWidth() - centerWidth) / 2), centerY, centerText);
}

void MenuEnumItem::Up() {
    if (this->options.empty()) return;
    if (this->workingVal > 0) {
        this->workingVal--;
    } else {
        this->workingVal = this->options.size() - 1;
    }
}

void MenuEnumItem::Down() {
    if (this->options.empty()) return;
    if (this->workingVal < this->options.size() - 1) {
        this->workingVal++;
    } else {
        this->workingVal = 0;
    }
}

void MenuEnumItem::Enter() {
    auto combinedAction = [this]() {
        this->value = this->workingVal;
        this->editing = false;
        this->activeScreen = false;
        this->onSave();

        if (this->customConfirmAction) {
            this->customConfirmAction();
        }
    };

    if (this->HasConfirm() && this->menu) {
        this->menu->OpenConfirmDialog(this->confirmText, combinedAction);
    } else {
        combinedAction();
    }
}

void MenuEnumItem::Abort() {
    this->editing = false;
    this->activeScreen = false;
}
