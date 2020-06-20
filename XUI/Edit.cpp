// self
#include "Edit.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"

// namespace
using namespace std;



SingleLineEdit::SingleLineEdit(
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


SingleLineEdit::~SingleLineEdit()
{
}


void SingleLineEdit::OnPaint(bool PaintChildren)
{
   m_TextStyle = GetContent().empty() ? CONTROL_STATUS_DISABLED : CONTROL_STATUS_NORMAL;

    __super::OnPaint(PaintChildren);
}


void SingleLineEdit::OnCharInput(WPARAM wParam, LPARAM lParam)
{
    auto text = GetContent();
    auto len = text.length();
    text.resize(len + 1);
    text[len] = (wchar_t)wParam;
    SetContent(text, true, CONTROL_STATUS_DISABLED);
}


void SingleLineEdit::OnKeyInput(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
        case VK_LEFT:
        {
        }
        break;

        case VK_RIGHT:
        {
        }
        break;

        case VK_BACK:
        {
            auto text = GetContent();
            if (!text.empty())
            {
                SetContent(text.substr(0, text.length() - 1), true, CONTROL_STATUS_DISABLED);
            }
        }
        break;

        case VK_HOME:
        {
        }
        break;

        case VK_END:
        {
        }
        break;

        case VK_DELETE:
        {
        }
        break;

        case VK_ESCAPE:
        {
        }
        break;

        case VK_RETURN:
        {
        }
        break;
    }
}
