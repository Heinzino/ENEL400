#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "buttonTask.hpp"
#include <lvgl.h>
#include "UI/ui.h"
#include "UI/lv_conf.h"
#include "UI/ui_helpers.h"

class Screen
{
public:
    virtual void updateScreen() = 0;             // Pure virtual function
    virtual void handleButton(ButtonID btn) = 0; 
    virtual ~Screen() = default;                 // Virtual destructor for polymorphic cleanup
};

#endif // SCREEN_HPP
