#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Common.h"
#include "Style.h"

// windows
#include <d2d1.h>
#include <dwrite.h>

// c++
#include <string>

// forward
class MainWnd;



class Text : public Style
{
public:
    // Contructor
    Text(
        std::wstring Content,

        CONTROL_STATUS Status,

        D2D_RECT_F Position = ZERO_RECT,

        float RadiusX = 0,
        float RadiusY = 0,

        D2D1_COLOR_F Color = D2D1::ColorF(D2D1::ColorF::Black),
        D2D1_COLOR_F ClearColor = D2D1::ColorF(D2D1::ColorF::White),

        DWRITE_TEXT_ALIGNMENT TextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER,
        DWRITE_PARAGRAPH_ALIGNMENT ParagraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER,

        const wchar_t* FontFamily = L"Airal",
        IDWriteFontCollection* FontCollection = NULL,
        DWRITE_FONT_WEIGHT FontWeight = DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE FontStyle = DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH FontStretch = DWRITE_FONT_STRETCH_NORMAL,
        float FontSize = 10.0f,
        const wchar_t* LocaleName = L"en-us"
    );
    // Deconstructor
    ~Text();

    // Handle paint event
    virtual void OnPaint();

    virtual void SavePosition();

    std::wstring GetContent();
    void SetContent(std::wstring Content);


private:
    MainWnd* m_Wnd;

    std::wstring m_Content;

    ID2D1SolidColorBrush* m_Brush;
    IDWriteTextFormat* m_TextFormat;
    IDWriteTextLayout* m_TextLayout;

    DWRITE_TEXT_ALIGNMENT m_TextAlignment;
    DWRITE_PARAGRAPH_ALIGNMENT m_ParagraphAlignment;

    const wchar_t* m_FontFamily;
    IDWriteFontCollection* m_FontCollection;
    DWRITE_FONT_WEIGHT m_FontWeight;
    DWRITE_FONT_STYLE m_FontStyle;
    DWRITE_FONT_STRETCH m_FontStretch;
    float m_FontSize;
    const wchar_t* m_LocaleName;
};

