#include "AdvanceSleep.h"
#if SLEEP_MODE != 0
double nowMs()
{
    return std::chrono::duration<double, std::milli>(std::chrono::system_clock::now().time_since_epoch()).count();
}
#endif
#if SLEEP_MODE == 1
auto eventQueue = atomic_queue::AtomicQueueB<
    advance_sleep::Event*,
    std::allocator<advance_sleep::Event*>,
    (advance_sleep::Event*)NULL,
    false,
    false,
    false>(1024);
std::priority_queue<advance_sleep::Event*, std::vector<advance_sleep::Event*>, advance_sleep::CompareEvent> pq;
volatile bool busySpinQuit = false;
void busySpin()
{
    while (!busySpinQuit) {
        advance_sleep::Event* p;
        while (eventQueue.try_pop(p)) {
            pq.push(p);
        }
        if (pq.empty()) {
            continue;
        }
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        double ts = std::chrono::duration<double, std::milli>(duration).count();
        while (!pq.empty() && pq.top()->wakeUpTimeMs <= ts) {
            pq.top()->p.set_value();
            pq.pop();
        }
    }
}
std::thread busySpinThread(busySpin);
void advanceSleep(double ms)
{
    advance_sleep::Event e;
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
#elif SLEEP_MODE != 0 // SLEEP_MODE == 2
void advanceSleep(double ms)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long)(ms * 1e6)));
}
#endif