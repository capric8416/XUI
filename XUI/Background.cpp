// self
#include "Background.h"

// project
#include "MainWnd.h"
#include "Control.h"



Background::Background(
    CONTROL_STATUS Status,

    D2D_RECT_F Position,

    D2D1_COLOR_F Color,

    float RadiusX,
    float RadiusY
) :
    Style(
        Status,
        Position,
        RadiusX,
        RadiusY
    ),
    m_Color(Color),
    m_Brush(nullptr)
{
    SolidBrush();
}


Background::~Background()
{
}


void Background::OnPaint(bool Clear)
{
    if (Clear || m_Owner->Hidden())
    {
        D2D1_ROUNDED_RECT rect{
            m_Owner->DPIPosition(),
            m_Position.radiusX,
            m_Position.radiusY
        };

        if (m_Owner->Hidden())
        {
            rect.rect.left -= 1;
            rect.rect.right += 1;
            rect.rect.top -= 1;
            rect.rect.bottom += 1;
        }

        s_RenderTarget->FillRoundedRectangle(&rect, __super::SolidBrush(D2D1::ColorF(D2D1::ColorF::White)));
    }

    if (!m_Owner->Hidden())
    {
        s_RenderTarget->FillRoundedRectangle(&m_Position, SolidBrush());
    }
}


ID2D1SolidColorBrush* Background::SolidBrush()
{
    if (m_Brush != nullptr)
    {
        return m_Brush;
    }

    m_Brush = __super::SolidBrush(m_Color);
    return m_Brush;
}
