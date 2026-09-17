#include "MenuInfoItem.h"

MenuInfoItem::MenuInfoItem(const char *labelInst, const std::vector<MenuInfoEntry>& entriesInst, bool visibleInst)
    : MenuItem(labelInst, visibleInst), entries(entriesInst) {
}

MenuInfoItem::MenuInfoItem(const char *labelInst, const std::vector<MenuInfoEntry>& entriesInst, const unsigned char *iconInst, bool visibleInst)
    : MenuItem(labelInst, iconInst, visibleInst), entries(entriesInst) {
}

void MenuInfoItem::Draw(U8G2 &display, int offsetY, bool active) {
    if (this->activeScreen) {
        this->drawInfoScreen(display);
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

void MenuInfoItem::Action() {
    this->activeScreen = true;
    this->scrollOffset = 0;
}

void MenuInfoItem::drawInfoScreen(U8G2 &display) {
    u8g2_uint_t height = display.getHeight();
    u8g2_uint_t width = display.getWidth();

    display.setCursor(0,0);
    display.setFontMode(1);
    display.setBitmapMode(1);

    if (this->entries.empty()) return;

    int maxVisible = 4; // 4 items per page
    
    int scrollbarX = width - 4;
    
    // Draw Scrollbar on the right
    if (this->entries.size() > (size_t)maxVisible) {
        display.drawFrame(scrollbarX, 2, 3, height - 4);
        
        float scrollFraction = (float)this->scrollOffset / (float)(this->entries.size() - maxVisible);
        int maxHandleHeight = height - 4 - 2;
        int handleHeight = std::max(4, maxHandleHeight / (int)(this->entries.size() - maxVisible + 1));
        int handleY = 3 + scrollFraction * (maxHandleHeight - handleHeight);
        
        display.drawBox(scrollbarX, handleY, 3, handleHeight);
    }
    
    // Draw Entries
    // We have 64px total height. 4 items * 16px = 64px.
    // Each item is drawn on a single line. Label on the left (normal), Value on the right (bold, scrolling if too long).
    int startY = 3;
    bool anyScroll = false;
    for (int i = 0; i < maxVisible; i++) {
        int entryIndex = this->scrollOffset + i;
        if (entryIndex >= (int)this->entries.size()) break;
        
        // Label
        display.setFont(u8g_font_6x13);
        display.drawUTF8(2, startY, this->entries[entryIndex].label);
        int baseLabelWidth = display.getUTF8Width(this->entries[entryIndex].label);
        display.drawUTF8(2 + baseLabelWidth, startY, ":");
        int labelWidth = baseLabelWidth + display.getUTF8Width(":");
        
        // Value
        display.setFont(u8g_font_6x13B);
        const char* valString = *(this->entries[entryIndex].value);
        if (valString != nullptr) {
            int valueX = 2 + labelWidth + 4; // 4px gap after colon
            int clipRight = scrollbarX - 2;
            int maxValWidth = clipRight - valueX;
            
            display.setClipWindow(valueX, startY, clipRight, startY + 13);
            int valWidth = display.getUTF8Width(valString);
            
            if (valWidth > maxValWidth) {
                anyScroll = true;
                char buffer[strlen(valString) + 4];
                sprintf(buffer, "%s - ", valString);
                int fullValWidth = display.getUTF8Width(buffer);
                
                // Use this->offset (which decrements) for scrolling
                // offset is unsigned, so decrementing below 0 wraps to 65535. 
                // We cast to int16_t to get 0, -1, -2, etc.
                int16_t signedOffset = (int16_t)this->offset;
                int shift = signedOffset % fullValWidth;
                int localX = valueX + shift;
                
                do {
                    display.drawUTF8(localX, startY, buffer);
                    localX += fullValWidth;
                } while (localX < clipRight);
            } else {
                display.drawUTF8(valueX, startY, valString);
            }
            display.setMaxClipWindow();
        }
        
        startY += 16; // 13px font height + 3px gap
    }
    
    if (anyScroll) {
        this->offset -= 1;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void MenuInfoItem::Up() {
    if (this->entries.empty()) return;
    int maxVisible = 4;
    if (this->scrollOffset > 0) {
        this->scrollOffset--;
    } else {
        // Wrap around to bottom
        if ((int)this->entries.size() > maxVisible) {
            this->scrollOffset = this->entries.size() - maxVisible;
        }
    }
}

void MenuInfoItem::Down() {
    if (this->entries.empty()) return;
    int maxVisible = 4;
    if (this->scrollOffset < (int)this->entries.size() - maxVisible) {
        this->scrollOffset++;
    } else {
        // Wrap around to top
        this->scrollOffset = 0;
    }
}

void MenuInfoItem::Enter() {
    this->activeScreen = false;
}

void MenuInfoItem::Abort() {
    this->activeScreen = false;
}
