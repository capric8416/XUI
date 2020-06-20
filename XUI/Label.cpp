// self
#include "Label.h"

// project
#include "Common.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"

// namespace
using namespace std;



Label::Label(
    wstring ID,
    RECT Position,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    bool Hidden,
    bool Disabled,
    bool Hintable,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Image*> ImageStyle,
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
        ImageStyle,
        AnimationStyle
    )
{
}


Label::~Label()
{
}
