// self
#include "LoadingCircle.h"

// project
#include "Common.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"
#include "MainWnd.h"

// namespace
using namespace std;



LoadingAnimation::LoadingAnimation(
    MainWnd* Wnd,
    Control* Owner,
    CONTROL_STATUS Status,
    D2D_RECT_F Position,
    float CircleXRadiusDetla,
    float CircleYRadiusDetla,
    D2D1::ColorF OutLineColor,
    D2D1::ColorF GradientStartColor,
    D2D1::ColorF GradientStopColor,
    float RadiusX,
    float RadiusY
) :
    Animation(Status, Position, RadiusX, RadiusY),
    m_OutlineBrush(nullptr),
    m_GradientBrush(nullptr),
    m_GeometryGroup(nullptr),
    m_GradientStopCollection(nullptr),
    m_TotalAngle(0),
    m_LastTime(TICK_COUNT),
    m_CurrentTime(TICK_COUNT)
{
    HRESULT hr = s_Wnd->RenderTarget()->CreateSolidColorBrush(D2D1::ColorF(OutLineColor), &m_OutlineBrush);

    m_HalfWidth = (m_Position.rect.right - m_Position.rect.left) / 2;
    m_HalfHeight = (m_Position.rect.bottom - m_Position.rect.top) / 2;

    m_Ellipse[0] = D2D1::Ellipse(
        D2D1::Point2F(
            m_Position.rect.left + m_HalfWidth,
            m_Position.rect.top + m_HalfHeight
        ),
        m_HalfWidth - CircleXRadiusDetla,
        m_HalfHeight - CircleYRadiusDetla
    );
    m_Ellipse[1] = D2D1::Ellipse(
        D2D1::Point2F(
            m_Position.rect.left + m_HalfWidth,
            m_Position.rect.top + m_HalfHeight
        ),
        m_HalfWidth,
        m_HalfHeight
    );

    m_GradientStops[0].color = D2D1::ColorF(GradientStartColor);
    m_GradientStops[0].position = 0.f;
    m_GradientStops[1].color = D2D1::ColorF(GradientStopColor);
    m_GradientStops[1].position = 1.f;

    hr = s_Wnd->RenderTarget()->CreateGradientStopCollection(
        m_GradientStops,
        LOADING_GEOMETRY_COUNT,
        D2D1_GAMMA_2_2,
        D2D1_EXTEND_MODE_CLAMP,
        &m_GradientStopCollection
    );

    hr = s_Wnd->RenderTarget()->CreateRadialGradientBrush(
        D2D1::RadialGradientBrushProperties(
            D2D1::Point2F(
                m_Position.rect.left + m_HalfWidth - (m_Position.rect.right - m_Position.rect.left) / 3,
                m_Position.rect.top + m_HalfHeight - (m_Position.rect.bottom - m_Position.rect.top) / 3
            ),
            D2D1::Point2F(0, 0),
            m_HalfWidth,
            m_HalfHeight
        ),
        m_GradientStopCollection,
        &m_GradientBrush
    );

    for (int i = 0; i < LOADING_GEOMETRY_COUNT; i++)
    {
        hr = s_Wnd->D2DFactory()->CreateEllipseGeometry(m_Ellipse[i], &m_EllipseArray[i]);
    }

    hr = s_Wnd->D2DFactory()->CreateGeometryGroup(
        D2D1_FILL_MODE_ALTERNATE,
        (ID2D1Geometry**)&m_EllipseArray,
        ARRAYSIZE(m_EllipseArray),
        &m_GeometryGroup
    );
}


LoadingAnimation::~LoadingAnimation()
{
    XSafeRelease(&m_OutlineBrush);
    XSafeRelease(&m_GradientBrush);
    XSafeRelease(&m_GeometryGroup);
    XSafeRelease(&m_GradientStopCollection);

    for (int i = 0; i < LOADING_GEOMETRY_COUNT; i++)
    {
        XSafeRelease(&m_EllipseArray[i]);
    }
}


void LoadingAnimation::OnAnimated()
{
    // Get current time
    m_CurrentTime = TICK_COUNT;

    // Calculate time elapsed in current frame.
    m_TimeDelta = (float)(m_CurrentTime - m_LastTime) * 0.1;

    // Increase the m_TotalAngle by the time elapsed in current frame.
    m_TotalAngle += m_TimeDelta;

    // Draw geometry group
    s_Wnd->RenderTarget()->DrawGeometry(m_GeometryGroup, m_OutlineBrush);

    // Roatate the gradient brush based on the total elapsed time
    D2D1_MATRIX_3X2_F rotMatrix = D2D1::Matrix3x2F::Rotation(
        m_TotalAngle,
        D2D1::Point2F(
            m_Position.rect.left + m_HalfWidth,
            m_Position.rect.top + m_HalfHeight
        )
    );
    m_GradientBrush->SetTransform(rotMatrix);

    // Fill geometry group with the transformed brush
    s_Wnd->RenderTarget()->FillGeometry(m_GeometryGroup, m_GradientBrush);

    // Update last time to current time for next loop
    m_LastTime = m_CurrentTime;
}



LoadingCircle::LoadingCircle(
    wstring ID,
    RECT Position,
    CONTROL_ALIGN HorizontalAlign,
    CONTROL_ALIGN VerticalAlign,
    RECT Margin,
    RECT Padding,
    bool Hidden,
    bool Disabled,
    bool Hintable,
    initializer_list<Background*> BackgroundStyle,
    initializer_list<Border*> BorderStyle,
    initializer_list<Text*> TextStyle,
    initializer_list<Animation*> AnimationStyle
) :
    Control(
        ID,
        Position,
        Hidden,
        Disabled,
        HorizontalAlign,
        VerticalAlign,
        Margin,
        Padding,
        Hintable,
        {},
        BackgroundStyle,
        BorderStyle,
        TextStyle,
        AnimationStyle
    )
{
    //Style(
    //    CONTROL_STATUS_NORMAL,
    //    new LoadingAnimation(
    //        m_Wnd,
    //        this,
    //        m_DPIPosition,
    //        5,
    //        5,
    //        D2D1::ColorF(0x009688)
    //    )
    //);
}


LoadingCircle::~LoadingCircle()
{
    XSafeDelete(m_Animation);
}

