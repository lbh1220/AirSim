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
namespace advance_sleep
{
class Event
{
public:
    double wakeUpTimeMs;
    std::promise<void> p;
};
struct CompareEvent
{
    bool operator()(advance_sleep::Event* a, advance_sleep::Event* b)
    {
        return a->wakeUpTimeMs > b->wakeUpTimeMs;
    }
};
extern atomic_queue::AtomicQueueB<
    advance_sleep::Event*,
    std::allocator<Event*>,
    (Event*)NULL,
    false,
    false,
    false>
    eventQueue;
extern volatile bool busySpinQuit;
extern std::priority_queue<advance_sleep::Event*, std::vector<advance_sleep::Event*>, advance_sleep::CompareEvent> pq;
void busySpin();
}
#endif
#endif
#endif