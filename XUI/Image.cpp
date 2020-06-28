// self
#include "Image.h"

// proejct
#include "Control.h"

// c/++
#include <assert.h>

// namespace
using namespace std;


#define MAX_BUF_LEN 4096 * 1024


Image::Image(CONTROL_STATUS Status, D2D_RECT_F Position, wstring Path)
	: Style(Status, Position)
    , m_Frames(0)
    , m_Width(0)
    , m_Height(0)
	, m_Path(Path)
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
        D2D_RECT_F pos = Resize(m_Width, m_Height, m_Position.rect);
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

    return __super::D2DBitmap(m_Path, ConvertedSourceBitmap());
}


ID2D1Bitmap* Image::D2DBitmap(wstring Path)
{
    if (Path.empty())
    {
        return nullptr;
    }

    return __super::D2DBitmap(Path, ConvertedSourceBitmap(Path));
}


IWICFormatConverter* Image::ConvertedSourceBitmap()
{
    if (m_Path.empty())
    {
        return nullptr;
    }

    WICFormatMeta meta = __super::ConvertedSourceBitmap(m_Path, m_Owner->DPIPosition());
    
    m_Frames = meta.Frames;
    m_Width = meta.Width;
    m_Height = meta.Height;

    return meta.Converter;
}


IWICFormatConverter* Image::ConvertedSourceBitmap(wstring Path)
{
    if (Path.empty())
    {
        return nullptr;
    }

    WICFormatMeta meta = __super::ConvertedSourceBitmap(Path, m_Owner->DPIPosition());

    m_Frames = meta.Frames;
    m_Width = meta.Width;
    m_Height = meta.Height;

    return meta.Converter;
}


D2D_RECT_F Image::Resize(float ImageWidth, float ImageHeight, D2D_RECT_F ContainerPosition)
{
    auto containerWidth = ContainerPosition.right - ContainerPosition.left;
    auto containerHeight = ContainerPosition.bottom - ContainerPosition.top;
    float containerRatio = containerHeight / containerWidth;

    float imageWidth = ImageWidth;
    float imageHeight = ImageHeight;
    float imageRatio = imageHeight / imageWidth;
    if (imageRatio > 1.0001f)  // v
    {
        if (ImageHeight > containerHeight)
        {
            float r = containerHeight / ImageHeight;
            imageHeight = containerHeight;
            imageWidth *= r;
            if (imageWidth > containerWidth)
            {
                r = containerWidth / imageWidth;
                imageWidth = containerWidth;
                imageHeight *= r;
            }
        }
    }
    else if (imageRatio < 1.0001f)  // h
    {
        if (ImageWidth > containerWidth)
        {
            float r = containerWidth / ImageWidth;
            imageWidth = containerWidth;
            imageHeight *= r;
            if (imageHeight > containerHeight)
            {
                r = containerHeight / imageHeight;
                imageHeight = containerHeight;
                imageWidth *= r;
            }
        }
    }

    float x = ContainerPosition.left + (float)(containerWidth - imageWidth) / 2;
    float y = ContainerPosition.top + (float)(containerHeight - imageHeight) / 2;
    return D2D_RECT_F{ x, y, x + imageWidth, y + imageHeight };
}


uint32_t FindJpegEndPos(uint8_t* Buffer, uint32_t Last)
{
    uint32_t offset = Last / 2;
    if (Buffer[offset] == 0)
    {
        return FindJpegEndPos(Buffer, offset);
    }
    else
    {
        uint8_t end[2] = { 0xFF, 0xD9 };
        for (uint32_t i = offset - 1; i < Last;)
        {
            if (memcmp(Buffer + i, end, 2) == 0)
            {
                return i + 2;
            }

            if (Buffer[i + 1] == 0xFF)
            {
                i++;
            }
            else
            {
                i += 2;
            }
        }
    }
}


uint8_t* Image::EncodeToJpeg(std::wstring Path, uint32_t Width, uint32_t Height, uint32_t& Size, float Quality)
{
    // Create a decoder
    IWICBitmapDecoder* decoder = NULL;
    HRESULT hr = s_IWICFactory->CreateDecoderFromFilename(
        Path.c_str(),                    // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &decoder                         // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    uint32_t count = 0;
    IWICBitmapFrameDecode* decodeFrame = NULL;

    if (SUCCEEDED(hr))
    {
        hr = decoder->GetFrameCount(&count);
        hr = decoder->GetFrame(0, &decodeFrame);
    }

    if (count > 1)
    {
        Size = 0;
        return nullptr;
    }

    uint32_t imageWidth = 0;
    uint32_t imageHeight = 0;
    if (SUCCEEDED(hr))
    {
        hr = decodeFrame->GetSize(&imageWidth, &imageHeight);
    }

    D2D_RECT_F pos = Resize(imageWidth, imageHeight, { 0, 0, (float)Width, (float)Height });

    // Create a decoder
    IWICBitmapEncoder* encoder = NULL;
    if (SUCCEEDED(hr))
    {
        hr = s_IWICFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &encoder);
    }

    uint8_t* jpegBuffer = (uint8_t*)calloc(MAX_BUF_LEN, sizeof(uint8_t));

    // Create a stream
    IWICStream* stream = NULL;
    if (SUCCEEDED(hr))
    {
        hr = s_IWICFactory->CreateStream(&stream);
        hr = stream->InitializeFromMemory((WICInProcPointer)jpegBuffer, MAX_BUF_LEN);
        hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
    }

    IPropertyBag2* propertybag = NULL;
    IWICBitmapFrameEncode* encodeFrame = NULL;
    if (SUCCEEDED(hr))
    {
        hr = encoder->CreateNewFrame(&encodeFrame, &propertybag);
    }

    // Create a BitmapScaler
    IWICBitmapScaler* scaler = NULL;
    if (SUCCEEDED(hr))
    {
        hr = s_IWICFactory->CreateBitmapScaler(&scaler);
    }

    // Initialize the bitmap scaler from the original bitmap map bits
    if (SUCCEEDED(hr))
    {
        hr = scaler->Initialize(
            decodeFrame,
            (uint32_t)(pos.right - pos.left),
            (uint32_t)(pos.bottom - pos.top),
            WICBitmapInterpolationModeFant
        );
    }

    IWICFormatConverter* converter = NULL;
    if (SUCCEEDED(hr))
    {
        hr = s_IWICFactory->CreateFormatConverter(&converter);
    }

    // Format convert to 32bppBGR 
    if (SUCCEEDED(hr))
    {
        hr = converter->Initialize(
            scaler,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppBGR,     // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither patterm
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    PROPBAG2 propBag2 = { 0 };
    VARIANT variant;
    propBag2.pstrName = (LPOLESTR)L"ImageQuality";
    VariantInit(&variant);
    variant.vt = VT_R4;
    variant.fltVal = Quality;
    if (SUCCEEDED(hr))
    {
        hr = propertybag->Write(1, &propBag2, &variant);
    }

    if (SUCCEEDED(hr))
    {
        hr = encodeFrame->Initialize(propertybag);
    }
    if (SUCCEEDED(hr))
    {
        hr = encodeFrame->WriteSource(converter, NULL);
    }
    if (SUCCEEDED(hr))
    {
        hr = encodeFrame->Commit();
    }

    if (SUCCEEDED(hr))
    {
        hr = encoder->Commit();
    }

    XSafeRelease(decoder);
    XSafeRelease(encoder);
    XSafeRelease(decodeFrame);
    XSafeRelease(encodeFrame);
    XSafeRelease(propertybag);
    XSafeRelease(stream);
    XSafeRelease(scaler);
    XSafeRelease(converter);

    Size = FindJpegEndPos(jpegBuffer, MAX_BUF_LEN);
    if (MAX_BUF_LEN > Size)
    {
        realloc(jpegBuffer, Size * sizeof(uint8_t));
    }

    return jpegBuffer;
}
