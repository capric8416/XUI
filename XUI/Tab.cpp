// self
#include "Tab.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"


// namespace
using namespace std;


TabGroup::TabGroup(
    wstring ID,
    RECT Position,
    TAB_NAV* Nav,
    TAB_BOX* Box,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Animation*> AnimationStyle
) :
    Control(
        ID,
        Position,
        false,
        false,
        CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN_VERTICAL_CENTER,
        ZERO_RECT,
        ZERO_RECT,
        true,
        {},
        BackgroundStyle,
        BorderStyle,
        TextStyle,
        AnimationStyle
    )
{
    SaveStyleValues(BackgroundStyle, BorderStyle, TextStyle, AnimationStyle);

    Bind(Nav, Box);

    for (const auto& child : m_Children)
    {
        child->SetParent(this);
        if (child->VerticalScrollEnabled())
        {
            m_VerticalScroolEnabled = true;
        }
        if (child->HorizontalScrollEnabled())
        {
            m_HorizontalScroolEnabled = true;
        }
    }
}


TabGroup::TabGroup(
    wstring ID,
    RECT Position,
    bool Hidden,
    bool Disabled,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    bool Hintable,
    TAB_NAV* Nav,
    TAB_BOX* Box,
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
    SaveStyleValues(BackgroundStyle, BorderStyle, TextStyle, AnimationStyle);

    Bind(Nav, Box);

    for (const auto& child : m_Children)
    {
        child->SetParent(this);
        if (child->VerticalScrollEnabled())
        {
            m_VerticalScroolEnabled = true;
        }
        if (child->HorizontalScrollEnabled())
        {
            m_HorizontalScroolEnabled = true;
        }
    }
}


void TabGroup::Navigate(Control* Parent, Control* Last, Control* Current, LONG Value)
{
    if (Value == -1)
    {
        m_Binding[Last]->Hide();
        m_Binding[Current]->Show();
    }
    else
    {
        if (Value == 0 && Last == Current)
        {
            m_Binding[Current]->Hide();
        }
        else
        {
            if (Last != nullptr)
            {
                m_Binding[Last]->Hide();
            }
            m_Binding[Current]->Show();
        }
    }

    m_Wnd->GetUI()->OnGroupItemChanged(Parent, Last, Current, Value);
}


void TabGroup::Bind(TAB_NAV* Nav, TAB_BOX* Box)
{
    m_Children.push_back(Nav);
    m_Children.push_back(Box);

    auto keys = Nav->Children();
    auto values = Box->Children();

    auto i = keys->begin();
    auto j = values->begin();

    for (size_t k = 0; k < keys->size(); k++)
    {
        if ((*i)->Hintable())
        {
            m_Binding[*i] = *j;
            j++;
        }

        i++;
    }
}
