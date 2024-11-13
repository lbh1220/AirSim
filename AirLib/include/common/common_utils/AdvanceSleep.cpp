#include "AdvanceSleep.h"
#if SLEEP_MODE != 0
double nowMs()
{
    return std::chrono::duration<double, std::milli>(std::chrono::system_clock::now().time_since_epoch()).count();
}
#endif
#if SLEEP_MODE == 1
boost::lockfree::queue<Event*> eventQueue;
std::priority_queue<Event*, std::vector<Event*>, CompareEvent> pq;
volatile bool busySpinQuit = false;
void busySpin()
{
    while (!busySpinQuit) {
        Event* p;
        while (eventQueue.pop(p)) {
            pq.push(p);
        }
        if (pq.empty()) {
            continue;
        }
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        double ts = std::chrono::duration<double, std::milli>(duration).count();
        if (pq.top()->wakeUpTimeMs <= ts) {
            pq.top()->p.set_value();
            pq.pop();
        }
    }
}
std::thread busySpinThread(busySpin);
void advanceSleep(double ms)
{
    Event e;
    e.wakeUpTimeMs = ms + nowMs();
    eventQueue.push(&e);
    e.p.get_future().wait();
}
#elif SLEEP_MODE == 3
void advanceSleep(double ms)
{
    auto start = nowMs();
    while (nowMs() - start < ms) {
        std::this_thread::yield();
    }
}
#elif SLEEP_MODE == 4
void advanceSleep(double ms)
{
    auto start = nowMs();
    static constexpr std::chrono::duration<double> MinSleepDuration(0);
    while (nowMs() - start < ms) {
        std::this_thread::sleep_for(MinSleepDuration);
    }
}
#elif SLEEP_MODE == 5
void advanceSleep(double ms)
{
    auto start = nowMs();
    while (nowMs() - start < ms) {
    }
}
#elif SLEEP_MODE != 0// SLEEP_MODE == 2
void advanceSleep(double ms)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long)(ms * 1e6)));
}
#endif