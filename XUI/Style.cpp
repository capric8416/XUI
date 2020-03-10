// self
#include "Style.h"

// project
#include "MainWnd.h"
#include "Control.h"



Style::Style(
    CONTROL_STATUS Status,
    D2D_RECT_F Position,
    D2D1_COLOR_F ClearColor,
    float RadiusX,
    float RadiusY
) :
    m_Wnd(MainWnd::Instance()),
    m_Owner(nullptr),
    m_Status(Status),
    m_PositionPercentage({ Position, RadiusX, RadiusY })
{
    m_D2DFactory = m_Wnd->D2DFactory();
    m_DWriteFactory = m_Wnd->DWriteFactory();
    m_RenderTarget = m_Wnd->RenderTarget();

    HRESULT hr = S_OK;

    // Create brush
    hr = m_Wnd->RenderTarget()->CreateSolidColorBrush(ClearColor, &m_ClearBrush);
}


Style::~Style()
{
    XSafeRelease(&m_ClearBrush);
}


void Style::OnPaint()
{
}


void Style::SetOwner(Control* Owner)
{
    m_Owner = Owner;
}


D2D_RECT_F Style::Position()
{
    return m_Position.rect;
}


void Style::SetPosition(D2D_RECT_F Pos)
{
    m_Position.rect = Pos;
}


CONTROL_STATUS Style::Status()
{
    return m_Status;
}


void Style::SavePosition()
{
    if (m_PositionPercentage.rect.left < 0)
    {
        m_PositionPercentage.rect.left = 0;
    }
    if (m_PositionPercentage.rect.top < 0)
    {
        m_PositionPercentage.rect.top = 0;
    }
    if (m_PositionPercentage.rect.right > DENOMINATOR)
    {
        m_PositionPercentage.rect.right = DENOMINATOR;
    }
    if (m_PositionPercentage.rect.bottom > DENOMINATOR)
    {
        m_PositionPercentage.rect.bottom = DENOMINATOR;
    }
    if (m_PositionPercentage.radiusX < 0)
    {
        m_PositionPercentage.radiusX = 0;
    }
    if (m_PositionPercentage.radiusY < 0)
    {
        m_PositionPercentage.radiusY = 0;
    }
    if (m_PositionPercentage.radiusX > DENOMINATOR)
    {
        m_PositionPercentage.radiusX = DENOMINATOR;
    }
    if (m_PositionPercentage.radiusY > DENOMINATOR)
    {
        m_PositionPercentage.radiusY = DENOMINATOR;
    }

    auto ownerPos = m_Owner->DPIPosition();
    auto ownerWidth = ownerPos.right - ownerPos.left;
    auto ownerHeight = ownerPos.bottom - ownerPos.top;

    if (IS_ZERO_RECT(m_PositionPercentage.rect) || IS_FULL_RECT(m_PositionPercentage.rect))
    {
        m_PositionPercentage.rect.right = m_PositionPercentage.rect.bottom = DENOMINATOR;
        m_Position.rect = ownerPos;
    }
    else
    {
        m_Position.rect = {
           ownerPos.left + m_PositionPercentage.rect.left * ownerWidth / DENOMINATOR,
           ownerPos.top + m_PositionPercentage.rect.top * ownerHeight / DENOMINATOR,
           ownerPos.left + m_PositionPercentage.rect.right * ownerWidth / DENOMINATOR,
           ownerPos.top + m_PositionPercentage.rect.bottom * ownerHeight / DENOMINATOR
        };
    }

    m_Position.radiusX = m_PositionPercentage.radiusX * ownerWidth / DENOMINATOR;
    m_Position.radiusY = m_PositionPercentage.radiusY * ownerHeight / DENOMINATOR;
}
