#ifndef __BLOCKING_QUEUE__
#define __BLOCKING_QUEUE__

#include <mutex>
#include <condition_variable>
#include <queue>

class BlockingQueue {
public:
    BlockingQueue();
    ~BlockingQueue();
    void push(const int& data);
    int pop();
    size_t size();
    bool is_empty();
private:
    std::queue<int> _queue;
    std::mutex _mutex;
    std::condition_variable _cond_var;
    BlockingQueue(const BlockingQueue& queue) = delete;
    BlockingQueue& operator=(const BlockingQueue& queue) = delete;
};

#endif
