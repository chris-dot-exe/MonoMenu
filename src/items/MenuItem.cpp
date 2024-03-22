#include "MenuItem.h"

#include <utility>
#include "U8g2lib.h"
#include "Logger.h"

bool MenuItem::IsVisible() const {
    return this->visible;
}

const char *MenuItem::GetLabel() {
    return this->label;
}

const unsigned char *MenuItem::GetIcon() {
    return this->icon;
}


MenuItem::MenuItem(const char *labelInst, bool visibleInst)
    : label(labelInst), visible(visibleInst) {

}
MenuItem::MenuItem(const char *labelInst, std::function<void()> onSaveInst, bool visibleInst)
        : label(labelInst), onSaveFunc(std::move(onSaveInst)), visible(visibleInst) {

}
MenuItem::MenuItem(const char *labelInst, const unsigned char *iconInst, bool visibleInst)
    : label(labelInst), icon(iconInst), visible(visibleInst) {

}

MenuItem::MenuItem(const char *labelInst, std::function<void()> onSaveInst, const unsigned char *iconInst, bool visibleInst)
    : label(labelInst), onSaveFunc(std::move(onSaveInst)), icon(iconInst), visible(visibleInst){

}

void MenuItem::onSave() {
    if (this->onSaveFunc) {
        //LOG(DEBUG, "onSave");
        this->onSaveFunc();
    }
}

