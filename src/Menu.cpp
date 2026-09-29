#include "Menu.h"
#include "items/MenuSubMenu.h"
#include "items/MenuInputItem.h"
#include "items/MenuToggleItem.h"
#include "items/MenuEnumItem.h"
#include "items/MenuInfoItem.h"
#include "items/MenuBackItem.h"
#include <typeinfo>

Menu::Menu(U8G2 &displayInst) : display(displayInst) {

}

void Menu::Open() {
  this->isOpen = true;
  this->lastInteraction = millis();
}

bool Menu::IsOpen() {
  return this->isOpen;
}

void Menu::Close() {
  this->offset = 0;
  this->activeItem = 0;
  this->isOpen = false;
  this->isConfirmDialogOpen = false;
}

void Menu::OpenConfirmDialog(const char* text, std::function<void()> onOkAction) {
    this->isConfirmDialogOpen = true;
    this->confirmDialogSelection = 0; // Default to Cancel
    this->confirmDialogText = text != nullptr ? text : "Confirm?";
    this->pendingConfirmAction = onOkAction;
}

bool Menu::invertInput = false;
bool* Menu::invertInputPtr = nullptr;
bool Menu::invertScroll = false;
bool* Menu::invertScrollPtr = nullptr;

void Menu::InvertMenuInput(bool invert) {
    Menu::invertInput = invert;
}
void Menu::InvertMenuInput(bool* invert) {
    Menu::invertInputPtr = invert;
}

void Menu::InvertScrollInput(bool invert) {
    Menu::invertScroll = invert;
}
void Menu::InvertScrollInput(bool* invert) {
    Menu::invertScrollPtr = invert;
}

void Menu::holdEvent() {
  if (this->buttonHold >= EventType::EVENT_NONE) {
    if (this->visibleItems[this->activeItem]->ScreenActive()) {
      if (millis() - this->buttonHoldStartTime >= this->buttonHoldThreshold) {
        if (millis() - this->buttonHoldStartTime >= buttonHoldThresholdFast) {
          if (this->buttonHold == EventType::EVENT_UP) {
            this->visibleItems[this->activeItem]->UpFast();
          } else if (this->buttonHold == EventType::EVENT_DOWN) {
            this->visibleItems[this->activeItem]->DownFast();
          }

        } else if (millis() - this->buttonHoldStartTime >= this->abortInterval) {
          //LOG(DEBUG, "holdEvent abort");
        } else {
          if (this->buttonHold == EventType::EVENT_UP) {
            this->visibleItems[this->activeItem]->Up();
          } else if (this->buttonHold == EventType::EVENT_DOWN) {
            this->visibleItems[this->activeItem]->Down();
          }
        }
      }
    }

  }
}

void Menu::Loop() {

  this->holdEvent();
  if (this->visibleItems[this->activeItem]->GetType() == ITEM_SUBMENU &&
      this->visibleItems[this->activeItem]->ScreenActive()) {
    this->visibleItems[this->activeItem]->Loop();
    return;
  }

  if (this->IsOpen()) {

    // check if menu is idle and timeout is hit
    if (this->timeoutActive) {
      if (millis() - this->lastInteraction > this->timeoutLimit) {
        this->Close();
        return;
      }
    }

    this->display.firstPage();
    this->checkVisibleItems();
    do {
      uint8_t index = 0;
      uint8_t renderedCount = 0;
      if (this->visibleItems[this->activeItem]->ScreenActive()) {
        this->visibleItems[this->activeItem]->Draw(this->display, this->yOffset[index], true);
      } else {
        for (int i = this->offset; i < min(this->visibleItemCount, this->offset + itemsPerPage); ++i) {
          if (i == this->activeItem) {
            // Render active item
            this->visibleItems[i]->Draw(this->display, this->yOffset[index],
                                        true); // Pass true to indicate active state
          } else {
            // Render inactive item
            this->visibleItems[i]->Draw(this->display, this->yOffset[index],
                                        false); // Pass false to indicate inactive state
          }
          index++;
        }
      }

      if (this->isConfirmDialogOpen) {
        uint8_t w = this->display.getWidth();
        uint8_t h = this->display.getHeight();
        uint8_t boxW = w - 10;
        uint8_t boxH = h - 20;
        uint8_t boxX = 5;
        uint8_t boxY = 10;
        
        // Background and border
        this->display.setDrawColor(0);
        this->display.drawBox(boxX, boxY, boxW, boxH);
        this->display.setDrawColor(1);
        this->display.drawFrame(boxX, boxY, boxW, boxH);
        
        // Title
        this->display.setFont(u8g_font_6x13); // Better font
        uint8_t textWidth = this->display.getUTF8Width(this->confirmDialogText);
        this->display.drawUTF8(boxX + (boxW - textWidth) / 2, boxY + 4, this->confirmDialogText);
        
        // Buttons
        this->display.setFont(u8g_font_5x8);
        uint8_t btnW = 35;
        uint8_t btnH = 14;
        uint8_t btnY = boxY + boxH - 18;
        uint8_t okX = boxX + 10;
        uint8_t cancelX = boxX + boxW - 10 - btnW;
        
        // Draw Cancel Button
        if (this->confirmDialogSelection == 0) {
            this->display.drawBox(cancelX, btnY, btnW, btnH);
            this->display.setDrawColor(0);
        } else {
            this->display.drawFrame(cancelX, btnY, btnW, btnH);
            this->display.setDrawColor(1);
        }
        uint8_t cancelTextW = this->display.getUTF8Width("Cancel");
        this->display.drawUTF8(cancelX + (btnW - cancelTextW) / 2, btnY + 3, "Cancel");
        this->display.setDrawColor(1);
        
        // Draw OK Button
        if (this->confirmDialogSelection == 1) {
            this->display.drawBox(okX, btnY, btnW, btnH);
            this->display.setDrawColor(0);
        } else {
            this->display.drawFrame(okX, btnY, btnW, btnH);
            this->display.setDrawColor(1);
        }
        uint8_t okTextW = this->display.getUTF8Width("OK");
        this->display.drawUTF8(okX + (btnW - okTextW) / 2, btnY + 3, "OK");
        this->display.setDrawColor(1);
      }

    } while (this->display.nextPage());

    // start over again
  }
}


void Menu::Init() {

  for (int i = 0; i < itemsPerPage; ++i) {
    this->yOffset[i] = i * itemHeight;
  }
}

/*
 * Private
 */
template<typename T, typename... Args>
T* Menu::addItem(Args &&... args) {
  if (this->itemCount >= this->capacity) {
    return nullptr;
  }

  auto newItem = std::make_shared<T>(std::forward<Args>(args)...);
  newItem->SetMenu(this);
  this->items.push_back(newItem);
  this->itemCount += 1;

  if (newItem->IsVisible()) {
    this->visibleItems.push_back(newItem);
    this->visibleItemCount += 1;
  }

  //LOGF(DEBUG, "Added Item");
  return newItem.get();
}

MenuInputItem* Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        double &value, float step, float stepFast, bool isInt, bool visible) {

  return this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, step, stepFast, isInt, visible);

}

MenuInputItem* Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        double &value, const unsigned char *icon, float step, float stepFast, bool isInt,
                        bool visible) {

  return this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, icon, step, stepFast, isInt, visible);

}

MenuInputItem* Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        OnSaveFunction onSave, double &value, const unsigned char *icon,
                        float step, float stepFast, bool isInt, bool visible) {

  return this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, onSave, icon, step, stepFast, isInt, visible);

}

MenuInputItem* Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        OnSaveFunction onSave, double &value, float step, float stepFast, bool isInt,
                        bool visible) {

  return this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, onSave, step, stepFast, isInt, visible);

}

MenuToggleItem* Menu::AddToggleItem(const char *label, bool &value, bool visible) {
  return this->addItem<MenuToggleItem>(label, value, visible);
}

MenuToggleItem* Menu::AddToggleItem(const char *label, bool &value, const unsigned char *icon, bool visible) {
  return this->addItem<MenuToggleItem>(label, value, icon, visible);
}

MenuToggleItem* Menu::AddToggleItem(const char *label, OnSaveFunction onSave, bool &value, bool visible) {
  return this->addItem<MenuToggleItem>(label, value, onSave, visible);
}

MenuToggleItem* Menu::AddToggleItem(const char *label, OnSaveFunction onSave, bool &value, const unsigned char *icon,
                         bool visible) {
  return this->addItem<MenuToggleItem>(label, value, onSave, icon, visible);
}

MenuEnumItem* Menu::AddEnumItem(const char *label, const char *title, const char* const* options, uint8_t numOptions, uint8_t &value, bool visible) {
  return this->addItem<MenuEnumItem>(label, title, options, numOptions, value, visible);
}

MenuEnumItem* Menu::AddEnumItem(const char *label, const char *title, const char* const* options, uint8_t numOptions, uint8_t &value, const unsigned char *icon, bool visible) {
  return this->addItem<MenuEnumItem>(label, title, options, numOptions, value, icon, visible);
}

MenuEnumItem* Menu::AddEnumItem(const char *label, const char *title, const char* const* options, uint8_t numOptions, uint8_t &value, OnSaveFunction onSave, bool visible) {
  return this->addItem<MenuEnumItem>(label, title, options, numOptions, value, onSave, visible);
}

MenuEnumItem* Menu::AddEnumItem(const char *label, const char *title, const char* const* options, uint8_t numOptions, uint8_t &value, OnSaveFunction onSave, const unsigned char *icon, bool visible) {
  return this->addItem<MenuEnumItem>(label, title, options, numOptions, value, onSave, icon, visible);
}

MenuInfoItem* Menu::AddInfoItem(const char *label, const std::vector<MenuInfoEntry>& entries, bool visible) {
  return this->addItem<MenuInfoItem>(label, entries, visible);
}

MenuInfoItem* Menu::AddInfoItem(const char *label, const std::vector<MenuInfoEntry>& entries, const unsigned char *icon, bool visible) {
  return this->addItem<MenuInfoItem>(label, entries, icon, visible);
}

MenuSubMenu* Menu::AddSubMenu(const char *label, Menu &menu, bool visible) {
  return this->addItem<MenuSubMenu>(label, menu, visible);
}

MenuSubMenu* Menu::AddSubMenu(const char *label, Menu &menu, const unsigned char *icon, bool visible) {
  return this->addItem<MenuSubMenu>(label, icon, menu, visible);
}

void Menu::SetEventHandler(std::function<void()> function) {
  this->handler = std::move(function);
}

void Menu::Event(EventType type, EventState state) {
  //LOGF(DEBUG, "Event type: %d state: %d Active: %d", type, state, this->visibleItems[this->activeItem]->GetType());

  if (this->timeoutActive) {
    this->lastInteraction = millis();
  }

  if (this->isConfirmDialogOpen) {
    if (type == EventType::EVENT_UP || type == EventType::EVENT_DOWN) {
      if (state == EventState::STATE_DOWN) {
        this->confirmDialogSelection = (this->confirmDialogSelection == 0) ? 1 : 0;
      }
    } else if (type == EventType::EVENT_ENTER) {
      if (state == EventState::STATE_UP && this->enterState == EventState::STATE_DOWN) {
        this->isConfirmDialogOpen = false;
        if (this->confirmDialogSelection == 1 && this->pendingConfirmAction) {
          this->pendingConfirmAction();
        } else if (this->confirmDialogSelection == 0) {
          this->visibleItems[this->activeItem]->Abort();
        }
        this->enterState = EventState::STATE_UP;
      } else if (state == EventState::STATE_DOWN) {
        this->enterState = state;
      }
    }
    return;
  }

  if (type == EventType::EVENT_UP || type == EventType::EVENT_DOWN) {
      bool isSubMenu = this->visibleItems[this->activeItem]->GetType() == ITEM_SUBMENU;
      bool isActive = this->visibleItems[this->activeItem]->ScreenActive();
      bool isInfoItem = this->visibleItems[this->activeItem]->GetType() == ITEM_INFO;
      
      bool currentInvertInput = Menu::invertInputPtr != nullptr ? *(Menu::invertInputPtr) : Menu::invertInput;
      bool currentInvertScroll = Menu::invertScrollPtr != nullptr ? *(Menu::invertScrollPtr) : Menu::invertScroll;
      
      // Invert input if we are editing an input item (not a submenu)
      if (isActive && !isSubMenu && !isInfoItem && currentInvertInput) {
          type = (type == EventType::EVENT_UP) ? EventType::EVENT_DOWN : EventType::EVENT_UP;
      } 
      // Invert scroll if we are navigating the menu (not editing an item)
      else if ((!isActive || isInfoItem) && currentInvertScroll) {
          type = (type == EventType::EVENT_UP) ? EventType::EVENT_DOWN : EventType::EVENT_UP;
      }
  }

  if (this->visibleItems[this->activeItem]->GetType() == ITEM_SUBMENU &&
      this->visibleItems[this->activeItem]->ScreenActive()) {
    this->visibleItems[this->activeItem]->Event(type, state);
    return;
  }

  switch (type) {
    case EventType::EVENT_ENTER:
      if (state == EventState::STATE_DOWN) {
        this->enterState = state;
      } else if (state == STATE_UP) {
        //LOG(DEBUG, "Processing Enter Up Click");
        if (this->enterState == EventState::STATE_DOWN) {
          if (this->IsOpen()) {
            //LOG(DEBUG, "Menu already open");
            if (this->visibleItems[this->activeItem]->ScreenActive()) {
              this->visibleItems[this->activeItem]->Enter();
            } else {
              this->visibleItems[this->activeItem]->Action();
            }
          } else {
            //LOG(DEBUG, "Menu opening");
            this->Open();
          }
        }
        this->enterState = EventState::STATE_UP;
      } else if (state == EventState::STATE_HELD) {
        long abortTime = millis();
        if (abortTime - this->lastMenuAbort > this->abortInterval) {
          if (this->enterState == EventState::STATE_DOWN) {
            if (this->visibleItems[this->activeItem]->ScreenActive()) {
              //LOG(DEBUG, "Processing Enter button held, active screen aborting.");
              this->visibleItems[this->activeItem]->Abort();
            } else {
              //LOG(DEBUG, "Processing Enter button held, aborting.");
              lastMenuAbort = abortTime;
              //LOG(DEBUG, "Close menu");
              this->Close();
            }
          }
        }
        this->enterState = EventState::STATE_HELD;
      }
      break;
    case EventType::EVENT_UP:
      if (state == EventState::STATE_DOWN) {
        //LOG(DEBUG, "Processing Up Click");
        this->buttonHold = type;
        this->buttonHoldStartTime = millis();
        if (this->visibleItems[this->activeItem]->ScreenActive()) {
          this->visibleItems[this->activeItem]->Up();
        } else {
          if (this->activeItem > 0) {
            this->activeItem--;
            if (this->activeItem < this->offset) {
              this->offset = this->activeItem;
            }
          } else {
            this->activeItem = this->visibleItemCount - 1;
            this->offset = std::max(0, this->visibleItemCount - 3);
          }
        }
      } else if (state == EventState::STATE_UP) {
        this->buttonHold = EventType::EVENT_NONE;
        this->buttonHoldStartTime = 0;
      }
      break;
    case EventType::EVENT_DOWN:
      if (state == EventState::STATE_DOWN) {
        //LOG(DEBUG, "Processing Down Click");
        this->buttonHold = EventType::EVENT_DOWN;
        this->buttonHoldStartTime = millis();
        if (this->visibleItems[this->activeItem]->ScreenActive()) {
          this->visibleItems[this->activeItem]->Down();
        } else {
          if (this->activeItem < this->visibleItemCount - 1) {
            this->activeItem++;
            if (this->activeItem >= this->offset + 3) {
              this->offset = this->activeItem - 2;
            }
          } else {
            this->activeItem = 0;
            this->offset = 0;
          }
        }
      } else if (state == EventState::STATE_UP) {
        this->buttonHold = EventType::EVENT_NONE;
        this->buttonHoldStartTime = 0;
      }
      break;
  }
}

void Menu::EventHandler() {
  if (this->handler) {
    this->handler();
  }
}

MenuBackItem* Menu::AddBackItem(const char *label) {
  return this->addItem<MenuBackItem>(label, this);

}

MenuBackItem* Menu::AddBackItem(const char *label, const unsigned char *icon) {
  return this->addItem<MenuBackItem>(label, icon, this);

}

void Menu::checkVisibleItems() {
  this->visibleItems.clear();
  this->visibleItemCount = 0;
  for (int i = 0; i < this->itemCount; ++i) {
    if (this->items[i]->IsVisible()) {
      this->visibleItems.push_back(this->items[i]);
      this->visibleItemCount += 1;
    }
  }
}

void Menu::TimeoutActive(bool status) {
  this->timeoutActive = status;
}

void Menu::SetTimeout(double timeoutSec) {
  this->timeoutLimit = timeoutSec * 1000;
}

double Menu::GetTimeout() const {
  return this->timeoutLimit / 1000;
}
