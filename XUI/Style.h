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

// forward
class MainWnd;
class Control;



class Style
{
public:
    Style(
        CONTROL_STATUS Status, 
        D2D_RECT_F Position = ZERO_RECT,
        D2D1_COLOR_F ClearColor = D2D1::ColorF(D2D1::ColorF::White),
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


protected:
    MainWnd* m_Wnd;
    Control* m_Owner;

    CONTROL_STATUS m_Status;

    D2D1_ROUNDED_RECT m_Position;
    D2D1_ROUNDED_RECT m_PositionPercentage;

    ID2D1SolidColorBrush* m_ClearBrush;

    ID2D1Factory* m_D2DFactory;
    IDWriteFactory* m_DWriteFactory;
    ID2D1HwndRenderTarget* m_RenderTarget;
};

