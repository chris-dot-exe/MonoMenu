#pragma once

enum MenuType {
    ITEM_NONE,
    ITEM_INPUT,
    ITEM_TOGGLE,
    ITEM_SUBMENU,
    ITEM_ENUM,
    ITEM_INFO
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

/**
 * @struct MenuInfoEntry
 * @brief Represents a single label/value pair in a MenuInfoItem.
 */
struct MenuInfoEntry {
    const char* label;
    const char** value; ///< Pointer to a const char* that holds the dynamic value string.
};

using OnSaveFunction = std::function<void()>;