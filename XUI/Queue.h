#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif // !VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

// windows
#include <Windows.h>

// c
#include <assert.h>

// c++
#include <list>



template <class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(int Capacity = -1) :
        m_Capacity(Capacity),
        m_Deleted(false),
        m_QueueNotFullEvent(NULL),
        m_QueueNotEmptyEvent(NULL)
    {
        InitializeCriticalSection(&m_CriticalSection);

        m_QueueNotFullEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
        m_QueueNotEmptyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    ~ThreadSafeQueue()
    {
        m_List.clear();

        if (m_QueueNotFullEvent != NULL)
        {
            CloseHandle(m_QueueNotFullEvent);
            m_QueueNotFullEvent = NULL;
        }
        if (m_QueueNotEmptyEvent != NULL)
        {
            CloseHandle(m_QueueNotEmptyEvent);
            m_QueueNotEmptyEvent;
        }

        if (!m_Deleted)
        {
            DeleteCriticalSection(&m_CriticalSection);
            m_Deleted = NULL;
        }
    }

    void Push(T* Item)
    {
        if (m_Capacity <= 0)
        {
            EnterCriticalSection(&m_CriticalSection);
            m_List.push_back(Item);
            SetEvent(m_QueueNotEmptyEvent);
            LeaveCriticalSection(&m_CriticalSection);
            return;
        }

        bool done = false;
        while (!done)
        {
            // If the queue is full, we must wait until it isn't.
            if (WaitForSingleObject(m_QueueNotFullEvent, INFINITE) != WAIT_OBJECT_0)
            {
                assert(FALSE);
            }

            // However, under some (rare) conditions we'll get here and find
            // the queue is already full again, so be prepared to loop.
            EnterCriticalSection(&m_CriticalSection);
            if (m_List.size() < m_Capacity)
            {
                m_List.push_back(Item);

                done = true;

                SetEvent(m_QueueNotEmptyEvent);

                if (m_List.size() < m_Capacity)
                {
                    SetEvent(m_QueueNotFullEvent);
                }
            }
            LeaveCriticalSection(&m_CriticalSection);
        }
    }

    T* Pop()
    {
        T* result = nullptr;

        while (result == nullptr)
        {
            // If the queue is empty, we must wait until it isn't.
            if (WaitForSingleObject(m_QueueNotEmptyEvent, INFINITE) != WAIT_OBJECT_0)
            {
                assert(FALSE);
            }

            // However, under some (rare) conditions we'll get here and find
            // the queue is already empty again, so be prepared to loop.
            EnterCriticalSection(&m_CriticalSection);
            if (!m_List.empty())
            {
                result = m_List.front();
                m_List.pop_front();
                assert(result != nullptr);

                // The queue shouldn't be full at this point!
                if (m_Capacity <= 0) assert(m_List.size() < m_Capacity);

                SetEvent(m_QueueNotFullEvent);

                if (!m_List.empty())
                {
                    SetEvent(m_QueueNotEmptyEvent);
                }
            }
            LeaveCriticalSection(&m_CriticalSection);
        }

        return result;
    }

    size_t size()
    {
        return m_List.size();
    }


protected:
    int m_Capacity;
    std::list<T*> m_List;

    HANDLE m_QueueNotFullEvent;
    HANDLE m_QueueNotEmptyEvent;

    bool m_Deleted;
    CRITICAL_SECTION m_CriticalSection;
};


