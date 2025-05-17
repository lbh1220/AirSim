#ifndef SLEEP_MODE
#define SLEEP_MODE 1
#endif
#if SLEEP_MODE != 0
#ifndef ADVANCE_SLEEP
#define ADVANCE_SLEEP
#include <chrono>
#include <future>
#include <thread>
#include <queue>
double nowMs();
void advanceSleep(double ms);
#if SLEEP_MODE == 1
#ifndef __cpp_lib_atomic_is_always_lock_free
#define __cpp_lib_atomic_is_always_lock_free 0
#endif
#include "atomic_queue/atomic_queue.h"
class Event
{
public:
    double wakeUpTimeMs;
    std::promise<void> p;
};
struct CompareEvent
{
    bool operator()(Event* a, Event* b)
    {
        return a->wakeUpTimeMs > b->wakeUpTimeMs;
    }
};
extern atomic_queue::AtomicQueueB<
    Event*,
    std::allocator<Event*>,
    (Event*)NULL,
    false,
    false,
    false>
    eventQueue;
extern std::priority_queue<Event*, std::vector<Event*>, CompareEvent> pq;
void busySpin();
extern std::thread busySpinThread;
#endif
#endif
#endif