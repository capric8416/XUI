// self
#include "Event.h"

// project
#include "Common.h"

// c++
#include <future>

// windows
#include <Windows.h>
#include <tchar.h>

// namespace
using namespace std;



Event::Event() :
    m_Handle(nullptr)
{
    m_Handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}


Event::~Event()
{
    if (m_Handle != NULL)
    {
        CloseHandle(m_Handle);
        m_Handle = NULL;
    }
}


bool Event::Validate()
{
    return m_Handle != nullptr;
}


bool Event::Set()
{
    return SetEvent(m_Handle);
}


bool Event::Reset()
{
    return ResetEvent(m_Handle);
}


bool Event::Timeout(unsigned long MilliSeconds)
{
    auto Ret = WaitForSingleObjectEx(m_Handle, MilliSeconds, FALSE);

    auto Succeed = false;
    if (Ret != WAIT_FAILED)
    {
        Succeed = (Ret == WAIT_TIMEOUT);
    }
    else
    {
        auto Error = GetLastError();
        XTRACE("WaitForSingleObjectEx: 0x%x, Error: 0x%x", Ret, Error);
    }

    return Succeed;
}


bool Event::Wait(unsigned long MilliSeconds)
{
    auto Ret = WaitForSingleObjectEx(m_Handle, MilliSeconds, FALSE);

    auto Succeed = false;
    if (Ret != WAIT_FAILED)
    {
        Succeed = (Ret == WAIT_OBJECT_0);
    }
    else
    {
        auto Error = GetLastError();
        XTRACE("WaitForSingleObjectEx: 0x%x, Error: 0x%x", Ret, Error);
    }

    return Succeed;
}


HANDLE Event::GetHandle()
{
    return m_Handle;
}


EventGroup::EventGroup() :
    m_Valid(true),
    m_Status(0),
    m_Events()
{

}


EventGroup::~EventGroup()
{
    Clear();
}


bool EventGroup::Validate()
{
    return m_Valid;
}


bool EventGroup::Add(int Key)
{
    if (GetEvent(Key))
        return false;

    auto E = new Event();
    if (!E->Validate())
    {
        XSafeDelete(E, false, false);
        return false;
    }

    m_Events.insert(pair<int, Event*>(Key, E));
    return true;
}


bool EventGroup::Add(initializer_list<int> Keys)
{
    for (const int& Key : Keys)
    {
        m_Valid = Add(Key);
    }
    return m_Valid;
}



void EventGroup::ResetAll()
{
    m_Status = 0;
    for (const auto& Item : m_Events)
    {
        Item.second->Reset();
    }
}


void EventGroup::Clear()
{
    for (const auto& Item : m_Events)
    {
        auto E = Item.second;
        XSafeDelete(E, false, true);
    }

    m_Events.clear();
}


bool EventGroup::Set(int Key)
{
    auto Value = GetEvent(Key);
    if (Value)
    {
        m_Status = Key;
        return Value->Set();
    }
    return false;
}


bool EventGroup::Reset(int Key)
{
    auto Value = GetEvent(Key);
    return Value ? Value->Reset() : false;
}


bool EventGroup::Timeout(int Key, unsigned long MilliSeconds)
{
    auto Value = GetEvent(Key);
    return Value ? Value->Timeout(MilliSeconds) : false;
}


bool EventGroup::Wait(int Key, unsigned long MilliSeconds)
{
    auto Value = GetEvent(Key);
    return Value ? Value->Wait(MilliSeconds) : true;
}


bool EventGroup::Wait(initializer_list<int> Keys, unsigned long MilliSeconds, unsigned long* Result)
{
    size_t Size = Keys.size();
    HANDLE* Handles = new HANDLE[Size];

    size_t i = 0;
    for (const auto& Key : Keys)
    {
        auto H = GetHandle(Key);
        if (H)
        {
            Handles[i] = H;
            i++;
        }
    }

    auto Ret = WaitForMultipleObjectsEx(i, Handles, FALSE, MilliSeconds, FALSE);
    if (Result != nullptr)
        *Result = Ret;

    bool Succeed = false;
    if (Ret != WAIT_FAILED)
    {
        Succeed = Ret < Size;
    }
    else
    {
        auto Error = GetLastError();
        XTRACE("WaitForMultipleObjectsEx: 0x%x, Error: 0x%x", Ret, Error);
    }

    XSafeDelete(Handles, true, false);

    return Succeed;
}


int EventGroup::GetStatus()
{
    return m_Status;
}


Event* EventGroup::GetEvent(int Key)
{
    auto iter = m_Events.find(Key);
    return iter != m_Events.end() ? iter->second : nullptr;
}


HANDLE EventGroup::GetHandle(int Key)
{
    auto E = GetEvent(Key);
    return E ? E->GetHandle() : nullptr;
}


bool WaitEventPair(EventGroup& First, EventGroup& Second, int Condition, unsigned long MilliSeconds)
{
    auto f1 = async([&First, &Condition, &MilliSeconds]() { return First.Wait(Condition, MilliSeconds); });
    auto f2 = async([&Second, &Condition, &MilliSeconds]() { return Second.Wait(Condition, MilliSeconds); });
    return f1.get() && f2.get();
}


bool WaitEventPair(EventGroup& First, EventGroup& Second, initializer_list<int> Conditions, unsigned long MilliSeconds)
{
    auto f1 = async([&First, &Conditions, &MilliSeconds]() { return First.Wait(Conditions, MilliSeconds); });
    auto f2 = async([&Second, &Conditions, &MilliSeconds]() { return Second.Wait(Conditions, MilliSeconds); });
    return f1.get() && f2.get();
}
