// self
#include "Button.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"

// namespace
using namespace std;



Button::Button(
    wstring ID,
    RECT Position,
    RECT Margin,
    RECT Padding,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    bool Hidden,
    bool Disabled,
    bool Hintable,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Animation*> AnimationStyle
) :
    Control(
        ID,
        Position,
        Hidden,
        Disabled,
        HorizontalAlign,
        VerticalAlign,
        Margin,
        Padding,
        Hintable,
        {},
        BackgroundStyle,
        BorderStyle,
        TextStyle,
        AnimationStyle
    )
{
}


Button::~Button()
{
}


void Button::OnLeftButtonDown(LONG X, LONG Y)
{
    Style(CONTROL_STATUS_DOWN);
    Invalidate();
}


void Button::OnLeftButtonUp(LONG X, LONG Y)
{
    Style(CONTROL_STATUS_NORMAL);
    Invalidate();
}


void Button::OnMouseEnter(LONG X, LONG Y)
{
    Style(CONTROL_STATUS_HOVER);
    Invalidate();
}


void Button::OnMouseLeave(LONG X, LONG Y)
{
    Style(CONTROL_STATUS_NORMAL);
    Invalidate();
}
