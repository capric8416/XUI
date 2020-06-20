#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"


class SingleLineEdit : public Control
{
public:
    // Constructor
    SingleLineEdit(
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
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    virtual ~SingleLineEdit();

    virtual void OnPaint(bool PaintChildren = false);

    virtual void OnCharInput(WPARAM wParam, LPARAM lParam);
    virtual void OnKeyInput(WPARAM wParam, LPARAM lParam);

};