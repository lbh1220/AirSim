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
#include "boost/lockfree/queue.hpp"
double nowMs();
void advanceSleep(double ms);
#if SLEEP_MODE == 1
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

extern boost::lockfree::queue<Event*> eventQueue;
extern std::priority_queue<Event*, std::vector<Event*>, CompareEvent> pq;
void busySpin();
extern std::thread busySpinThread;
#endif
#endif
#endif