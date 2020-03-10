#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"



class Label : public Control
{
public:
    // Constructor
    Label(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = false,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    ~Label();
};