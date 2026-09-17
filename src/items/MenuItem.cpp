#include "MenuItem.h"
#include <Arduino.h>
#include "../Menu.h"
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
    return hasIcon ? icon : nullptr;
}

void MenuItem::SetMenu(Menu* m) {
    this->menu = m;
}

void MenuItem::SetConfirm(const char* text, std::function<void()> customAction) {
    this->confirmText = text;
    this->customConfirmAction = customAction;
}

bool MenuItem::HasConfirm() const {
    return this->confirmText != nullptr;
}


MenuItem::MenuItem(const char *labelInst, bool visibleInst)
        : label(labelInst), visible(visibleInst), icon(nullptr) {

}
MenuItem::MenuItem(const char *labelInst, OnSaveFunction onSaveInst, bool visibleInst)
        : label(labelInst), onSaveFunc(std::move(onSaveInst)), visible(visibleInst), icon(nullptr) {

}
MenuItem::MenuItem(const char *labelInst, const unsigned char *iconInst, bool visibleInst)
        : label(labelInst), icon(iconInst), visible(visibleInst), hasIcon(true) {

}

MenuItem::MenuItem(const char *labelInst, OnSaveFunction onSaveInst, const unsigned char *iconInst, bool visibleInst)
        : label(labelInst), onSaveFunc(std::move(onSaveInst)), icon(iconInst), visible(visibleInst), hasIcon(true) {

}

void MenuItem::onSave() {
  if (this->onSaveFunc) {
    //LOG(DEBUG, "onSave");
    this->onSaveFunc();
  }
}

void MenuItem::Action() {
    auto combinedAction = [this]() {
        this->onSave();
        if (this->customConfirmAction) this->customConfirmAction();
    };

    if (this->HasConfirm() && this->menu) {
        this->menu->OpenConfirmDialog(this->confirmText, combinedAction);
    } else {
        combinedAction();
    }
}

