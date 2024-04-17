#include "MenuInputItem.h"

MenuInputItem::MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst,
                             float minInst, float maxInst, double &valueInst, float stepInst, float stepFastInst,
                             bool isIntInst, bool visibleInst)
    : MenuItem(labelInst, visibleInst), value(valueInst), title(titleInst),
    pre(preInst), min(minInst), max(maxInst), post(postInst), step(stepInst), stepFast(stepFastInst), isInt(isIntInst) {

}

MenuInputItem::MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst,
                             float minInst, float maxInst, double &valueInst, const unsigned char *iconInst,
                             float stepInst, float stepFastInst, bool isIntInst, bool visibleInst)
    : MenuItem(labelInst, iconInst, visibleInst), value(valueInst), title(titleInst),
    pre(preInst), min(minInst), max(maxInst), post(postInst), step(stepInst), stepFast(stepFastInst), isInt(isIntInst) {
}

MenuInputItem::MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst,
                             float minInst, float maxInst, double &valueInst, OnSaveFunction onSaveInst, float stepInst, float stepFastInst,
                             bool isIntInst, bool visibleInst)
        : MenuItem(labelInst, onSaveInst, visibleInst), value(valueInst), title(titleInst),
          pre(preInst), min(minInst), max(maxInst), post(postInst), step(stepInst), stepFast(stepFastInst), isInt(isIntInst) {

}

MenuInputItem::MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst,
                             float minInst, float maxInst, double &valueInst, OnSaveFunction onSaveInst, const unsigned char *iconInst,
                             float stepInst, float stepFastInst, bool isIntInst, bool visibleInst)
        : MenuItem(labelInst, onSaveInst, iconInst, visibleInst), value(valueInst), title(titleInst),
          pre(preInst), min(minInst), max(maxInst), post(postInst), step(stepInst), stepFast(stepFastInst), isInt(isIntInst) {
}

void MenuInputItem::init() {
    if (this->workingVal < this->min) {this->workingVal = this->min;}
    if (this->workingVal > this->max) {this->workingVal = this->max;}
}

void MenuInputItem::Draw(U8G2 &display, int offsetY, bool active) {
    if (this->editing) {
        //display.userInterfaceInputValue(this->title, this->pre, this->workingVal, this->min, this->max, 1, this->post);
        this->drawInputScreen(display);
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

void MenuInputItem::Action() {
    this->editing = !this->editing;
    this->workingVal = this->value;
    this->activeScreen = true;
}

void MenuInputItem::drawInputScreen(U8G2 &display) {
    char buffer[32];
    u8g2_uint_t width;
    u8g2_uint_t height = display.getHeight();

    display.setCursor(0,0);

    display.setFontMode(1);
    display.setBitmapMode(1);

    display.setFont(u8g_font_7x14B);
    width = display.getUTF8Width(this->title);
    display.drawUTF8(((display.getWidth() - width) / 2), 0, this->title);
    display.drawLine(2, 14, 124, 14);


    display.setFont(u8g_font_5x8);

    if(this->isInt) {
        sprintf(buffer, "Max: %.0f%s", this->max, this->post);
    } else {
        sprintf(buffer, "Max: %.1f%s", this->max, this->post);
    }
    width = display.getUTF8Width(buffer);
    display.drawUTF8(display.getWidth() - width, height-8, buffer);
    if(this->isInt) {
        sprintf(buffer, "Min: %.0f%s", this->min, this->post);
    } else {
        sprintf(buffer, "Min: %.1f%s", this->min, this->post);
    }

    display.drawUTF8(0, height-8, buffer);


    display.setFont(u8g_font_9x18B);

    if(this->isInt) {
        sprintf(buffer, "%s %.0f%s", this->pre, this->workingVal, this->post);
    } else {
        sprintf(buffer, "%s %.1f%s", this->pre, this->workingVal, this->post);
    }
    width = display.getUTF8Width(buffer);
    display.drawUTF8(((display.getWidth() - width) / 2), 39-9, buffer);

}

void MenuInputItem::Up() {
    this->workingVal += this->step;
    if(this->workingVal >= this->max) {
        this->workingVal = this->max;
    }
}

void MenuInputItem::UpFast() {
    this->workingVal += this->stepFast;
    if(this->workingVal >= this->max) {
        this->workingVal = this->max;
    }
}

void MenuInputItem::Down() {
    this->workingVal -= this->step;
    if(this->workingVal <= this->min) {
        this->workingVal = this->min;
    }
}

void MenuInputItem::DownFast() {
    this->workingVal -= this->stepFast;
    if(this->workingVal <= this->min) {
        this->workingVal = this->min;
    }
}

void MenuInputItem::Enter() {
    this->value = this->workingVal;
    this->editing = false;
    this->activeScreen = false;
    this->onSave();
}

void MenuInputItem::Abort() {
    this->editing = false;
    this->activeScreen = false;
}