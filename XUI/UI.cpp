// self
#include "UI.h"

// project
#include "Animation.h"



UI::UI() :
    m_Tree(nullptr),
    m_Resized(false),
    m_Resizing(false),
    m_Prepared(false),
    m_MouseScrollLines(0)
{
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_MouseScrollLines, 0);
}


UI::~UI()
{
    XSafeDelete(m_Tree);
}


Control* UI::Create()
{
    return nullptr;
}


void UI::Resize()
{
    if (!m_Resizing)
    {
        m_Resizing = true;

        m_Tree->Resize();

        m_Resized = true;

        m_Resizing = false;
    }
}


void UI::Prepared()
{
    m_Prepared = true;
}


SHORT UI::MouseScrollLines()
{
    return m_MouseScrollLines;
}


void UI::StartAnimation(bool Enabled, UINT8 FPS)
{
    if (Enabled)
    {
        Animation::Start(FPS);
    }
}


void UI::StopAnimation(bool Enabled)
{
    if (Enabled)
    {
        Animation::Stop();
    }
}


void UI::ExecuteAnimation()
{
    Animation::Execute();
}


void UI::OnPaint(Control* Target, bool Refresh)
{
    if (m_Resized)
    {
        m_Tree->OnPaint(Target, Refresh);

        if (Target == nullptr && Refresh)
        {
            Prepared();
        }
    }
}


void UI::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        m_Tree->OnLeftButtonDown(POINT{ LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_LBUTTONUP:
        m_Tree->OnLeftButtonUp(POINT{ LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_RBUTTONDOWN:
        m_Tree->OnRightButtonDown(POINT{ LOWORD(lParam), HIWORD(lParam) });
        break;

    case WM_RBUTTONUP:
        m_Tree->OnRightButtonUp(POINT{ LOWORD(lParam), HIWORD(lParam) }, wParam);
        break;

    case WM_MOUSEMOVE:
        m_Tree->OnMouseMove(POINT{ LOWORD(lParam), HIWORD(lParam) }, wParam);
        break;

    case WM_MOUSEWHEEL:
        m_Tree->OnMouseVerticalWheel(POINT{ LOWORD(lParam), HIWORD(lParam) }, wParam);
        break;

    case WM_MOUSEHWHEEL:
        m_Tree->OnMouseHorizontalWheel(POINT{ LOWORD(lParam), HIWORD(lParam) }, wParam);
        break;
    }
}


bool UI::IsRoot(Control* Target)
{
    return m_Tree == Target;
}


void UI::OnLeftClick(Control* Target)
{
}


void UI::OnLeftDoubleClick(Control* Target)
{
}


void UI::OnRightClick(Control* Target)
{
}


void UI::OnRightDoubleClick(Control* Target)
{
}


void UI::OnGroupItemChanged(Control* Parent, Control* Last, Control* Current, LONG Value)
{
}


void UI::OnSliderValueChanged(Control* Parent, Control* Target, INT32 Last, INT32 Current)
{
}


void UI::OnMouseVerticalScroll(Control* Target, SHORT Delta)
{
}


void UI::OnMouseHorizontalScroll(Control* Target, SHORT Delta)
{
}