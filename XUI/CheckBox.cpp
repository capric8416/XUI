// self
#include "CheckBox.h"

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



CheckBox::CheckBox(
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


CheckBox::~CheckBox()
{
}


void CheckBox::SetGrouped()
{
    m_Grouped = true;
}


void CheckBox::SetChecked(bool Status, bool Notify)
{
    m_Checked = Status;
    Style(m_Checked ? CONTROL_STATUS_CHECKED : CONTROL_STATUS_NORMAL);
    ((CheckGroup*)m_Parent)->OnChildChecked(this, m_Checked, Notify);
}


bool CheckBox::Checked()
{
    return m_Checked;
}


void CheckBox::OnLeftButtonClick(LONG X, LONG Y)
{
    __super::OnLeftButtonClick(X, Y);

    m_Checked = !m_Checked;
    Style(m_Checked ? CONTROL_STATUS_CHECKED : CONTROL_STATUS_NORMAL);

    if (m_Grouped)
    {
        ((CheckGroup*)m_Parent)->OnChildChecked(this, m_Checked);
    }
    else
    {
        m_Wnd->GetUI()->OnGroupItemChanged(this, this, this, m_Checked);
    }

    Invalidate();
}


CheckGroup::CheckGroup(
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
    m_Pinned(nullptr),
    m_LastCheck(nullptr),
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

    if (m_Children.size() > 0)
    {
        for (const auto& child : m_Children)
        {
            auto t = (CheckBox*)child;
            t->SetGrouped();
            if (t->Checked())
            {
                m_Checked.insert(t);
            }
        }
    }
}


CheckGroup::CheckGroup(
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
    m_Pinned(nullptr),
    m_LastCheck(nullptr),
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

    if (m_Children.size() > 0)
    {
        for (const auto& child : m_Children)
        {
            auto t = (CheckBox*)child;
            t->SetGrouped();
            if (t->Checked())
            {
                m_Checked.insert(t);
            }
        }
    }
}


CheckGroup::~CheckGroup()
{
}


void CheckGroup::Attach(CheckBox* Child, bool Checked, bool Pin, bool Paint)
{
    __super::Attach(Child);

    Child->SetGrouped();
    if (Checked)
    {
        m_Checked.insert(Child);
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


void CheckGroup::Dettach(CheckBox* Child)
{
    if (m_Pinned == Child)
    {
        m_Pinned = nullptr;
    }

    auto iter = m_Checked.begin();
    while (iter != m_Checked.end())
    {
        auto target = *iter;
        if (target == Child)
        {
            m_Checked.erase(iter);
            break;
        }

        iter++;
    }

    auto iter1 = m_Children.begin();
    while (iter1 != m_Children.end())
    {
        auto target = *iter1;
        if (target != Child)
        {
            iter1++;
            continue;
        }

        XSafeDelete(target);

        m_Children.erase(iter1);

        Resize();

        Invalidate();

        break;
    }
}


void CheckGroup::Resize()
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
            child->SetPosition(0, index* height, DENOMINATOR, bottom);
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
            child->SetPosition(index* width, 0, (index + 1)* width, DENOMINATOR);
        }

        index++;
    }

    __super::Resize();
}


void CheckGroup::OnChildChecked(CheckBox* Child, bool Checked, bool Notify)
{
    bool changed = false;
    auto last = m_Checked.size();

    if (Checked)
    {
        m_Checked.insert(Child);
        changed = m_Checked.size() > last;
    }
    else
    {
        m_Checked.erase(Child);
        changed = m_Checked.size() < last;
    }

    if (Notify && changed)
    {
        if (strcmp(m_Parent->TypeName(), typeid(TabGroup).name()) == 0)
        {
            ((TabGroup*)m_Parent)->Navigate(this, m_LastCheck, Child, Checked);
        }
        else
        {
            m_Wnd->GetUI()->OnGroupItemChanged(this, m_LastCheck, Child, Checked);
        }
    }

    m_LastCheck = Child;
}


set<CheckBox*> CheckGroup::Checked()
{
    return m_Checked;
}


void CheckGroup::Pin(wstring ID)
{
    for (const auto& child : m_Children)
    {
        if (child->ID() == ID)
        {
            m_Pinned = (CheckBox*)child;
        }
    }
}


void CheckGroup::Unpin(wstring ID)
{
    for (const auto& child : m_Children)
    {
        if (child->ID() == ID)
        {
            m_Pinned = nullptr;
        }
    }
}


CheckBox* CheckGroup::Pinned()
{
    return m_Pinned;
}


void CheckGroup::OnMouseVerticalWheel(LONG X, LONG Y, WPARAM wParam)
{
    lock_guard<mutex> lock(m_Mutex);

    auto offset = (short)HIWORD(wParam);
    if (offset == 0 || m_Children.size() == 0)
    {
        return;
    }

    auto firstChild = m_Children[0];
    auto height = firstChild->Height();
    auto heightPercent = firstChild->HeightPercentage();
    auto maxHeight = Height();
    auto maxVisiable = maxHeight / height;

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

    auto dpiOffset = (float)offset / m_Wnd->DpiScaleY();
    auto percentOffset = offset / height * heightPercent;

    for (const auto& child : m_Children)
    {
        child->VerticalMovePosition(offset, dpiOffset, percentOffset);
    }

    Invalidate();
}


void CheckGroup::OnMouseHorizontalWheel(LONG X, LONG Y, WPARAM wParam)
{
    lock_guard<mutex> lock(m_Mutex);

    auto offset = (short)HIWORD(wParam);
    if (offset == 0 || m_Children.size() == 0)
    {
        return;
    }

    auto firstChild = m_Children[0];
    auto width = firstChild->Width();
    auto widthPercent = firstChild->WidthPercentage();
    auto maxWidth = Width();
    auto maxVisiable = maxWidth / width;

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

    auto dpiOffset = (float)offset / m_Wnd->DpiScaleX();
    auto percentOffset = offset / width * widthPercent;

    for (const auto& child : m_Children)
    {
        child->HorizontalMovePosition(offset, dpiOffset, percentOffset);
    }

    Invalidate();
}
