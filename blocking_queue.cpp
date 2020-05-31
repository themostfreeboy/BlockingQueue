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
        // 程序已退出
        return;
    }
    _queue.push(data);
    _cond_var.notify_one();
}

int BlockingQueue::pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    // 使用while防止虚假唤醒
    while (!_quit && _queue.empty()) {
        _cond_var.wait(lock, [this] { return _quit || !_queue.empty(); });
    }
    if (_quit) {
        // 程序已退出，返回-1
        return -1;
    }
    // 队列一定非空
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
        _cond_var.notify_all();
    }
}
