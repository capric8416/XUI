// self
#include "Border.h"

// project
#include "MainWnd.h"
#include "Control.h"



Border::Border(
    CONTROL_STATUS Status,

    D2D_RECT_F Position,

    D2D1_COLOR_F Color,

    float RadiusX,
    float RadiusY,
    float StrokeWidth,
    ID2D1StrokeStyle* StrokeStyle
) :
    Style(
        Status,
        Position
    ),
    m_Color(Color),
    m_StrokeWidth(StrokeWidth),
    m_StrokeStyle(StrokeStyle)
{
    SolidBrush();
}


Border::~Border()
{
}


void Border::OnPaint()
{
    if (!m_Owner->Hidden())
    {
        s_RenderTarget->DrawRoundedRectangle(&m_Position, SolidBrush(), m_StrokeWidth, m_StrokeStyle);
    }
}


ID2D1SolidColorBrush* Border::SolidBrush()
{
    return __super::SolidBrush(m_Color);
}
