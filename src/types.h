#pragma once

enum MenuType {
    ITEM_NONE,
    ITEM_INPUT,
    ITEM_TOGGLE,
    ITEM_SUBMENU
};

enum EventType {
    EVENT_NONE,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_ENTER
};
enum EventState {
    STATE_DOWN = 1,
    STATE_UP,
    STATE_HELD
};

using OnSaveFunction = std::function<void()>;