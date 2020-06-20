// self
#include "RadioBox.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"
#include "Tab.h"

// c/c++
#include <typeinfo>

// namespace
using namespace std;




RadioBox::RadioBox(
    wstring ID,
    bool Checked,
    RECT Position,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    LONG* User,
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
    ),
    m_Checked(Checked),
    m_User(User),
    m_Grouped(false)
{
    if (m_Checked)
    {
        Style(CONTROL_STATUS_CHECKED);
    }
}


RadioBox::~RadioBox()
{
}


void RadioBox::SetGrouped()
{
    m_Grouped = true;
}


void RadioBox::SetChecked(bool Status, bool Notify)
{
    m_Checked = Status;
    Style(m_Checked ? CONTROL_STATUS_CHECKED : CONTROL_STATUS_NORMAL);
    ((RadioGroup*)m_Parent)->OnChildChecked(this, Notify);
}


size_t RadioBox::GetPos()
{
    return m_Pos;
}


void RadioBox::SetPos(size_t Value)
{
    m_Pos = Value;
}


void RadioBox::OnLeftButtonClick(LONG X, LONG Y)
{
    __super::OnLeftButtonClick(X, Y);

    Check();
}


void RadioBox::Check()
{
    Style(CONTROL_STATUS_CHECKED);

    if (m_Grouped)
    {
        ((RadioGroup*)m_Parent)->OnChildChecked(this);
    }
    else
    {
        m_Wnd->GetUI()->OnGroupItemChanged(this, this, this);
    }

    Invalidate();
}


void RadioBox::Uncheck()
{
    Style(CONTROL_STATUS_NORMAL);

    Invalidate();
}


bool RadioBox::Checked()
{
    return m_Checked;
}



RadioGroup::RadioGroup(
    wstring ID,
    RECT Position,
    bool Vertical,
    bool Scroll,
    initializer_list<Control*> Children,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Image*> ImageStyle,
    initializer_list<Animation*> AnimationStyle
) :
    Control(
        ID,
        Position,
        Children,
        BackgroundStyle,
        BorderStyle,
        TextStyle,
        ImageStyle,
        AnimationStyle
    ),
    m_Vertical(Vertical),
    m_Checked(nullptr),
    m_Pinned(nullptr)
{
    if (Vertical)
    {
        m_VerticalScroolEnabled = Scroll;
    }
    else
    {
        m_HorizontalScroolEnabled = Scroll;
    }

    for (const auto& child : m_Children)
    {
        auto t = (RadioBox*)child;
        t->SetGrouped();
        if (t->Checked())
        {
            m_Checked = t;
        }
    }
}


RadioGroup::RadioGroup(
    wstring ID,
    RECT Position,
    bool Vertical,
    bool Scroll,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    bool Hidden,
    bool Disabled,
    bool Hintable,
    initializer_list<Control*> Children,
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
        Children,
        BackgroundStyle,
        BorderStyle,
        TextStyle,
        ImageStyle,
        AnimationStyle
    ),
    m_Vertical(Vertical),
    m_Checked(nullptr),
    m_Pinned(nullptr)
{
    if (Vertical)
    {
        m_VerticalScroolEnabled = Scroll;
    }
    else
    {
        m_HorizontalScroolEnabled = Scroll;
    }

    for (const auto& child : m_Children)
    {
        auto t = (RadioBox*)child;
        t->SetGrouped();
        if (t->Checked())
        {
            m_Checked = t;
        }
    }
}


RadioGroup::~RadioGroup()
{
}


void RadioGroup::Attach(RadioBox* Child, bool Checked, bool Pin, bool Paint)
{
    __super::Attach(Child);

    Child->SetPos(Size() - 1);

    Child->SetGrouped();
    if (Checked)
    {
        m_Checked = Child;
        Child->SetChecked(true, false);
    }

    if (Pin)
    {
        m_Pinned = Child;
    }

    Resize();

    if (Paint)
    {
        Invalidate();
    }
}


void RadioGroup::Dettach(RadioBox* Child)
{
    if (m_Pinned == Child)
    {
        m_Pinned = nullptr;
    }

    if (m_Checked == Child)
    {
        m_Checked = nullptr;
    }

    size_t pos = 0;
    auto iter = m_Children.begin();
    while (iter != m_Children.end())
    {
        auto target = *iter;
        if (target != Child)
        {
            ((RadioBox*)target)->SetPos(pos);
            pos++;
            iter++;
        }
        else
        {
            XSafeDelete(target);
            iter = m_Children.erase(iter);
        }
    }

    Resize();

    Invalidate();
}


void RadioGroup::Resize()
{
    LONG width = 0;
    LONG height = 0;

    size_t index = 0;

    for (const auto& child : m_Children)
    {
        if (m_Vertical)
        {
            if (height == 0)
            {
                height = child->HeightPercentage();
            }

            child->SetPosition(0, index * height, DENOMINATOR, (index + 1) * height);
        }
        else
        {
            if (width == 0)
            {
                width = child->WidthPercentage();
            }

            child->SetPosition(index * width, 0, (index + 1) * width, DENOMINATOR);
        }

        index++;
    }

    __super::Resize();
}


void RadioGroup::OnChildChecked(Control* Child, bool Notify)
{
    if (m_Checked != nullptr && Child != m_Checked)
    {
        m_Checked->Uncheck();
    }

    if (m_Checked != Child && Notify)
    {
        if (strcmp(m_Parent->TypeName(), typeid(TabGroup).name()) == 0)
        {
            ((TabGroup*)m_Parent)->Navigate(this, m_Checked, Child);
        }
        else
        {
            m_Wnd->GetUI()->OnGroupItemChanged(this, m_Checked, Child);
        }
    }

    m_Checked = (RadioBox*)Child;
    if (!Notify)
    {
        m_Checked->Check();
    }
}


RadioBox* RadioGroup::Checked()
{
    return m_Checked;
}


void RadioGroup::Siblings(RadioBox* Current, RadioBox*& Previous, RadioBox*& Next)
{
    auto size = Size();
    auto pos = Current->GetPos();
    if (size == 1)
    {
        Previous = nullptr;
        Next = nullptr;
    }
    else
    {
        Previous = (RadioBox*)m_Children[pos > 0 ? pos - 1 : size - 1];
        Next = (RadioBox*)m_Children[pos < size - 1 ? pos + 1 : 0];
        if (size == 2)
        {
            if (pos == 0)
            {
                Previous = nullptr;
            }
            else
            {
                Next = nullptr;
            }
        }
    }
}


bool RadioGroup::Check(size_t Pos)
{
    if (0 <= Pos && Pos < Size())
    {
        OnChildChecked(m_Children[Pos], false);
        m_Checked->SetPos(Pos);
        return true;
    }

    return false;
}


void RadioGroup::Pin(wstring ID)
{
    for (const auto& child : m_Children)
    {
        if (child->ID() == ID)
        {
            m_Pinned = (RadioBox*)child;
        }
    }
}


void RadioGroup::Unpin(wstring ID)
{
    for (const auto& child : m_Children)
    {
        if (child->ID() == ID)
        {
            m_Pinned = nullptr;
        }
    }
}


RadioBox* RadioGroup::Pinned()
{
    return m_Pinned;
}
