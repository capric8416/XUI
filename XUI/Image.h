#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Style.h"

// c/c++
#include <stdint.h>


class Image : public Style
{
public:
	Image(CONTROL_STATUS Status, D2D_RECT_F Position = ZERO_RECT, std::wstring Path = L"");
	virtual ~Image();

	virtual void OnPaint();

	bool SetPath(std::wstring Path);

	virtual ID2D1Bitmap* D2DBitmap();
	virtual ID2D1Bitmap* D2DBitmap(std::wstring Path);
	virtual IWICFormatConverter* ConvertedSourceBitmap();
	virtual IWICFormatConverter* ConvertedSourceBitmap(std::wstring Path);

	static D2D_RECT_F Resize(float ImageWidth, float ImageHeight, D2D_RECT_F ContainerPosition);
	static uint8_t* EncodeToJpeg(std::wstring Path, uint32_t Width, uint32_t Height, uint32_t& Size, float Quality = 0.5f);

private:
	uint32_t m_Frames;
	uint32_t m_Width;
	uint32_t m_Height;
	std::wstring m_Path;
};