#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Common.h"

// windows
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

// c/c++
#include <string>
#include <unordered_map>

// forward
class MainWnd;
class Control;


typedef struct WICFormatMeta_s
{
    IWICFormatConverter* Converter;
    uint32_t Frames;
    uint32_t Width;
    uint32_t Height;
} WICFormatMeta;


class Style
{
public:
    Style(
        CONTROL_STATUS Status, 
        D2D_RECT_F Position = ZERO_RECT,
        float RadiusX = 0,
        float RadiusY = 0
    );
    ~Style();

    // Handle paint event
    virtual void OnPaint();

    virtual void SetOwner(Control* Owner);

    virtual D2D_RECT_F Position();
    virtual void SetPosition(D2D_RECT_F Pos);

    virtual void SavePosition();

    virtual CONTROL_STATUS Status();

    virtual ID2D1SolidColorBrush* SolidBrush();
    virtual IDWriteTextFormat* TextFormat();
    virtual IDWriteTextLayout* TextLayout();
    virtual ID2D1Bitmap* D2DBitmap();
    virtual IWICFormatConverter* ConvertedSourceBitmap();

    static ID2D1SolidColorBrush* SolidBrush(D2D1_COLOR_F Color);
    static IDWriteTextFormat* TextFormat(
        const wchar_t* FontFamilyName, IDWriteFontCollection* FontCollection, DWRITE_FONT_WEIGHT FontWeight, 
        DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, FLOAT FontSize, const wchar_t* LocaleName,
        DWRITE_TEXT_ALIGNMENT TextAlignment, DWRITE_PARAGRAPH_ALIGNMENT ParagraphAlignment, bool NoWrapAndEllipsis = true
    );
    static IDWriteTextLayout* TextLayout(std::wstring Text, IDWriteTextFormat* Format, float MaxWidth, float MaxHeight);
    static WICFormatMeta ConvertedSourceBitmap(std::wstring Path);
    static ID2D1Bitmap* D2DBitmap(std::wstring Path, IWICFormatConverter* Converter);
    
    static void ReleaseResourceCache();

protected:
    Control* m_Owner;

    CONTROL_STATUS m_Status;

    D2D1_ROUNDED_RECT m_Position;
    D2D1_ROUNDED_RECT m_PositionPercentage;

    static MainWnd* s_Wnd;
    static HWND s_NativeWnd;
    static ID2D1Factory* s_D2DFactory;
    static IDWriteFactory* s_DWriteFactory;
    static ID2D1HwndRenderTarget* s_RenderTarget;
    static IWICImagingFactory* s_IWICFactory;
    static std::unordered_map<std::wstring, ID2D1SolidColorBrush*> s_SolidBrushCache;
    static std::unordered_map<std::wstring, IDWriteTextFormat*> s_TextFormatCache;
    static std::unordered_map<std::wstring, IDWriteTextLayout*> s_TextLayoutCache;
    static std::unordered_map<std::wstring, ID2D1Bitmap*> s_D2DBitmapCache;
    static std::unordered_map<std::wstring, WICFormatMeta> s_ConvertedSourceBitmapCache;
};

