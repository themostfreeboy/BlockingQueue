#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <string>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <atomic>
#include "blocking_queue.h"

// push队列操作线程数
const static int PUSH_THREAD_NUM = 5;
// pop队列操作线程数
const static int POP_THREAD_NUM = 50;

static std::atomic<bool> quit;

static void push_queue(int id, BlockingQueue* queue) {
    std::string thread_name = "push_" + std::to_string(id);
    while (!quit) {
        // 产生0-9999随机数
        int data = rand() % 10000;
        size_t queue_size = queue->size();
        queue->push(data);
        // 构造完整字符串，防止由于std::cout在多线程下，线程不安全，完整字符串被分开
        std::stringstream info;
        info << "[" << thread_name << "] push queue, data=" << data << ", queue_size=" << queue_size << "\n";
        std::cout << info.str();
        usleep(200000);
    }
}

static void pop_queue(int id, BlockingQueue* queue) {
    std::string thread_name = "pop_" + std::to_string(id);
    while (!quit) {
        int data = queue->pop();
        size_t queue_size = queue->size();
        // 构造完整字符串，防止由于std::cout在多线程下，线程不安全，完整字符串被分开
        std::stringstream info;
        info << "[" << thread_name << "] pop queue, data=" << data << ", queue_size=" << queue_size << "\n";
        std::cout << info.str();
        usleep(200000);
    }
}

// 捕捉信号，用于程序的优雅退出
static void signalHandler(int signum) {
    quit = true;
} 

int main(int argc, char** argv) {
    quit = false;
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    srand((unsigned int)time(NULL));

    BlockingQueue queue;
    std::vector<std::thread> threads;
    for (int i = 0; i < PUSH_THREAD_NUM; ++i) {
        threads.push_back(std::thread(push_queue, i, &queue));
    }
    for (int i = 0; i < POP_THREAD_NUM; ++i) {
        threads.push_back(std::thread(pop_queue, i, &queue));
    }

    while (!quit) {
        usleep(200000);
    }

    queue.quit();
    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            // 等待线程的自然结束
            thread.join();
        }
    }

    std::cout << "\nprogram going to quit\n";

    return 0;
}
