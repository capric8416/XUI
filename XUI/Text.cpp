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
    D2D1_COLOR_F ClearColor,

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
    ClearColor,
    RadiusX,
    RadiusY
),
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
    HRESULT hr = S_OK;

    // Create brush
    hr = m_RenderTarget->CreateSolidColorBrush(Color, &m_Brush);
}


Text::~Text()
{
    XSafeRelease(&m_TextLayout);
    XSafeRelease(&m_TextFormat);
    XSafeRelease(&m_Brush);
}


void Text::OnPaint()
{
    if (m_TextLayout == NULL)
    {
        m_DWriteFactory->CreateTextLayout(
            m_Content.c_str(),
            m_Content.length(),
            m_TextFormat,
            m_Position.rect.right - m_Position.rect.left,
            m_Position.rect.bottom - m_Position.rect.top,
            &m_TextLayout
        );
    }

    m_RenderTarget->DrawTextLayout(D2D1::Point2F(m_Position.rect.left, m_Position.rect.top), m_TextLayout, m_Brush);
}


void Text::SavePosition()
{
    __super::SavePosition();

    HRESULT hr = S_OK;

    // Create format
    hr = m_DWriteFactory->CreateTextFormat(
        m_FontFamily,
        m_FontCollection,
        m_FontWeight,
        m_FontStyle,
        m_FontStretch,
        m_FontSize,
        m_LocaleName,
        &m_TextFormat
    );
    hr = m_TextFormat->SetTextAlignment(m_TextAlignment);
    hr = m_TextFormat->SetParagraphAlignment(m_ParagraphAlignment);
    hr = m_TextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

    DWRITE_TRIMMING trim1;
    trim1.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
    trim1.delimiter = 1;
    trim1.delimiterCount = 10;
    IDWriteInlineObject* trim2 = NULL;
    m_DWriteFactory->CreateEllipsisTrimmingSign(m_TextFormat, &trim2);
    hr = m_TextFormat->SetTrimming(&trim1, trim2);

    // Create layout
    hr = m_DWriteFactory->CreateTextLayout(
        m_Content.c_str(),
        m_Content.length(),
        m_TextFormat,
        m_Position.rect.right - m_Position.rect.left,
        m_Position.rect.bottom - m_Position.rect.top,
        &m_TextLayout
    );
}


wstring Text::GetContent()
{
    return m_Content;
}


void Text::SetContent(wstring Content)
{
    HRESULT hr = S_OK;

    m_Content = Content;

    XSafeRelease(&m_TextLayout);
    hr = m_DWriteFactory->CreateTextLayout(
        m_Content.c_str(),
        m_Content.length(),
        m_TextFormat,
        m_Position.rect.right - m_Position.rect.left,
        m_Position.rect.bottom - m_Position.rect.top,
        &m_TextLayout
    );
}

