#include <assert.h>
#include "blocking_queue.h"

BlockingQueue::BlockingQueue() {
    _quit = false;
}

BlockingQueue::~BlockingQueue() {
    quit();
}

void BlockingQueue::push(const int& data) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_quit) {
        _cond_var.notify_all();
        return;
    }
    _queue.push(data);
    _cond_var.notify_all();
}

int BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_quit) {
        return 0xffffffff;
    }
    _cond_var.wait(lock, [this] { return _quit || !_queue.empty(); });
    if (_quit) {
        return 0xffffffff;
    }
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

void BlockingQueue::quit() {
    if (!_quit) {
        _quit = true;
    }
}
