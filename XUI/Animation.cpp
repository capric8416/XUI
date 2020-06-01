// self
#include "Animation.h"

// project
#include "Control.h"
#include "Event.h"
#include "MainWnd.h"

// namespace
using namespace std;



// static
UINT8 Animation::s_Tick;
Event Animation::s_Stop;
thread* Animation::s_Thread;
list<Control*> Animation::s_Tasks;



Animation::Animation(
    CONTROL_STATUS Status,
    D2D_RECT_F Position,
    float RadiusX,
    float RadiusY
) :
    Style(
        Status,
        Position,
        RadiusX,
        RadiusY
    )
{
}


Animation::~Animation()
{
}


void Animation::Start(UINT8 FPS)
{
    if (s_Thread != nullptr)
    {
        XTRACE("thread is already running");
        return;
    }

    if (FPS > 60)
    {
        FPS = 60;
    }

    s_Tick = 1000 / FPS;

    s_Stop.Reset();
    s_Thread = new thread(Run);
}


void Animation::Stop()
{
    s_Stop.Set();

    s_Thread->join();

    XSafeDelete(s_Thread);
}


void Animation::Run()
{
    while (!s_Stop.Wait(s_Tick))
    {
        if (!s_Wnd->Resizing())
        {
            PostMessage(s_NativeWnd, WM_PAINT, ON_ANIMATED_WPARAM, 0);
        }
    }
}


void Animation::Add(Control* Target)
{
    s_Tasks.push_back(Target);
}


void Animation::Execute()
{
    for (auto iter = s_Tasks.begin(); iter != s_Tasks.end(); iter++)
    {
        (*iter)->OnAnimated();
    }
}


void Animation::OnAnimated()
{
}
