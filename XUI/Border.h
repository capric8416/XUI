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

// forward
class MainWnd;



class Border : public Style
{
public:
    // Constructor
    Border(
        CONTROL_STATUS Status,

        D2D_RECT_F Position = ZERO_RECT,

        D2D1_COLOR_F Color = D2D1::ColorF(D2D1::ColorF::Black),
        D2D1_COLOR_F ClearColor = D2D1::ColorF(D2D1::ColorF::White),

        float RadiusX = 0,
        float RadiusY = 0,
        float StrokeWidth = 1.0f,
        ID2D1StrokeStyle* StrokeStyle = NULL
    );
    // Deconstructor
    ~Border();

    // Handle paint event
    virtual void OnPaint();


private:
    ID2D1SolidColorBrush* m_Brush;

    float m_StrokeWidth;
    ID2D1StrokeStyle* m_StrokeStyle;
};

