// self
#include "UI.h"

// project
#include "Edit.h"
#include "Animation.h"
#include "Language.h"


// namespace
using namespace std;



UI::UI(Language* Lang) :
    m_Language(Lang),
    m_Tree(nullptr),
    m_MainWnd(nullptr),
    m_Focused(nullptr),
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
    XSafeDelete(m_Language);
}


MainWnd* UI::GetMainWnd()
{
    return m_MainWnd;
}


void UI::SetMainWnd(MainWnd* Wnd)
{
    m_MainWnd = Wnd;
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


void UI::OnCharInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_Focused && m_Focused->TypeName() == typeid(SingleLineEdit).name())
    {
        m_Focused->OnCharInput(wParam, lParam);
    }
}


int UI::OnCharInput(wstring ID, wstring Text, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


void UI::OnKeyInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (m_Focused && m_Focused->TypeName() == typeid(SingleLineEdit).name())
    {
        m_Focused->OnKeyInput(wParam, lParam);
    }
}


int UI::OnKeyInput(wstring ID, int Key, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
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


int UI::OnLeftClick(wstring ID, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


int UI::OnLeftDoubleClick(wstring ID, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


void UI::OnRightClick(Control* Target)
{
}


void UI::OnRightDoubleClick(Control* Target)
{
}


int UI::OnRightClick(wstring ID, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


int UI::OnRightDoubleClick(wstring ID, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


void UI::OnGroupItemChanged(Control* Parent, Control* Last, Control* Current, LONG Value)
{
}


void UI::OnSliderValueChanged(Control* Parent, Control* Target, INT32 Last, INT32 Current)
{
}


int UI::OnGroupItemChanged(wstring ID, LONG Pos, LONG Value, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


int UI::OnSliderValueChanged(wstring ID, LONG Value, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


void UI::OnMouseVerticalScroll(Control* Target, SHORT Delta)
{
}


void UI::OnMouseHorizontalScroll(Control* Target, SHORT Delta)
{
}


int UI::OnMouseVerticalScroll(wstring ID, SHORT Delta, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


int UI::OnMouseHorizontalScroll(wstring ID, SHORT Delta, wchar_t* Buffer)
{
    wcscpy(Buffer, L"{\"message\": \"Not implemented\"}");
    return 0;
}


void UI::OnFocus(Control* Target)
{
    if (m_Focused != nullptr)
    {
        m_Focused->OnLoseFocus();
    }

    if (Target != nullptr)
    {
        m_Focused = Target;
    }
}


Language* UI::GetLanguage()
{
    return m_Language;
}
