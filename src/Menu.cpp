#include "Menu.h"
#include "items/MenuSubMenu.h"
#include "items/MenuInputItem.h"
#include "items/MenuToggleItem.h"
#include "items/MenuBackItem.h"
#include <typeinfo>
Menu::Menu(U8G2 &displayInst) : display(displayInst) {

}

void Menu::Open() {
  this->isOpen = true;
}

bool Menu::IsOpen() {
  return this->isOpen;
}

void Menu::Close() {
  this->offset = 0;
  this->activeItem = 0;
  this->isOpen = false;
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
    this->display.firstPage();
    this->checkVisibleItems();
    do {
      uint8_t index = 0;
      uint8_t renderedCount = 0;
      if (this->visibleItems[this->activeItem]->ScreenActive()) {
        this->visibleItems[this->activeItem]->Draw(this->display, this->yOffset[index], true);
        continue;
      }

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
void Menu::addItem(Args &&... args) {
  if (this->itemCount >= this->capacity) {
    return;
  }

  auto newItem = std::make_shared<T>(std::forward<Args>(args)...);
  this->items.push_back(newItem);
  this->itemCount += 1;

  if (newItem->IsVisible()) {
    this->visibleItems.push_back(newItem);
    this->visibleItemCount += 1;
  }

  //LOGF(DEBUG, "Added Item");
}

void Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        double &value, float step, float stepFast, bool isInt, bool visible) {

  this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, step, stepFast, isInt, visible);

}

void Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        double &value, const unsigned char *icon, float step, float stepFast, bool isInt,
                        bool visible) {

  this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, icon, step, stepFast, isInt, visible);

}

void Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        OnSaveFunction onSave, double &value, const unsigned char *icon,
                        float step, float stepFast, bool isInt, bool visible) {

  this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, onSave, icon, step, stepFast, isInt, visible);

}

void Menu::AddInputItem(const char *label, const char *title, const char *pre, const char *post, float min, float max,
                        OnSaveFunction onSave, double &value, float step, float stepFast, bool isInt,
                        bool visible) {

  this->addItem<MenuInputItem>(label, title, pre, post, min, max,
                               value, onSave, step, stepFast, isInt, visible);

}

void Menu::AddToggleItem(const char *label, bool &value, bool visible) {
  this->addItem<MenuToggleItem>(label, value, visible);
}

void Menu::AddToggleItem(const char *label, bool &value, const unsigned char *icon, bool visible) {
  this->addItem<MenuToggleItem>(label, value, icon, visible);
}

void Menu::AddToggleItem(const char *label, OnSaveFunction onSave, bool &value, bool visible) {
  this->addItem<MenuToggleItem>(label, value, onSave, visible);
}

void Menu::AddToggleItem(const char *label, OnSaveFunction onSave, bool &value, const unsigned char *icon,
                         bool visible) {
  this->addItem<MenuToggleItem>(label, value, onSave, icon, visible);
}

void Menu::AddSubMenu(const char *label, Menu &menu, bool visible) {
  this->addItem<MenuSubMenu>(label, menu, visible);
}

void Menu::AddSubMenu(const char *label, Menu &menu, const unsigned char *icon, bool visible) {
  this->addItem<MenuSubMenu>(label, icon, menu, visible);
}

void Menu::SetEventHandler(std::function<void()> function) {
  this->handler = std::move(function);
}

void Menu::Event(EventType type, EventState state) {
  //LOGF(DEBUG, "Event type: %d state: %d Active: %d", type, state, this->visibleItems[this->activeItem]->GetType());

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

void Menu::AddBackItem(const char *label) {
  this->addItem<MenuBackItem>(label, this);

}

void Menu::AddBackItem(const char *label, const unsigned char *icon) {
  this->addItem<MenuBackItem>(label, icon, this);

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
