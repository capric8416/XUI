// self
#include "Image.h"

// proejct
#include "Control.h"

// c/++
#include <assert.h>

// namespace
using namespace std;


Image::Image(CONTROL_STATUS Status, D2D_RECT_F Position, wstring Path)
	: Style(Status, Position)
    , m_Frames(0)
    , m_Width(0)
    , m_Height(0)
	, m_Path(Path)
	, m_D2DBitmap(nullptr)
	, m_ConvertedSourceBitmap(nullptr)
{
    D2DBitmap();
}


Image::~Image()
{
}


void Image::OnPaint()
{
    if (!m_Owner->Hidden() && D2DBitmap())
    {
        auto ownerWidth = m_Position.rect.right - m_Position.rect.left;
        auto ownerHeight = m_Position.rect.bottom - m_Position.rect.top;
        float ownerRatio = ownerHeight / ownerWidth;
        
        float imageWidth = m_Width;
        float imageHeight = m_Height;
        float imageRatio = imageHeight / imageWidth;
        if (imageRatio > 1.0001f)  // v
        {
            if (m_Height > ownerHeight)
            {
                float r = ownerHeight / m_Height;
                imageHeight = ownerHeight;
                imageWidth *= r;
                if (imageWidth > ownerWidth)
                {
                    r = ownerWidth / imageWidth;
                    imageWidth = ownerWidth;
                    imageHeight *= r;
                }
            }
        }
        else if (imageRatio < 1.0001f)  // h
        {
            if (m_Width > ownerWidth)
            {
                float r = ownerWidth / m_Width;
                imageWidth = ownerWidth;
                imageHeight *= r;
                if (imageHeight > ownerHeight)
                {
                    r = ownerHeight / imageHeight;
                    imageHeight = ownerHeight;
                    imageWidth *= r;
                }
            }
        }

        float x = m_Position.rect.left + (float)(ownerWidth - imageWidth) / 2;
        float y = m_Position.rect.top + (float)(ownerHeight - imageHeight) / 2;
        D2D_RECT_F pos{ x, y, x + imageWidth, y + imageHeight};

        s_RenderTarget->DrawBitmap(D2DBitmap(), pos);
    }
}


bool Image::SetPath(wstring Path)
{
    if (Path == m_Path)
    {
        return false;
    }

    D2DBitmap(Path);

    m_Path = Path;

    return true;
}


ID2D1Bitmap* Image::D2DBitmap()
{
    if (m_Path.empty())
    {
        return nullptr;
    }

    if (m_D2DBitmap != nullptr)
    {
        return m_D2DBitmap;
    }

    m_D2DBitmap = __super::D2DBitmap(m_Path, ConvertedSourceBitmap());
    return m_D2DBitmap;
}


ID2D1Bitmap* Image::D2DBitmap(wstring Path)
{
    if (Path.empty())
    {
        return nullptr;
    }

    if (m_D2DBitmap != nullptr && Path == m_Path)
    {
        return m_D2DBitmap;
    }

    m_D2DBitmap = __super::D2DBitmap(Path, ConvertedSourceBitmap(Path));
    return m_D2DBitmap;
}


IWICFormatConverter* Image::ConvertedSourceBitmap()
{
    if (m_ConvertedSourceBitmap != nullptr)
    {
        return m_ConvertedSourceBitmap;
    }

    WICFormatMeta meta = __super::ConvertedSourceBitmap(m_Path);
    
    m_ConvertedSourceBitmap = meta.Converter;
    m_Frames = meta.Frames;
    m_Width = meta.Width;
    m_Height = meta.Height;

    return m_ConvertedSourceBitmap;
}


IWICFormatConverter* Image::ConvertedSourceBitmap(wstring Path)
{
    if (m_ConvertedSourceBitmap != nullptr && Path == m_Path)
    {
        return m_ConvertedSourceBitmap;
    }

    WICFormatMeta meta = __super::ConvertedSourceBitmap(Path);

    m_ConvertedSourceBitmap = meta.Converter;
    m_Frames = meta.Frames;
    m_Width = meta.Width;
    m_Height = meta.Height;

    return m_ConvertedSourceBitmap;
}
