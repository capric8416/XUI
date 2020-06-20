#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "RadioBox.h"
#include "CheckBox.h"

// c/c++
#include <map>


#define TAB_NAV Control
#define TAB_BOX Control
#define TAB_RADIO_NAV RadioGroup
#define TAB_CHECK_NAV CheckGroup


class TabGroup : public Control
{
public:
    // Constructor
    TabGroup(
        std::wstring ID,
        RECT Position,
        TAB_NAV* Nav,
        TAB_BOX* Box,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    TabGroup(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hintable = true,
        TAB_NAV* Nav = nullptr,
        TAB_BOX* Box = nullptr,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );

    virtual ~TabGroup();

    virtual void Navigate(Control* Parent, Control* Last, Control* Current, LONG Value = -1);

protected:
    void Bind(TAB_NAV* Nav, TAB_BOX* Box);

    std::map<Control*, Control*> m_Binding;
};
