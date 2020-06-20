#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// project
#include "Control.h"
#include "Animation.h"

// define
#define LOADING_GEOMETRY_COUNT 2




class LoadingAnimation : public Animation
{
public:
    LoadingAnimation
    (
        MainWnd* Wnd,
        Control* Owner,
        CONTROL_STATUS Status,
        D2D_RECT_F Position,
        float CircleXRadiusDetla,
        float CircleYRadiusDetla,
        D2D1::ColorF OutLineColor = D2D1::ColorF(D2D1::ColorF::Black),
        D2D1::ColorF GradientStartColor = D2D1::ColorF(D2D1::ColorF::DarkGreen),
        D2D1::ColorF GradientStopColor = D2D1::ColorF(D2D1::ColorF::White),

        float RadiusX = 0,
        float RadiusY = 0
    );

    virtual ~LoadingAnimation();

    virtual void OnAnimated();


private:
    float m_HalfWidth;
    float m_HalfHeight;

    float m_TotalAngle;
    DWORD m_LastTime;
    DWORD m_CurrentTime;
    float m_TimeDelta;

    D2D1_ELLIPSE m_Ellipse[LOADING_GEOMETRY_COUNT];

    D2D1_GRADIENT_STOP m_GradientStops[LOADING_GEOMETRY_COUNT];

    ID2D1EllipseGeometry* m_EllipseArray[LOADING_GEOMETRY_COUNT];

    ID2D1SolidColorBrush* m_OutlineBrush;
    ID2D1RadialGradientBrush* m_GradientBrush;

    ID2D1GeometryGroup* m_GeometryGroup;

    ID2D1GradientStopCollection* m_GradientStopCollection;
};



class LoadingCircle : public Control
{
public:
    // Constructor
    LoadingCircle(
        std::wstring ID,
        RECT Position = ZERO_RECT,
        CONTROL_ALIGN HorizontalAlign = CONTROL_ALIGN_HORIZONTAL_CENTER,
        CONTROL_ALIGN VerticalAlign = CONTROL_ALIGN_VERTICAL_CENTER,
        RECT Margin = ZERO_RECT,
        RECT Padding = ZERO_RECT,
        bool Hidden = false,
        bool Disabled = false,
        bool Hintable = true,
        std::initializer_list<Background*> BackgroundStyle = {},
        std::initializer_list<Border*> BorderStyle = {},
        std::initializer_list<Text*> TextStyle = {},
        std::initializer_list<Image*> ImageStyle = {},
        std::initializer_list<Animation*> AnimationStyle = {}
    );
    // Deconstructor
    virtual ~LoadingCircle();


private:
    LoadingAnimation* m_Animation;
};