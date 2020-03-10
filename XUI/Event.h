#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// c++
#include <initializer_list>
#include <map>
#include <memory>


class Event
{
public:
    Event();
    ~Event();

    bool Validate();

    bool Set();
    bool Reset();

    bool Timeout(unsigned long MilliSeconds = 0);
    bool Wait(unsigned long MilliSeconds = 0xffffffff);

    void* GetHandle();

private:
    void* m_Handle;
};


class EventGroup
{
public:
    EventGroup();
    ~EventGroup();

    bool Validate();

    bool Add(int Key);
    bool Add(std::initializer_list<int> Keys);

    void ResetAll();

    void Clear();

    bool Set(int Key);
    bool Reset(int Key);

    bool Timeout(int Key, unsigned long MilliSeconds = 0);
    bool Wait(int Key, unsigned long MilliSeconds = 0xffffffff);
    bool Wait(std::initializer_list<int> Keys, unsigned long MilliSeconds = 0xffffffff, unsigned long* Result = nullptr);

    int GetStatus();
    Event* GetEvent(int Key);
    void* GetHandle(int Key);

private:
    bool m_Valid;
    int m_Status;
    std::map<int, Event*> m_Events;
};


bool WaitEventPair(EventGroup& First, EventGroup& Second, int Condition, unsigned long MilliSeconds = 0xffffffff);

bool WaitEventPair(EventGroup& First, EventGroup& Second, std::initializer_list<int> Conditions, unsigned long MilliSeconds = 0xffffffff);
