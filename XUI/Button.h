#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"




class Button : public Control
{
public:
    // Constructor
    Button(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    ~Button();

    // Handle left button down event
    virtual void OnLeftButtonDown(LONG X, LONG Y);

    // Handle left button up event
    virtual void OnLeftButtonUp(LONG X, LONG Y);

    // Handle mouse enter event
    virtual void OnMouseEnter(LONG X, LONG Y);

    // Handle mouse leave event
    virtual void OnMouseLeave(LONG X, LONG Y);
};

