
#pragma once

#include "Logger.h"
#include "MenuItem.h"

/**
     * @enum MenuInputItem
     * @brief Type of menu item that renders a toggle right to the label
    */

class MenuInputItem : public MenuItem {
public:
/**
     * @brief Constructor
     *
     * @param label Label of the menu item
     * @param value Value of the toggle
     * @param icon Icon shown in front of the menu item
     * @param visible Visibility of the menu item

    */
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, std::function<void()> onSaveInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, std::function<void()> onSaveInst, const unsigned char *iconInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);
    MenuInputItem(const char *labelInst, const char *titleInst, const char *preInst, const char *postInst, float  minInst, float  maxInst,
                  double  &valueInst, const unsigned char *iconInst, float stepInst=0.1, float stepFastInst=0.5, bool isIntInst=false, bool visibleInst = true);

    void Draw(U8G2 &display, int offsetY, bool active) override;
    void Action() override;
    void Up() override;
    void UpFast() override;
    void Down() override;
    void DownFast() override;
    void Enter() override;
    void Abort() override;
    MenuType GetType() override {return ITEM_INPUT; };
private:
    double &value;
    double workingVal;
    const char *title;
    const char *pre;
    const char *post;
    float min;
    float max;
    float step;
    float stepFast;
    bool editing = false;
    bool isInt = false;

    void drawInputScreen(U8G2 &display);
    void init();

};