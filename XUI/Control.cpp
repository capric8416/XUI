// self
#include "Control.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "SliderBar.h"
#include "Animation.h"
#include "Background.h"
#include "Border.h"
#include "Image.h"
#include "Text.h"

// c/c++
#include <typeinfo>

// namespace
using namespace std;



Control::Control(
    wstring ID,
    RECT Position,
    initializer_list<Control*> Children,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Image*> ImageStyle,
    initializer_list<Animation*> AnimationStyle
) :
    m_Wnd(MainWnd::Instance()),
    m_Parent(nullptr),
    m_MouseEnter(nullptr),
    m_ID(ID),
    m_Position(ZERO_RECT),
    m_DPIPosition(ZERO_RECT),
    m_PositionPercentage(Position),
    m_Margin(ZERO_RECT),
    m_DPIMargin(ZERO_RECT),
    m_MarginPercentage(ZERO_RECT),
    m_Padding(ZERO_RECT),
    m_DPIPadding(ZERO_RECT),
    m_PaddingPercentage(ZERO_RECT),
    m_Hidden(false),
    m_Disabled(false),
    m_Hintable(true),
    m_Children(Children),
    m_LeftButtonClickTick(0),
    m_RightButtonClickTick(0),
    m_LeftButtonDoubleClickTick(0),
    m_RightButtonDoubleClickTick(0),
    m_VerticalScroolEnabled(false),
    m_HorizontalScroolEnabled(false),
    m_BackgroundStyle(CONTROL_STATUS_NORMAL),
    m_BorderStyle(CONTROL_STATUS_NORMAL),
    m_TextStyle(CONTROL_STATUS_NORMAL),
    m_AnimationStyle(CONTROL_STATUS_NORMAL),
    m_ImageStyle(CONTROL_STATUS_NORMAL)
{
    SaveStyleValues(BackgroundStyle, BorderStyle, TextStyle, ImageStyle, AnimationStyle);

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


Control::Control(
    wstring ID,
    RECT Position,
    bool Hidden,
    bool Disabled,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    bool Hintable,
    initializer_list<Control*> Children,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Image*> ImageStyle,
    initializer_list<Animation*> AnimationStyle
) :
    m_Wnd(MainWnd::Instance()),
    m_Parent(nullptr),
    m_MouseEnter(nullptr),
    m_ID(ID),
    m_Position(ZERO_RECT),
    m_DPIPosition(ZERO_RECT),
    m_PositionPercentage(Position),
    m_Margin(ZERO_RECT),
    m_DPIMargin(ZERO_RECT),
    m_MarginPercentage(Margin),
    m_Padding(ZERO_RECT),
    m_DPIPadding(ZERO_RECT),
    m_PaddingPercentage(Padding),
    m_Hidden(Hidden),
    m_Disabled(Disabled),
    m_Hintable(Hintable),
    m_Children(Children),
    m_LeftButtonClickTick(0),
    m_RightButtonClickTick(0),
    m_LeftButtonDoubleClickTick(0),
    m_RightButtonDoubleClickTick(0),
    m_VerticalScroolEnabled(false),
    m_HorizontalScroolEnabled(false),
    m_BackgroundStyle(CONTROL_STATUS_NORMAL),
    m_BorderStyle(CONTROL_STATUS_NORMAL),
    m_TextStyle(CONTROL_STATUS_NORMAL),
    m_AnimationStyle(CONTROL_STATUS_NORMAL),
    m_ImageStyle(CONTROL_STATUS_NORMAL)
{
    SaveStyleValues(BackgroundStyle, BorderStyle, TextStyle, ImageStyle, AnimationStyle);

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


Control::~Control()
{
    // delete children
    for (auto& target : m_Children)
    {
        delete target;
    }

    // clear children
    m_Children.clear();

    // delete styles
    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        for (const auto& style : m_BackgroundStyles[i])
        {
            delete style;
        }
        m_BackgroundStyles[i].clear();

        for (const auto& style : m_BorderStyles[i])
        {
            delete style;
        }
        m_BorderStyles[i].clear();

        for (const auto& style : m_TextStyles[i])
        {
            delete style;
        }
        m_TextStyles[i].clear();

        for (const auto& style : m_ImageStyles[i])
        {
            delete style;
        }
        m_ImageStyles[i].clear();

        for (const auto& style : m_AnimationStyles[i])
        {
            delete style;
        }
        m_AnimationStyles[i].clear();
    }
}


Control* Control::Parent()
{
    return m_Parent;
}


void Control::SetParent(Control* Parent)
{
    m_Parent = Parent;
}


vector<Control*>* Control::Children()
{
    return &m_Children;
}


void Control::Attach(Control* Control)
{
    wstring id = Control->ID();
    if (id.length() > 0)
    {
        Control->SetParent(this);
        m_Children.push_back(Control);
    }
    else
    {
        // TODO: ID empty error
    }
}


void Control::Attach(initializer_list<Control*> Children)
{
    m_Children = Children;

    for (const auto& child : m_Children)
    {
        child->SetParent(this);
    }
}


void Control::Style(CONTROL_STATUS Status)
{
    if (m_BackgroundStyles[Status].size() > 0)
    {
        m_BackgroundStyle = Status;
    }
    if (m_BorderStyles[Status].size() > 0)
    {
        m_BorderStyle = Status;
    }
    if (m_TextStyles[Status].size() > 0)
    {
        m_TextStyle = Status;
    }
    if (m_AnimationStyles[Status].size() > 0)
    {
        m_AnimationStyle = Status;
    }
}


void Control::Style(uint8_t Index, Background* BackgroundStyle, Border* BorderStyle, Text* TextStyle, Animation* AnimationStyle)
{
    uint8_t i;
    CONTROL_STATUS Status;
    
    if (BackgroundStyle != nullptr)
    {
        i = 0;
        Status = BackgroundStyle->Status();
        for (auto& style : m_BackgroundStyles[Status])
        {
            if (i == Index)
            {
                delete style;
                BackgroundStyle->SetOwner(this);
                style = BackgroundStyle;
                break;
            }

            i++;
        }
    }
    if (BorderStyle != nullptr)
    {
        i = 0;
        Status = BorderStyle->Status();
        for (auto& style : m_BorderStyles[Status])
        {
            if (i == Index)
            {
                delete style;
                BorderStyle->SetOwner(this);
                style = BorderStyle;
                break;
            }

            i++;
        }
    }
    if (TextStyle != nullptr)
    {
        i = 0;
        Status = TextStyle->Status();
        for (auto& style : m_TextStyles[Status])
        {
            if (i == Index)
            {
                delete style;
                TextStyle->SetOwner(this);
                style = TextStyle;
                break;
            }

            i++;
        }
    }
    if (AnimationStyle != nullptr)
    {
        i = 0;
        Status = AnimationStyle->Status();
        for (auto& style : m_AnimationStyles[Status])
        {
            if (i == Index)
            {
                delete style;
                AnimationStyle->SetOwner(this);
                style = AnimationStyle;
                break;
            }

            i++;
        }
    }
}


wstring Control::GetContent()
{
    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        for (const auto& style : m_TextStyles[i])
        {
            return style->GetContent();
        }
    }

    return L"";
}


bool Control::SetContent(wstring Content, bool Paint, CONTROL_STATUS Exclue)
{
    bool changed = false;
    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        if (i == Exclue)
        {
            continue;
        }

        for (const auto& style : m_TextStyles[i])
        {
            auto x = style->SetContent(Content);
            if (x)
            {
                changed = true;
            }
        }
    }

    if (changed & Paint)
    {
        Invalidate();
    }

    return changed;
}


bool Control::SetImage(wstring Path, bool Paint)
{
    bool changed = false;
    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        for (const auto& style : m_ImageStyles[i])
        {
            auto x = style->SetPath(Path);
            if (x)
            {
                changed = true;
            }
        }
    }

    if (changed & Paint)
    {
        Invalidate();
    }

    return changed;
}


wstring Control::ID()
{
    return m_ID;
}


MainWnd* Control::Wnd()
{
    return m_Wnd;
}


bool Control::Hidden()
{
    return m_Hidden || (m_Parent != nullptr && m_Parent->Hidden());
}


void Control::Hide()
{
    if (m_Hidden != true)
    {
        m_Hidden = true;
        m_Clear = true;
        Invalidate();
    }
}


void Control::Show()
{
    if (m_Hidden == true)
    {
        m_Hidden = false;
        Invalidate();
    }
}


void Control::SetVisibility(bool Value, bool ReDraw)
{
    bool last = m_Hidden;
    m_Hidden = Value == false;

    if (m_Hidden != last)
    {
        if (m_Hidden)
        {
            m_Clear = true;
        }

        if (ReDraw)
        {
            Invalidate();
        }
    }
}


void Control::FlipVisibility()
{
    m_Hidden = !m_Hidden;
    Invalidate();
}


bool Control::Disabled()
{
    return m_Disabled;
}


void Control::Disable()
{
    m_Disabled = true;
}


void Control::Enable()
{
    m_Disabled = false;
}


void Control::SetUsability(bool Value)
{
    bool last = m_Disabled;
    m_Disabled = Value == false;

    if (m_Disabled != last)
    {
        Invalidate();
    }
}


void Control::FlipUsability()
{
    m_Disabled = !m_Disabled;
    Invalidate();
}


bool Control::Hintable()
{
    return !m_Hidden && !m_Disabled && m_Hintable;
}


bool Control::VerticalScrollEnabled()
{
    return m_VerticalScroolEnabled;
}


bool Control::HorizontalScrollEnabled()
{
    return m_HorizontalScroolEnabled;
}


void Control::Resize()
{
    SavePosition();
}


RECT Control::Position()
{
    return m_Position;
}


D2D_RECT_F Control::DPIPosition()
{
    return m_DPIPosition;
}


inline RECT Control::PositionPercentage()
{
    return m_PositionPercentage;
}


inline LONG Control::Width()
{
    return m_Position.right - m_Position.left;
}


inline LONG Control::Height()
{
    return m_Position.bottom - m_Position.top;
}


LONG Control::WidthPercentage()
{
    return m_PositionPercentage.right - m_PositionPercentage.left;
}


LONG Control::HeightPercentage()
{
    return m_PositionPercentage.bottom - m_PositionPercentage.top;
}


void Control::SetPosition(RECT Pos)
{
    m_PositionPercentage = Pos;
}


void Control::SetPosition(LONG Left, LONG Top, LONG Right, LONG Bottom)
{
    m_PositionPercentage.left = Left;
    m_PositionPercentage.top = Top;
    m_PositionPercentage.right = Right;
    m_PositionPercentage.bottom = Bottom;
}


void Control::VerticalMovePosition(LONG Offset, FLOAT DpiOffset, LONG PercentOffset)
{
    m_Position.top += Offset;
    m_Position.bottom += Offset;

    m_PositionPercentage.top += PercentOffset;
    m_PositionPercentage.bottom += PercentOffset;

    m_DPIPosition.top += DpiOffset;
    m_DPIPosition.bottom += DpiOffset;

    m_Hidden = m_PositionPercentage.top < 0 || m_PositionPercentage.bottom > DENOMINATOR;

    SaveStylePosition();
}


void Control::HorizontalMovePosition(LONG Offset, FLOAT DpiOffset, LONG PercentOffset)
{
    m_Position.left += Offset;
    m_Position.right += Offset;

    m_PositionPercentage.left += PercentOffset;
    m_PositionPercentage.right += PercentOffset;

    m_DPIPosition.left += DpiOffset;
    m_DPIPosition.right += DpiOffset;

    m_Hidden = m_PositionPercentage.left < 0 || m_PositionPercentage.right > DENOMINATOR;

    SaveStylePosition();
}


RECT Control::Padding()
{
    return m_Padding;
}


D2D_RECT_F Control::DPIPadding()
{
    return m_DPIPadding;
}


void Control::Invalidate()
{
    PostMessage(m_Wnd->NativeWnd(), WM_PAINT, ON_PAINT_WPARAM, (LPARAM)this);
}


void Control::OnPaint(Control* target, bool Refresh)
{
    if (target != nullptr && !Refresh)
    {
        target->OnPaint(true);
    }
    else
    {
        OnPaint();

        for (const auto& child : m_Children)
        {
            if (!child->Hidden())
            {
                child->OnPaint(nullptr, true);
            }
        }
    }
}


void Control::OnPaint(bool PaintChildren)
{
    auto hidden = Hidden();
    if (Hidden() && !m_Clear)
    {
        return;
    }

    bool clear = strcmp(this->TypeName(), typeid(SliderBarBody).name()) == 0;
    for (const auto& style : m_BackgroundStyles[m_BackgroundStyle])
    {
        style->OnPaint(clear);
        if (hidden && m_Clear)
        {
            break;
        }
    }

    if (hidden && m_Clear)
    {
        m_Clear = false;
        return;
    }

    for (const auto& style : m_BorderStyles[m_BorderStyle])
    {
        style->OnPaint();
    }

    for (const auto& style : m_ImageStyles[m_ImageStyle])
    {
        style->OnPaint();
    }

    for (const auto& style : m_TextStyles[m_TextStyle])
    {
        style->OnPaint();
    }

    if (PaintChildren)
    {
        for (const auto& child : m_Children)
        {
            if (!child->Hidden())
            {
                child->OnPaint(true);
            }
        }
    }
}


void Control::OnAnimated()
{
    if (m_Hidden)
    {
        return;
    }

    for (const auto& style : m_AnimationStyles[m_AnimationStyle])
    {
        style->OnAnimated();
    }
}


void Control::OnLeftButtonDown(POINT Pt)
{
    Control* child = Find(Pt);
    if (child != nullptr)
    {
        child->OnLeftButtonDown(Pt);
    }
    else if (Hintable())
    {
        m_LeftButtonDownPoint = Pt;
        m_LeftButtonDownTick = TICK_COUNT;
        OnLeftButtonDown(Pt.x, Pt.y);
    }
}


void Control::OnLeftButtonDown(LONG X, LONG Y)
{
    OnFocus();
}


void Control::OnLeftButtonUp(POINT Pt)
{
    Control* child = Find(Pt);
    if (child != nullptr)
    {
        child->OnLeftButtonUp(Pt);
    }
    else if (Hintable())
    {
        m_LeftButtonUpPoint = Pt;
        m_LeftButtonUpTick = TICK_COUNT;

        OnLeftButtonUp(Pt.x, Pt.y);

        if (
            m_LeftButtonDownTick > 0 &&
            m_LeftButtonUpTick > 0 &&
            m_LeftButtonUpTick - m_LeftButtonDownTick <= CLICK_DELTA &&
            m_LeftButtonDownPoint.x >= 0 &&
            m_LeftButtonDownPoint.y >= 0 &&
            m_LeftButtonUpPoint.x >= 0 &&
            m_LeftButtonUpPoint.y >= 0
            )
        {
            OnLeftButtonClick(Pt.x, Pt.y);
        }

        m_LeftButtonDownPoint = { -1, -1 };
        m_LeftButtonUpPoint = { -1, -1 };

        m_LeftButtonDownTick = 0;
        m_LeftButtonUpTick = 0;
    }
}


void Control::OnLeftButtonUp(LONG X, LONG Y)
{
}


void Control::OnLeftButtonClick(LONG X, LONG Y)
{
    m_Wnd->GetUI()->OnLeftClick(this);

    if (m_LeftButtonUpTick - m_LeftButtonClickTick <= DBL_CLICK_DELTA && m_LeftButtonUpTick - m_LeftButtonDoubleClickTick > DBL_CLICK_DELTA)
    {
        OnLeftButtonDoubleClick(X, Y);
        m_LeftButtonDoubleClickTick = m_LeftButtonUpTick;
    }

    m_LeftButtonClickTick = m_LeftButtonUpTick;
}


void Control::OnLeftButtonDoubleClick(LONG X, LONG Y)
{
    m_Wnd->GetUI()->OnLeftDoubleClick(this);
}


void Control::OnRightButtonDown(POINT Pt)
{
    Control* child = Find(Pt);
    if (child != nullptr)
    {
        child->OnRightButtonDown(Pt);
    }
    else if (Hintable())
    {
        m_RightButtonDownPoint = Pt;
        m_RightButtonDownTick = TICK_COUNT;
        OnRightButtonDown(Pt.x, Pt.y);
    }
}


void Control::OnRightButtonDown(LONG X, LONG Y)
{
    OnFocus();
}


void Control::OnRightButtonUp(POINT Pt, WPARAM wParam)
{
    Control* child = Find(Pt);
    if (child != nullptr)
    {
        child->OnRightButtonUp(Pt, wParam);
    }
    else if (Hintable())
    {
        m_RightButtonUpPoint = Pt;
        m_RightButtonUpTick = TICK_COUNT;

        OnRightButtonUp(Pt.x, Pt.y);

        if (
            m_RightButtonDownTick > 0 &&
            m_RightButtonUpTick > 0 &&
            m_RightButtonUpTick - m_RightButtonDownTick <= CLICK_DELTA &&
            m_RightButtonDownPoint.x >= 0 &&
            m_RightButtonDownPoint.y >= 0 &&
            m_RightButtonUpPoint.x >= 0 &&
            m_RightButtonUpPoint.y >= 0
            )
        {
            OnRightButtonClick(Pt.x, Pt.y);
        }

        m_RightButtonDownPoint = { -1, -1 };
        m_RightButtonUpPoint = { -1, -1 };

        m_RightButtonDownTick = 0;
        m_RightButtonUpTick = 0;
    }
}


void Control::OnRightButtonUp(LONG X, LONG Y)
{
}


void Control::OnRightButtonClick(LONG X, LONG Y)
{
    m_Wnd->GetUI()->OnRightClick(this);

    if (m_RightButtonUpTick - m_RightButtonClickTick <= DBL_CLICK_DELTA && m_RightButtonUpTick - m_RightButtonDoubleClickTick > DBL_CLICK_DELTA)
    {
        OnRightButtonDoubleClick(X, Y);
        m_RightButtonDoubleClickTick = m_RightButtonUpTick;
    }

    m_RightButtonClickTick = m_RightButtonUpTick;
}


void Control::OnRightButtonDoubleClick(LONG X, LONG Y)
{
    m_Wnd->GetUI()->OnRightDoubleClick(this);
}


void Control::OnMouseMove(POINT Pt, WPARAM wParam)
{
    Control* child = Find(Pt);
    if (child != nullptr)
    {
        if (child != m_MouseEnter)
        {
            if (m_MouseEnter != nullptr && m_MouseEnter->Hintable())
            {
                m_MouseEnter->OnMouseLeave(m_MouseEnter, Pt);
            }

            if (child->Hintable())
            {
                child->OnMouseEnter(child, Pt);
            }

            m_MouseEnter = child;
        }
        else if (wParam == MK_LBUTTON)
        {
            if (child->Hintable() && (Pt.x - m_LastMousePoint.x != 0 || Pt.y - m_LastMousePoint.y != 0))
            {
                child->OnMouseDrag(child, Pt, m_LastMousePoint);
            }
        }
    }
    else
    {
        if (m_MouseEnter != nullptr && m_MouseEnter->Hintable())
        {
            m_MouseEnter->OnMouseLeave(m_MouseEnter, Pt);
            m_MouseEnter = nullptr;
        }
    }

    m_LastMousePoint = Pt;
}


void Control::OnMouseEnter(Control* Target, POINT Pt)
{
    Control* child = Target->Find(Pt);
    if (child != nullptr)
    {
        child->OnMouseEnter(child, Pt);
    }
    else if (Hintable())
    {
        OnMouseEnter(Pt.x, Pt.y);
    }
}


void Control::OnMouseEnter(LONG X, LONG Y)
{
}


void Control::OnMouseLeave(Control* Target, POINT Pt)
{
    Control* child = Target->Find(Pt);
    if (child != nullptr)
    {
        child->OnMouseLeave(child, Pt);
    }
    else if (Hintable())
    {
        OnMouseLeave(Pt.x, Pt.y);
    }
}


void Control::OnMouseLeave(LONG X, LONG Y)
{
    for (const auto& child : m_Children)
    {
        if (child->Hintable())
        {
            child->OnMouseLeave(X, Y);
        }
    }
}


void Control::OnMouseDrag(Control* Target, POINT Pt, POINT LastPt)
{
    Control* child = Target->Find(Pt);
    if (child != nullptr)
    {
        child->OnMouseDrag(child, Pt, LastPt);
    }
    else if (Hintable())
    {
        OnMouseDrag(Pt, LastPt);
    }
}


void Control::OnMouseDrag(POINT Pt, POINT LastPt)
{
}


void Control::OnMouseVerticalWheel(POINT Pt, WPARAM wParam)
{
    if (m_Parent == nullptr)
    {
        ScreenToClient(m_Wnd->NativeWnd(), &Pt);
    }

    Control* child = Find(Pt);
    if (child != nullptr && child->Hintable() && child->VerticalScrollEnabled())
    {
        child->OnMouseVerticalWheel(Pt, wParam);
    }
    else if (Hintable() && m_VerticalScroolEnabled)
    {
        OnMouseVerticalWheel(Pt.x, Pt.y, wParam);
    }
}


void Control::OnMouseVerticalWheel(LONG X, LONG Y, WPARAM wParam)
{
    m_Wnd->GetUI()->OnMouseVerticalScroll(this, (short)HIWORD(wParam));
}


void Control::OnMouseHorizontalWheel(POINT Pt, WPARAM wParam)
{
    if (m_Parent == nullptr)
    {
        ScreenToClient(m_Wnd->NativeWnd(), &Pt);
    }

    Control* child = Find(Pt);
    if (child != nullptr && child->Hintable() && child->HorizontalScrollEnabled())
    {
        child->OnMouseHorizontalWheel(Pt, wParam);
    }
    else if (Hintable() && m_HorizontalScroolEnabled)
    {
        OnMouseHorizontalWheel(Pt.x, Pt.y, wParam);
    }
}


void Control::OnMouseHorizontalWheel(LONG X, LONG Y, WPARAM wParam)
{
    m_Wnd->GetUI()->OnMouseHorizontalScroll(this, (short)HIWORD(wParam));
}


void Control::OnFocus()
{
    m_Wnd->GetUI()->OnFocus(this);
}


void Control::OnLoseFocus()
{
}


void Control::OnCharInput(WPARAM wParam, LPARAM lParam)
{
}


void Control::OnKeyInput(WPARAM wParam, LPARAM lParam)
{
}


Control* Control::Find(POINT& Pt)
{
    for (const auto& child : m_Children)
    {
        if (child->Hintable() && child->Hint(Pt))
        {
            return child;
        }
    }
    return nullptr;
}


Control* Control::Find(RECT& Position)
{
    for (const auto& child : m_Children)
    {
        if (child->Hintable() && child->Hint(Position))
        {
            return child;
        }
    }
    return nullptr;
}


Control* Control::Find(wstring ID)
{
    return nullptr;
}


void Control::Collect(unordered_map<wstring, Control*>& Container)
{
    Container[m_ID] = this;

    for (const auto& child : m_Children)
    {
        child->Collect(Container);
    }
}


bool Control::Hint(POINT& Pt)
{
    return PtInRect(&m_Position, Pt) == TRUE;
}


bool Control::Hint(RECT& Position)
{
    return EqualRect(&m_Position, &Position) == TRUE;
}


bool Control::Contains(RECT& Position)
{
    return m_Position.left <= Position.left
        && m_Position.top <= Position.top
        && m_Position.right >= Position.right
        && m_Position.bottom >= Position.bottom;
}


void Control::SavePosition(UINT16 Index)
{
    // parent info
    RECT parentPos;
    D2D_RECT_F parentDPIPos;
    RECT parentPadding;
    D2D_RECT_F parentDPIPadding;
    if (m_Wnd->IsRoot(this))
    {
        parentPos = m_Wnd->Position();
        parentDPIPos = {
            (float)parentPos.left / m_Wnd->DpiScaleX(),
            (float)parentPos.top / m_Wnd->DpiScaleY(),
            (float)parentPos.right / m_Wnd->DpiScaleX(),
            (float)parentPos.bottom / m_Wnd->DpiScaleY()
        };
        parentPadding = { 0, 0, 0, 0 };
        parentDPIPadding = { 0, 0, 0, 0 };
    }
    else if (m_Parent != nullptr)
    {
        parentPos = m_Parent->Position();
        parentDPIPos = m_Parent->DPIPosition();
        parentPadding = m_Parent->Padding();
        parentDPIPadding = m_Parent->DPIPadding();
    }
    else
    {
        return;
    }

    auto parentWidth = parentPos.right - parentPos.left - parentPadding.left - parentPadding.right;
    auto parentHeight = parentPos.bottom - parentPos.top - parentPadding.top - parentPadding.bottom;

    auto parentDPIWidth = parentDPIPos.right - parentDPIPos.left - parentDPIPadding.left - parentDPIPadding.right;
    auto parentDPIHeight = parentDPIPos.bottom - parentDPIPos.top - parentDPIPadding.top - parentDPIPadding.bottom;

    // calc margin
    if (!IS_ZERO_RECT(m_MarginPercentage))
    {
        m_Margin = {
            m_MarginPercentage.left > 0 ? m_MarginPercentage.left * parentWidth / DENOMINATOR : -m_MarginPercentage.left,
            m_MarginPercentage.top > 0 ? m_MarginPercentage.top * parentHeight / DENOMINATOR : -m_MarginPercentage.top,
            m_MarginPercentage.right > 0 ? m_MarginPercentage.right * parentWidth / DENOMINATOR : -m_MarginPercentage.right,
            m_MarginPercentage.bottom > 0 ? m_MarginPercentage.bottom * parentHeight / DENOMINATOR : -m_MarginPercentage.bottom
        };

        m_DPIMargin = {
            m_MarginPercentage.left > 0 ? m_MarginPercentage.left * parentDPIWidth / DENOMINATOR : -m_MarginPercentage.left,
            m_MarginPercentage.top > 0 ? m_MarginPercentage.top * parentDPIHeight / DENOMINATOR : -m_MarginPercentage.top,
            m_MarginPercentage.right > 0 ? m_MarginPercentage.right * parentDPIWidth / DENOMINATOR : -m_MarginPercentage.right,
            m_MarginPercentage.bottom > 0 ? m_MarginPercentage.bottom * parentDPIHeight / DENOMINATOR : -m_MarginPercentage.bottom
        };
    }

    // calc padding
    if (!IS_ZERO_RECT(m_PaddingPercentage))
    {
        m_Padding = {
            m_PaddingPercentage.left > 0 ? m_PaddingPercentage.left * parentWidth / DENOMINATOR : -m_PaddingPercentage.left,
            m_PaddingPercentage.top > 0 ? m_PaddingPercentage.top * parentHeight / DENOMINATOR : -m_PaddingPercentage.top,
            m_PaddingPercentage.right > 0 ? m_PaddingPercentage.right * parentWidth / DENOMINATOR : -m_PaddingPercentage.right,
            m_PaddingPercentage.bottom > 0 ? m_PaddingPercentage.bottom * parentHeight / DENOMINATOR : -m_PaddingPercentage.bottom
        };

        m_DPIPadding = {
            m_PaddingPercentage.left > 0 ? m_PaddingPercentage.left * parentDPIWidth / DENOMINATOR : -m_PaddingPercentage.left,
            m_PaddingPercentage.top > 0 ? m_PaddingPercentage.top * parentDPIHeight / DENOMINATOR : -m_PaddingPercentage.top,
            m_PaddingPercentage.right > 0 ? m_PaddingPercentage.right * parentDPIWidth / DENOMINATOR : -m_PaddingPercentage.right,
            m_PaddingPercentage.bottom > 0 ? m_PaddingPercentage.bottom * parentDPIHeight / DENOMINATOR : -m_PaddingPercentage.bottom
        };
    }

    // calc position
    LONG widthMargin;
    LONG heightMargin;
    float widthDPIMargin;
    float heightDPIMargin;
    if (Index == 0)
    {
        Index++;
        widthMargin = m_Margin.left;
        heightMargin = m_Margin.top;
        widthDPIMargin = m_DPIMargin.left;
        heightDPIMargin = m_DPIMargin.top;
    }
    else
    {
        widthMargin = m_Margin.left + m_Margin.right;
        heightMargin = m_Margin.top + m_Margin.bottom;
        widthDPIMargin = m_DPIMargin.left + m_DPIMargin.right;
        heightDPIMargin = m_DPIMargin.top + m_DPIMargin.bottom;
    }
    if (IS_ZERO_RECT(m_PositionPercentage) || IS_FULL_RECT(m_PositionPercentage))
    {
        m_PositionPercentage.right = m_PositionPercentage.bottom = DENOMINATOR;

        m_Position = {
            parentPos.left + parentPadding.left + Index * widthMargin,
            parentPos.top + parentPadding.top + Index * heightMargin,
            parentPos.right - parentPadding.right - Index * widthMargin,
            parentPos.bottom - parentPadding.bottom - Index * heightMargin,
        };

        m_DPIPosition = {
            parentDPIPos.left + parentDPIPadding.left + Index * widthDPIMargin,
            parentDPIPos.top + parentDPIPadding.top + Index * heightDPIMargin,
            parentDPIPos.right - parentDPIPadding.right - Index * widthDPIMargin,
            parentDPIPos.bottom - parentDPIPadding.bottom - Index * heightDPIMargin,
        };
    }
    else
    {
        m_Position = {
            parentPos.left + parentPadding.left + Index * widthMargin + m_PositionPercentage.left * parentWidth / DENOMINATOR,
            parentPos.top + parentPadding.top + Index * heightMargin + m_PositionPercentage.top * parentHeight / DENOMINATOR,
            parentPos.left + parentPadding.left + Index * widthMargin + m_PositionPercentage.right * parentWidth / DENOMINATOR,
            parentPos.top + parentPadding.top + Index * heightMargin + m_PositionPercentage.bottom * parentHeight / DENOMINATOR,
        };

        m_DPIPosition = {
            parentDPIPos.left + parentDPIPadding.left + Index * widthDPIMargin + m_PositionPercentage.left * parentDPIWidth / DENOMINATOR,
            parentDPIPos.top + parentDPIPadding.top + Index * heightDPIMargin + m_PositionPercentage.top * parentDPIHeight / DENOMINATOR,
            parentDPIPos.left + parentDPIPadding.left + Index * widthDPIMargin + m_PositionPercentage.right * parentDPIWidth / DENOMINATOR,
            parentDPIPos.top + parentDPIPadding.top + Index * heightDPIMargin + m_PositionPercentage.bottom * parentDPIHeight / DENOMINATOR,
        };
    }

    XTRACE(
        "%s (%d)\n"
        "    margin: [%ld, %ld, %ld, %ld]\n"
        "    padding: [%ld, %ld, %ld, %ld]\n"
        "    parent pos: [%ld, %ld, %ld, %ld]\n"
        "    pos: [%ld, %ld, %ld, %ld]\n"
        "    dpi pos: [%f, %f, %f, %f]\n",
        m_ID.c_str(), Index,
        m_Margin.left, m_Margin.top, m_Margin.right, m_Margin.bottom,
        m_Padding.left, m_Padding.top, m_Padding.right, m_Padding.bottom,
        parentPos.left, parentPos.top, parentPos.right, parentPos.bottom,
        m_Position.left, m_Position.top, m_Position.right, m_Position.bottom,
        m_DPIPosition.left, m_DPIPosition.top, m_DPIPosition.right, m_DPIPosition.bottom
    );

    // calc styles' position
    SaveStylePosition();

    // calc children's position
    UINT16 i = 0;
    for (const auto& child : m_Children)
    {
        child->SavePosition(i++);
    }
}


const char* Control::TypeName()
{
    return typeid(*this).name();
}


size_t Control::Size()
{
    return m_Children.size();
}


void Control::SaveStylePosition()
{
    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        for (const auto& style : m_BackgroundStyles[i])
        {
            style->SavePosition();
        }

        for (const auto& style : m_BorderStyles[i])
        {
            style->SavePosition();
        }

        for (const auto& style : m_TextStyles[i])
        {
            style->SavePosition();
        }

        for (const auto& style : m_AnimationStyles[i])
        {
            style->SavePosition();
        }

        for (const auto& style : m_ImageStyles[i])
        {
            style->SavePosition();
        }
    }
}


void Control::SaveStyleValues(
    initializer_list<Background*>& BackgroundStyle,
    initializer_list<Border*>& BorderStyle,
    initializer_list<Text*>& TextStyle,
    initializer_list<Image*>& ImageStyle,
    initializer_list<Animation*>& AnimationStyle
)
{
    for (const auto& style : BackgroundStyle)
    {
        style->SetOwner(this);
        m_BackgroundStyles[style->Status()].push_back(style);
    }

    for (const auto& style : BorderStyle)
    {
        style->SetOwner(this);
        m_BorderStyles[style->Status()].push_back(style);
    }

    for (const auto& style : TextStyle)
    {
        style->SetOwner(this);
        m_TextStyles[style->Status()].push_back(style);
    }

    for (const auto& style : ImageStyle)
    {
        style->SetOwner(this);
        m_ImageStyles[style->Status()].push_back(style);
    }

    for (const auto& style : AnimationStyle)
    {
        style->SetOwner(this);
        m_AnimationStyles[style->Status()].push_back(style);
    }

    for (const auto& child : m_Children)
    {
        child->SetParent(this);
    }
}
