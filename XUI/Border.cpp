// self
#include "Border.h"

// project
#include "MainWnd.h"
#include "Control.h"



Border::Border(
    CONTROL_STATUS Status,

    D2D_RECT_F Position,

    D2D1_COLOR_F Color,
    D2D1_COLOR_F ClearColor,

    float RadiusX,
    float RadiusY,
    float StrokeWidth,
    ID2D1StrokeStyle* StrokeStyle
) :
    Style(
        Status,
        Position,
        ClearColor
    )
{
    m_StrokeWidth = StrokeWidth;
    m_StrokeStyle = StrokeStyle;

    HRESULT hr = S_OK;

    // Create brush
    hr = m_RenderTarget->CreateSolidColorBrush(Color, &m_Brush);
}


Border::~Border()
{
    XSafeRelease(&m_Brush);
}


void Border::OnPaint()
{
    m_RenderTarget->DrawRoundedRectangle(&m_Position, m_ClearBrush, m_StrokeWidth, m_StrokeStyle);

    if (!m_Owner->Hidden())
    {
        m_RenderTarget->DrawRoundedRectangle(&m_Position, m_Brush, m_StrokeWidth, m_StrokeStyle);
    }
}