/**
 * @file MenuInputItem.h
 * 
 * @brief Defines the MenuInputItem class for representing an input item in a menu.
*/

#pragma once

#include "Logger.h"
#include "MenuItem.h"

/**
 * @enum MenuInputItem
 * @brief Represents the type of menu item that renders a toggle right to the label.
*/

/**
 * @class MenuInputItem
 * @brief Represents an input item in a menu, allowing users to input and adjust values.
*/
class MenuInputItem : public MenuItem {
public:
    /**
     * @brief Constructs a MenuInputItem object with the specified parameters.
     *
     * @param labelInst The label of the menu item.
     * @param titleInst The title of the input item.
     * @param preInst The prefix for displaying the input value.
     * @param postInst The postfix for displaying the input value.
     * @param minInst The minimum value allowed for the input.
     * @param maxInst The maximum value allowed for the input.
     * @param valueInst Reference to the variable storing the input value.
     * @param onSaveInst The callback function invoked when the value is saved.
     * @param stepInst The step size for adjusting the input value.
     * @param stepFastInst The step size for fast adjustment of the input value.
     * @param isIntInst Flag indicating if the input value is an integer.
     * @param visibleInst Flag indicating if the menu item is initially visible.
    */
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, OnSaveFunction onSaveInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    /**
     * @brief Constructs a MenuInputItem object with the specified parameters including an icon.
     *
     * @param labelInst The label of the menu item.
     * @param titleInst The title of the input item.
     * @param preInst The prefix for displaying the input value.
     * @param postInst The postfix for displaying the input value.
     * @param minInst The minimum value allowed for the input.
     * @param maxInst The maximum value allowed for the input.
     * @param valueInst Reference to the variable storing the input value.
     * @param onSaveInst The callback function invoked when the value is saved.
     * @param iconInst The icon displayed for the input item.
     * @param stepInst The step size for adjusting the input value.
     * @param stepFastInst The step size for fast adjustment of the input value.
     * @param isIntInst Flag indicating if the input value is an integer.
     * @param visibleInst Flag indicating if the menu item is initially visible.
    */
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, OnSaveFunction onSaveInst, const unsigned char *iconInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    /**
     * @brief Constructs a MenuInputItem object with the specified parameters including an icon.
     *
     * @param labelInst The label of the menu item.
     * @param titleInst The title of the input item.
     * @param preInst The prefix for displaying the input value.
     * @param postInst The postfix for displaying the input value.
     * @param minInst The minimum value allowed for the input.
     * @param maxInst The maximum value allowed for the input.
     * @param valueInst Reference to the variable storing the input value.
     * @param stepInst The step size for adjusting the input value.
     * @param stepFastInst The step size for fast adjustment of the input value.
     * @param isIntInst Flag indicating if the input value is an integer.
     * @param visibleInst Flag indicating if the menu item is initially visible.
    */
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    /**
     * @brief Constructs a MenuInputItem object with the specified parameters including an icon.
     *
     * @param labelInst The label of the menu item.
     * @param titleInst The title of the input item.
     * @param preInst The prefix for displaying the input value.
     * @param postInst The postfix for displaying the input value.
     * @param minInst The minimum value allowed for the input.
     * @param maxInst The maximum value allowed for the input.
     * @param valueInst Reference to the variable storing the input value.
     * @param iconInst The icon displayed for the input item.
     * @param stepInst The step size for adjusting the input value.
     * @param stepFastInst The step size for fast adjustment of the input value.
     * @param isIntInst Flag indicating if the input value is an integer.
     * @param visibleInst Flag indicating if the menu item is initially visible.
    */
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, const unsigned char *iconInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    /**
     * @brief Draws the input item on the display.
     *
     * @param display Reference to the U8G2 display.
     * @param offsetY The vertical offset for drawing the item.
     * @param active Flag indicating if the item is active.
    */
    void Draw(U8G2 &display, int offsetY, bool active) override;

    /**
     * @brief Performs the action associated with the input item.
    */
    void Action() override;

    /**
     * @brief Handles the 'Up' action for adjusting the input value.
    */
    void Up() override;

    /**
     * @brief Handles the 'Fast Up' action for quickly adjusting the input value.
    */
    void UpFast() override;

    /**
     * @brief Handles the 'Down' action for adjusting the input value.
    */
    void Down() override;

    /**
     * @brief Handles the 'Fast Down' action for quickly adjusting the input value.
    */
    void DownFast() override;

    /**
     * @brief Handles the 'Enter' action for confirming the input value.
    */
    void Enter() override;

    /**
     * @brief Handles the 'Abort' action for cancelling input.
    */
    void Abort() override;

    /**
     * @brief Gets the type of the menu item.
     *
     * @return The type of the menu item.
    */
    MenuType GetType() override { return ITEM_INPUT; };

private:
    double &value; ///< Reference to the variable storing the input value.
    double workingVal; ///< Temporary storage for the input value during editing.
    const char *title; ///< The title of the input item.
    const char *pre; ///< The prefix for displaying the input value.
    const char *post; ///< The postfix for displaying the input value.
    float min; ///< The minimum value allowed for the input.
    float max; ///< The maximum value allowed for the input.
    float step; ///< The step size for adjusting the input value.
    float stepFast; ///< The step size for fast adjustment of the input value.
    bool editing = false; ///< Flag indicating if the input is being edited.
    bool isInt = false; ///< Flag indicating if the input value is an integer.

    /**
     * @brief Draws the input screen for editing the input value.
     *
     * @param display Reference to the U8G2 display.
    */
    void drawInputScreen(U8G2 &display);

    /**
     * @brief Initializes the input item.
    */
    void init();
}; 