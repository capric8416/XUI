// self
#include "Background.h"

// project
#include "MainWnd.h"
#include "Control.h"



Background::Background(
    CONTROL_STATUS Status,

    D2D_RECT_F Position,

    D2D1_COLOR_F Color,
    D2D1_COLOR_F ClearColor,

    float RadiusX,
    float RadiusY
) :
    Style(
        Status,
        Position,
        ClearColor,
        RadiusX,
        RadiusY
    )
{
    HRESULT hr = S_OK;

    // Create brush
    hr = m_RenderTarget->CreateSolidColorBrush(Color, &m_Brush);
}


Background::~Background()
{
    XSafeRelease(&m_Brush);
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

        m_RenderTarget->FillRoundedRectangle(&rect, m_ClearBrush);
    }

    if (!m_Owner->Hidden())
    {
        m_RenderTarget->FillRoundedRectangle(&m_Position, m_Brush);
    }
}
