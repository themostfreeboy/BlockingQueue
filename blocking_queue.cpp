#include <assert.h>
#include "blocking_queue.h"

BlockingQueue::BlockingQueue() {
    return;
}

BlockingQueue::~BlockingQueue() {
    return;
}

void BlockingQueue::push(const int& data) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(data);
    _cond_var.notify_all();
}

int BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond_var.wait(lock, [this] { return !_queue.empty(); });
    assert(!_queue.empty());
    int data = _queue.front();
    _queue.pop();
    return data;
}

size_t BlockingQueue::size() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.size();
}

bool BlockingQueue::is_empty() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _queue.empty();
}
