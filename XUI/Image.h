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
	~Image();

	virtual void OnPaint();

	bool SetPath(std::wstring Path);

	virtual ID2D1Bitmap* D2DBitmap();
	virtual ID2D1Bitmap* D2DBitmap(std::wstring Path);
	virtual IWICFormatConverter* ConvertedSourceBitmap();
	virtual IWICFormatConverter* ConvertedSourceBitmap(std::wstring Path);

private:
	uint32_t m_Frames;
	uint32_t m_Width;
	uint32_t m_Height;
	std::wstring m_Path;
	ID2D1Bitmap* m_D2DBitmap;
	IWICFormatConverter* m_ConvertedSourceBitmap;
};