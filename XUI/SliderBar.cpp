// self
#include "SliderBar.h"

// project
#include "Common.h"
#include "MainWnd.h"
#include "Background.h"
#include "Border.h"
#include "Text.h"
#include "Label.h"

// c++
#include <cmath>

// namespace
using namespace std;



SliderBarBody::SliderBarBody(
    wstring ID,
    RECT Position,
    INT32 Value,
    INT32 MinValue,
    INT32 MaxValue,
    INT32 StepValue,
    bool Vertical,
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
    ),
    m_Value(Value),
    m_MinValue(MinValue),
    m_MaxValue(MaxValue),
    m_StepValue(StepValue),
    m_Vertical(Vertical)
{
}


SliderBarBody::~SliderBarBody()
{
}


void SliderBarBody::OnLeftButtonClick(LONG X, LONG Y)
{
    __super::OnLeftButtonClick(X, Y);

    UpdateValue(X, Y);
}


void SliderBarBody::OnMouseDrag(POINT Pt, POINT LastPt)
{
    __super::OnMouseDrag(Pt, LastPt);

    UpdateValue(Pt.x, Pt.y);
}


void SliderBarBody::UpdateValue(LONG X, LONG Y, bool Init, bool Paint)
{
    bool init = false;

    D2D_RECT_F oldPos;
    D2D_RECT_F newPos;

    float offset;
    float count;

    INT32 value;

    bool unchanged = false;

    for (int i = CONTROL_STATUS_BTIV + 1; i < CONTROL_STATUS_SIZE; i++)
    {
        for (const auto& style : m_BackgroundStyles[i])
        {
            if (!init)
            {
                init = true;

                oldPos = style->Position();

                if (!m_Vertical)
                {
                    count = m_DPIPosition.right - m_DPIPosition.left;

                    if (Init)
                    {
                        newPos = { oldPos.left, oldPos.top, oldPos.left + count * m_Value / m_MaxValue, oldPos.bottom };
                    }
                    else
                    {
                        float right = (float)X / m_Wnd->DpiScaleX();
                        value = m_MaxValue * (right - oldPos.left) / count;
                        if (value < m_MinValue)
                        {
                            value = m_MinValue;
                        }
                        else if (value > m_MaxValue)
                        {
                            value = m_MaxValue;
                        }
                        else
                        {
                            INT32 multiple = value / m_StepValue;
                            if (value - multiple * m_StepValue >= m_StepValue / 2)
                            {
                                value = (multiple + 1) * m_StepValue;
                            }
                            else
                            {
                                value = multiple * m_StepValue;
                            }
                        }
                        right = oldPos.left + count * value / m_MaxValue;

                        unchanged = value == m_Value;
                        m_Value = value;

                        newPos = { oldPos.left, oldPos.top, right, oldPos.bottom };
                    }

                    offset = newPos.right - newPos.left;
                }
                else
                {
                    count = m_DPIPosition.bottom - m_DPIPosition.top;

                    if (Init)
                    {
                        newPos = { oldPos.left, oldPos.bottom - count * m_Value / m_MaxValue, oldPos.right, oldPos.bottom };
                    }
                    else
                    {
                        float top = (float)Y / m_Wnd->DpiScaleY();
                        value = m_MaxValue * (oldPos.bottom - top) / count;
                        if (value < m_MinValue)
                        {
                            value = m_MinValue;
                        }
                        else if (value > m_MaxValue)
                        {
                            value = m_MaxValue;
                        }
                        else
                        {
                            INT32 multiple = value / m_StepValue;
                            if (value - multiple * m_StepValue >= m_StepValue / 2)
                            {
                                value = (multiple + 1) * m_StepValue;
                            }
                            else
                            {
                                value = multiple * m_StepValue;
                            }
                        }
                        top = oldPos.bottom - count * value / m_MaxValue;

                        unchanged = value == m_Value;
                        m_Value = value;

                        newPos = { oldPos.left, top, oldPos.right, oldPos.bottom };
                    }

                    offset = newPos.bottom - newPos.top;
                }
            }

            if (!unchanged)
            {
                style->SetPosition(newPos);
            }
        }

        if (unchanged)
        {
            break;
        }
    }

    if (Paint && !Init && !unchanged)
    {
        ((SliderBar*)m_Parent)->OnValueChange(value);

        Invalidate();
    }
}


void SliderBarBody::SavePosition(UINT16 Index)
{
    __super::SavePosition(Index);

    UpdateValue(0, 0, true);
}


void SliderBarBody::Value(INT32 Value)
{
    m_Value = Value;
}



bool SliderBarBody::SetMin(INT32 Value)
{
    if (m_MinValue == Value)
    {
        return false;
    }

    m_MinValue = Value;
    return true;
}


bool SliderBarBody::SetMax(INT32 Value)
{
    if (m_MaxValue == Value)
    {
        return false;
    }

    m_MaxValue = Value;
    return true;
}


SliderBar::SliderBar(
    wstring ID,
    RECT Position,
    INT32 Value,
    INT32 MinValue,
    INT32 MaxValue,
    INT32 StepValue,
    bool Vertical,
    SliderBarLabel* TitleLabel,
    SliderBarBody* SliderBody,
    SliderBarLabel* ValueLabel,
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
    { TitleLabel, SliderBody, ValueLabel },
    BackgroundStyle,
    BorderStyle,
    TextStyle,
    AnimationStyle
),
m_SliderLabel(TitleLabel),
m_SliderValue(ValueLabel),
m_SliderBody(SliderBody),
m_Value(Value),
m_MinValue(MinValue),
m_MaxValue(MaxValue),
m_StepValue(StepValue),
m_Vertical(Vertical),
m_Formatter(nullptr)
{
}


SliderBar::~SliderBar()
{
    XSafeDelete(m_SliderLabel);
    XSafeDelete(m_SliderValue);
    XSafeDelete(m_SliderBody);
}


void SliderBar::OnValueChange(INT32 Value)
{
    m_Wnd->GetUI()->OnSliderValueChanged(this, m_SliderBody, m_Value, Value);

    m_Value = Value;
    
    if (m_Formatter != nullptr)
    {
        m_SliderValue->SetContent(m_Formatter(m_Value));
    }
    else
    {
        m_SliderValue->SetContent(to_wstring(m_Value));
    }
}


INT32 SliderBar::Value()
{
    return m_Value;
}


bool SliderBar::Value(INT32 Value)
{
    if (Value != m_Value && m_MinValue <= Value && Value <= m_MaxValue)
    {
        m_Value = Value;

        bool paint = !m_Hidden;

        if (m_Formatter != nullptr)
        {
            m_SliderValue->SetContent(m_Formatter(m_Value));
        }
        else
        {
            m_SliderValue->SetContent(to_wstring(m_Value));
        }

        m_SliderBody->Value(Value);
        m_SliderBody->UpdateValue(0, 0, true);

        if (paint)
        {
            Invalidate();
        }

        return true;
    }

    return false;
}


void SliderBar::Min()
{
    Value(m_MinValue);
}


INT32 SliderBar::GetMin()
{
    return m_MinValue;
}


bool SliderBar::SetMin(INT32 Value)
{
    if (m_MinValue == Value)
    {
        return false;
    }

    m_MinValue = Value;
    m_SliderBody->SetMin(Value);
    return true;
}


void SliderBar::Max()
{
    Value(m_MaxValue);
}


INT32 SliderBar::GetMax()
{
    return m_MaxValue;
}


bool SliderBar::SetMax(INT32 Value)
{
    if (m_MaxValue == Value)
    {
        return false;
    }

    m_MaxValue = Value;
    m_SliderBody->SetMax(Value);
    return true;
}


bool SliderBar::SetTitle(wstring Text)
{
    return m_SliderLabel->SetContent(Text);
}


bool SliderBar::SetTitle(INT32 Value)
{
    if (m_Formatter != nullptr)
    {
        return m_SliderLabel->SetContent(m_Formatter(Value));
    }
    else
    {
        return m_SliderLabel->SetContent(to_wstring(Value));
    }
}


void SliderBar::SetFormater(function<wstring(INT32)> Formatter)
{
    m_Formatter = Formatter;
}

