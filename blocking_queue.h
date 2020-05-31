#ifndef __BLOCKING_QUEUE__
#define __BLOCKING_QUEUE__

#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

class BlockingQueue {
public:
    BlockingQueue();
    ~BlockingQueue();
    void push(const int& data);
    int pop();
    size_t size();
    bool is_empty();
    void quit();
private:
    std::queue<int> _queue;
    std::mutex _mutex;
    std::condition_variable _cond_var;
    std::atomic<bool> _quit;
    BlockingQueue(const BlockingQueue& queue) = delete;
    BlockingQueue& operator=(const BlockingQueue& queue) = delete;
};

#endif
