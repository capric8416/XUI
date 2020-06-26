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


void RadioBox::Check(bool Lazy)
{
    Style(CONTROL_STATUS_CHECKED);

    m_Checked = true;

    if (m_Grouped)
    {
        ((RadioGroup*)m_Parent)->OnChildChecked(this, true, Lazy);
    }
    else
    {
        m_Wnd->GetUI()->OnGroupItemChanged(this, this, this);
    }

    if (!Lazy)
    {
        Invalidate();
    }
}


void RadioBox::Uncheck(bool Lazy)
{
    Style(CONTROL_STATUS_NORMAL);

    m_Checked = false;

    if (!Lazy)
    {
        Invalidate();
    }
}


bool RadioBox::Checked()
{
    return m_Checked;
}


void RadioBox::OnMouseEnter(LONG X, LONG Y)
{
    __super::OnMouseEnter(X, Y);
    Style(CONTROL_STATUS_HOVER);
    Invalidate();
}


void RadioBox::OnMouseLeave(LONG X, LONG Y)
{
    __super::OnMouseLeave(X, Y);
    Style(m_Checked ? CONTROL_STATUS_CHECKED : CONTROL_STATUS_NORMAL);
    Invalidate();
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
    m_Pinned(nullptr),
    m_Mutex()
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
    m_Pinned(nullptr),
    m_Mutex()
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


void RadioGroup::Attach(RadioBox* Child, bool Checked, bool Pin, bool Paint, bool Lazy)
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

    if (!Lazy)
    {
        Resize();
    }

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

            auto bottom = (index + 1) * height;
            m_VerticalScroolEnabled = bottom > DENOMINATOR;

            child->SetVisibility(bottom <= DENOMINATOR, false);
            child->SetPosition(0, index * height, DENOMINATOR, bottom);
        }
        else
        {
            if (width == 0)
            {
                width = child->WidthPercentage();
            }

            auto right = (index + 1) * width;
            m_HorizontalScroolEnabled = right > DENOMINATOR;

            child->SetVisibility(right <= DENOMINATOR, false);
            child->SetPosition(index * width, 0, (index + 1) * width, DENOMINATOR);
        }

        index++;
    }

    __super::Resize();
}


void RadioGroup::OnChildChecked(Control* Child, bool Notify, bool Lazy)
{
    if (m_Checked != nullptr && Child != m_Checked)
    {
        m_Checked->Uncheck(Lazy);
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
        m_Checked->Check(Lazy);
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


bool RadioGroup::Check(size_t Pos, bool Lazy)
{
    if (0 <= Pos && Pos < Size())
    {
        OnChildChecked(m_Children[Pos], false, Lazy);
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


void RadioGroup::OnMouseVerticalWheel(LONG X, LONG Y, WPARAM wParam)
{
    lock_guard<mutex> lock(m_Mutex);

    auto offset = X != 0 && Y != 0 ? (short)HIWORD(wParam) : (short)wParam;
    if (offset == 0 || m_Children.size() == 0)
    {
        return;
    }

    auto firstChild = m_Children[0];
    auto height = firstChild->Height();
    auto heightPercent = firstChild->HeightPercentage();
    auto maxHeight = Height();
    auto maxVisiable = maxHeight / height;

    LONG percentOffset;
    if (offset == MIN_SHORT)
    {
        auto topPos = firstChild->PositionPercentage();
        percentOffset = -topPos.top;
    }
    else if (offset == MAX_SHORT)
    {
        auto bottomPos = m_Children[m_Children.size() - 1]->PositionPercentage();
        percentOffset = DENOMINATOR - bottomPos.bottom;
    }
    else
    {
        auto absOffset = offset > 0 ? offset : -offset;
        if (absOffset < height)
        {
            absOffset = height;
        }
        else
        {
            if (absOffset < maxHeight)
            {
                absOffset = absOffset / height * height;
            }
            else
            {
                absOffset = (maxHeight - height) / height * height;
            }
        }
        offset = offset > 0 ? absOffset : -absOffset;

        if (offset < 0)
        {
            auto topSentinel = m_Children[m_Children.size() - maxVisiable]->PositionPercentage();
            if (topSentinel.top <= 0)
            {
                return;
            }
        }
        else
        {
            auto bottomSentinel = m_Children[maxVisiable - 1]->PositionPercentage();
            if (bottomSentinel.bottom >= maxVisiable * heightPercent)
            {
                return;
            }
        }

        percentOffset = offset / height * heightPercent;
    }

    UINT16 i = 0;
    for (const auto& child : m_Children)
    {
        child->VerticalMovePosition(percentOffset, i++);
    }

    Invalidate();
}


void RadioGroup::OnMouseHorizontalWheel(LONG X, LONG Y, WPARAM wParam)
{
    lock_guard<mutex> lock(m_Mutex);

    auto offset = X != 0 && Y != 0 ? (short)HIWORD(wParam) : (short)wParam;
    if (offset == 0 || m_Children.size() == 0)
    {
        return;
    }

    auto firstChild = m_Children[0];
    auto width = firstChild->Width();
    auto widthPercent = firstChild->WidthPercentage();
    auto maxWidth = Width();
    auto maxVisiable = maxWidth / width;

    LONG percentOffset;
    if (offset == MIN_SHORT)
    {
        auto leftPos = firstChild->PositionPercentage();
        percentOffset = -leftPos.left;
    }
    else if (offset == MAX_SHORT)
    {
        auto rightPos = m_Children[m_Children.size() - 1]->PositionPercentage();
        percentOffset = DENOMINATOR - rightPos.right;
    }
    else
    {
        auto absOffset = offset > 0 ? offset : -offset;
        if (absOffset < width)
        {
            absOffset = width;
        }
        else
        {
            if (absOffset < maxWidth)
            {
                absOffset = absOffset / width * width;
            }
            else
            {
                absOffset = (maxWidth - width) / width * width;
            }
        }
        offset = offset > 0 ? absOffset : -absOffset;

        if (offset < 0)
        {
            auto leftSentinel = m_Children[m_Children.size() - maxVisiable]->PositionPercentage();
            if (leftSentinel.left <= 0)
            {
                return;
            }
        }
        else
        {
            auto rightSentinel = m_Children[maxVisiable - 1]->PositionPercentage();
            if (rightSentinel.right >= maxVisiable * widthPercent)
            {
                return;
            }
        }

        percentOffset = offset / width * widthPercent;
    }

    UINT16 i = 0;
    for (const auto& child : m_Children)
    {
        child->HorizontalMovePosition(percentOffset, i++);
    }

    Invalidate();
}
