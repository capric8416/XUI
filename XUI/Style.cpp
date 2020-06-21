// self
#include "Style.h"

// project
#include "MainWnd.h"
#include "Control.h"

// c/c++
#include <sstream>

// namespace
using namespace std;


// static
MainWnd* Style::s_Wnd = nullptr;
HWND Style::s_NativeWnd = nullptr;
ID2D1Factory* Style::s_D2DFactory = nullptr;
IDWriteFactory* Style::s_DWriteFactory = nullptr;
IWICImagingFactory* Style::s_IWICFactory = nullptr;
ID2D1HwndRenderTarget* Style::s_RenderTarget = nullptr;
unordered_map<wstring, ID2D1SolidColorBrush*> Style::s_SolidBrushCache;
unordered_map<wstring, IDWriteTextFormat*> Style::s_TextFormatCache;
unordered_map<wstring, IDWriteTextLayout*> Style::s_TextLayoutCache;
unordered_map<wstring, ID2D1Bitmap*> Style::s_D2DBitmapCache;
unordered_map<wstring, WICFormatMeta> Style::s_ConvertedSourceBitmapCache;


Style::Style(
    CONTROL_STATUS Status,
    D2D_RECT_F Position,
    float RadiusX,
    float RadiusY
) :
    m_Owner(nullptr),
    m_Status(Status),
    m_PositionPercentage({ Position, RadiusX, RadiusY })
{
    if (s_Wnd == nullptr)
    {
        s_Wnd = MainWnd::Instance();
        s_NativeWnd = s_Wnd->NativeWnd();
        s_D2DFactory = s_Wnd->D2DFactory();
        s_DWriteFactory = s_Wnd->DWriteFactory();
        s_IWICFactory = s_Wnd->IWICFactory();
        s_RenderTarget = s_Wnd->RenderTarget();
    }
}


Style::~Style()
{
}


void Style::OnPaint()
{
}


void Style::SetOwner(Control* Owner)
{
    m_Owner = Owner;
}


D2D_RECT_F Style::Position()
{
    return m_Position.rect;
}


void Style::SetPosition(D2D_RECT_F Pos)
{
    m_Position.rect = Pos;
}


CONTROL_STATUS Style::Status()
{
    return m_Status;
}


ID2D1SolidColorBrush* Style::SolidBrush()
{
    return nullptr;
}


IDWriteTextFormat* Style::TextFormat()
{
    return nullptr;
}


IDWriteTextLayout* Style::TextLayout()
{
    return nullptr;
}


IWICFormatConverter* Style::ConvertedSourceBitmap()
{
    return nullptr;
}


ID2D1Bitmap* Style::D2DBitmap()
{
    return nullptr;
}


ID2D1SolidColorBrush* Style::SolidBrush(D2D1_COLOR_F Color)
{
    wstringstream tmp;
    tmp << Color.r << L" " << Color.g << L" " << Color.b << L" " << Color.a;
    wstring key = tmp.str();

    auto iter = s_SolidBrushCache.find(key);
    if (iter != s_SolidBrushCache.end() && iter->second != nullptr)
    {
        return iter->second;
    }

    ID2D1SolidColorBrush* brush;
    HRESULT hr = s_Wnd->RenderTarget()->CreateSolidColorBrush(Color, &brush);
    if (SUCCEEDED(hr))
    {
        s_SolidBrushCache[key] = brush;
        return brush;
    }

    return nullptr;
}


IDWriteTextFormat* Style::TextFormat(
    const wchar_t* FontFamilyName, IDWriteFontCollection* FontCollection, DWRITE_FONT_WEIGHT FontWeight, 
    DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, FLOAT FontSize, const wchar_t* LocaleName,
    DWRITE_TEXT_ALIGNMENT TextAlignment, DWRITE_PARAGRAPH_ALIGNMENT ParagraphAlignment, bool NoWrapAndEllipsis
)
{
    wstringstream tmp;
    tmp << FontFamilyName << L" " << FontCollection << L" " << FontWeight << L" " << FontStyle << L" " << FontStretch
        << L" " << FontSize << L" " << LocaleName << L" " << TextAlignment << L" " << ParagraphAlignment << L" " << NoWrapAndEllipsis;
    wstring key = tmp.str();

    auto iter = s_TextFormatCache.find(key);
    if (iter != s_TextFormatCache.end() && iter->second != nullptr)
    {
        return iter->second;
    }

    IDWriteTextFormat* format;
    HRESULT hr = s_DWriteFactory->CreateTextFormat(
        FontFamilyName, FontCollection, FontWeight, FontStyle,
        FontStretch, FontSize / s_Wnd->DpiScaleX(), LocaleName, &format
    );
    if (SUCCEEDED(hr))
    {
        hr = format->SetTextAlignment(TextAlignment);
        hr = format->SetParagraphAlignment(ParagraphAlignment);

        if (NoWrapAndEllipsis)
        {
            hr = format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

            DWRITE_TRIMMING trim1;
            trim1.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
            trim1.delimiter = 1;
            trim1.delimiterCount = 10;
            IDWriteInlineObject* trim2 = NULL;
            s_DWriteFactory->CreateEllipsisTrimmingSign(format, &trim2);
            hr = format->SetTrimming(&trim1, trim2);
        }

        s_TextFormatCache[key] = format;
        return format;
    }

    return nullptr;
}


IDWriteTextLayout* Style::TextLayout(wstring Text, IDWriteTextFormat* Format, float MaxWidth, float MaxHeight)
{
    wstringstream tmp;
    tmp << Text << L" " << Format << L" " << MaxWidth << L" " << MaxHeight;
    wstring key = tmp.str();

    auto iter = s_TextLayoutCache.find(key);
    if (iter != s_TextLayoutCache.end() && iter->second != nullptr)
    {
        return iter->second;
    }

    IDWriteTextLayout* layout;
    HRESULT hr = s_DWriteFactory->CreateTextLayout(Text.c_str(), Text.size(), Format, MaxWidth, MaxHeight, &layout);
    if (SUCCEEDED(hr))
    {
        s_TextLayoutCache[key] = layout;
        return layout;
    }

    return nullptr;
}


WICFormatMeta Style::ConvertedSourceBitmap(wstring Path)
{
    auto iter = s_ConvertedSourceBitmapCache.find(Path);
    if (iter != s_ConvertedSourceBitmapCache.end() && iter->second.Converter != nullptr)
    {
        return iter->second;
    }

    while (s_ConvertedSourceBitmapCache.size() > 6)
    {
        auto i = s_ConvertedSourceBitmapCache.begin();
        auto p = i->first;
        XSafeRelease(i->second.Converter);
        s_ConvertedSourceBitmapCache.erase(i);

        auto j = s_D2DBitmapCache.find(p);
        if (j != s_D2DBitmapCache.end())
        {
            XSafeRelease(j->second);
            s_D2DBitmapCache.erase(j);
        }
    }

    // Create a decoder
    IWICBitmapDecoder* Decoder = NULL;

    HRESULT hr = s_IWICFactory->CreateDecoderFromFilename(
        Path.c_str(),                    // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &Decoder                         // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* Frame = NULL;

    WICFormatMeta meta = {nullptr, 0, 0, 0 };
    if (SUCCEEDED(hr))
    {
        hr = Decoder->GetFrameCount(&meta.Frames);
        hr = Decoder->GetFrame(0, &Frame);
    }

    if (SUCCEEDED(hr))
    {
        hr = Frame->GetSize(&meta.Width, &meta.Height);
    }

    IWICFormatConverter* bitmap = nullptr;

    // Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        hr = s_IWICFactory->CreateFormatConverter(&bitmap);
    }

    if (SUCCEEDED(hr))
    {
        hr = bitmap->Initialize(
            Frame,                           // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (bitmap != nullptr)
    {
        meta.Converter = bitmap;
        s_ConvertedSourceBitmapCache[Path] = meta;
    }

    XSafeRelease(Decoder);
    XSafeRelease(Frame);

    return meta;
}


ID2D1Bitmap* Style::D2DBitmap(wstring Path, IWICFormatConverter* Converter)
{
    auto iter = s_D2DBitmapCache.find(Path);
    if (iter != s_D2DBitmapCache.end() && iter->second != nullptr)
    {
        return iter->second;
    }

    ID2D1Bitmap* bitmap;
    HRESULT hr = s_RenderTarget->CreateBitmapFromWicBitmap(Converter, NULL, &bitmap);

    if (SUCCEEDED(hr))
    {
        s_D2DBitmapCache[Path] = bitmap;
    }

    return bitmap;
}


void Style::ReleaseResourceCache()
{
    for (auto iter = s_SolidBrushCache.begin(); iter != s_SolidBrushCache.end(); iter++)
    {
        XSafeRelease(iter->second);
    }
    s_SolidBrushCache.clear();

    for (auto iter = s_TextFormatCache.begin(); iter != s_TextFormatCache.end(); iter++)
    {
        XSafeRelease(iter->second);
    }
    s_TextFormatCache.clear();

    for (auto iter = s_TextLayoutCache.begin(); iter != s_TextLayoutCache.end(); iter++)
    {
        XSafeRelease(iter->second);
    }
    s_TextLayoutCache.clear();

    for (auto iter = s_D2DBitmapCache.begin(); iter != s_D2DBitmapCache.end(); iter++)
    {
        XSafeRelease(iter->second);
    }
    s_D2DBitmapCache.clear();

    for (auto iter = s_ConvertedSourceBitmapCache.begin(); iter != s_ConvertedSourceBitmapCache.end(); iter++)
    {
        XSafeRelease(iter->second.Converter);
    }
    s_ConvertedSourceBitmapCache.clear();
}


void Style::SavePosition()
{
    if (m_PositionPercentage.rect.left < 0)
    {
        m_PositionPercentage.rect.left = 0;
    }
    if (m_PositionPercentage.rect.top < 0)
    {
        m_PositionPercentage.rect.top = 0;
    }
    if (m_PositionPercentage.rect.right > DENOMINATOR)
    {
        m_PositionPercentage.rect.right = DENOMINATOR;
    }
    if (m_PositionPercentage.rect.bottom > DENOMINATOR)
    {
        m_PositionPercentage.rect.bottom = DENOMINATOR;
    }
    if (m_PositionPercentage.radiusX < 0)
    {
        m_PositionPercentage.radiusX = 0;
    }
    if (m_PositionPercentage.radiusY < 0)
    {
        m_PositionPercentage.radiusY = 0;
    }
    if (m_PositionPercentage.radiusX > DENOMINATOR)
    {
        m_PositionPercentage.radiusX = DENOMINATOR;
    }
    if (m_PositionPercentage.radiusY > DENOMINATOR)
    {
        m_PositionPercentage.radiusY = DENOMINATOR;
    }

    auto ownerPos = m_Owner->DPIPosition();
    auto ownerWidth = ownerPos.right - ownerPos.left;
    auto ownerHeight = ownerPos.bottom - ownerPos.top;

    if (IS_ZERO_RECT(m_PositionPercentage.rect) || IS_FULL_RECT(m_PositionPercentage.rect))
    {
        m_PositionPercentage.rect.right = m_PositionPercentage.rect.bottom = DENOMINATOR;
        m_Position.rect = ownerPos;
    }
    else
    {
        m_Position.rect = {
           ownerPos.left + m_PositionPercentage.rect.left * ownerWidth / DENOMINATOR,
           ownerPos.top + m_PositionPercentage.rect.top * ownerHeight / DENOMINATOR,
           ownerPos.left + m_PositionPercentage.rect.right * ownerWidth / DENOMINATOR,
           ownerPos.top + m_PositionPercentage.rect.bottom * ownerHeight / DENOMINATOR
        };
    }

    m_Position.radiusX = m_PositionPercentage.radiusX * ownerWidth / DENOMINATOR;
    m_Position.radiusY = m_PositionPercentage.radiusY * ownerHeight / DENOMINATOR;
}
