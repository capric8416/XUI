// self
#include "Text.h"

// project
#include "MainWnd.h"
#include "Control.h"

// namespace
using namespace std;



Text::Text(
    wstring Content,

    CONTROL_STATUS Status,

    D2D_RECT_F Position,
    float RadiusX,
    float RadiusY,

    D2D1_COLOR_F Color,

    DWRITE_TEXT_ALIGNMENT TextAlignment,
    DWRITE_PARAGRAPH_ALIGNMENT ParagraphAlignment,

    const wchar_t* FontFamily,
    IDWriteFontCollection* FontCollection,
    DWRITE_FONT_WEIGHT FontWeight,
    DWRITE_FONT_STYLE FontStyle,
    DWRITE_FONT_STRETCH FontStretch,
    float FontSize,
    const wchar_t* LocaleName
) :
Style(
    Status,
    Position,
    RadiusX,
    RadiusY
),
m_Color(Color),
m_Content(Content),
m_TextAlignment(TextAlignment),
m_ParagraphAlignment(ParagraphAlignment),
m_FontFamily(FontFamily),
m_FontCollection(FontCollection),
m_FontWeight(FontWeight),
m_FontStyle(FontStyle),
m_FontStretch(FontStretch),
m_FontSize(FontSize),
m_LocaleName(LocaleName)
{
    SolidBrush();
}


Text::~Text()
{
}


void Text::OnPaint()
{
    s_RenderTarget->DrawTextLayout(D2D1::Point2F(m_Position.rect.left, m_Position.rect.top), TextLayout(), SolidBrush());
}


void Text::SavePosition()
{
    __super::SavePosition();

    TextLayout(true);
}


wstring Text::GetContent()
{
    return m_Content;
}


bool Text::SetContent(wstring Content)
{
    if (Content == m_Content)
    {
        return false;
    }

    TextLayout(Content);

    m_Content = Content;

    return true;
}


ID2D1SolidColorBrush* Text::SolidBrush()
{
    return __super::SolidBrush(m_Color);
}


IDWriteTextFormat* Text::TextFormat()
{
    return __super::TextFormat(
        m_FontFamily, m_FontCollection, m_FontWeight, m_FontStyle, m_FontStretch,
        m_FontSize, m_LocaleName, m_TextAlignment, m_ParagraphAlignment, true
    );
}


IDWriteTextLayout* Text::TextLayout(bool Resize)
{
    return __super::TextLayout(
        m_Content,
        TextFormat(),
        m_Position.rect.right - m_Position.rect.left,
        m_Position.rect.bottom - m_Position.rect.top
    );
}


IDWriteTextLayout* Text::TextLayout(std::wstring Content)
{
    return __super::TextLayout(
        Content,
        TextFormat(),
        m_Position.rect.right - m_Position.rect.left,
        m_Position.rect.bottom - m_Position.rect.top
    );
}

