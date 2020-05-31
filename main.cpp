#include <iostream>
#include <thread>
#include <vector>
#include <csignal>
#include <string>
#include <unistd.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "blocking_queue.h"

const static int PUSH_THREAD_NUM = 5;
const static int POP_THREAD_NUM = 20;

static std::atomic<bool> quit;

void push_queue(int id, BlockingQueue* queue) {
    std::string thread_name = "push_" + std::to_string(id);
    while (!quit) {
        int data = rand() % 10000;
        size_t queue_size = queue->size();
        queue->push(data);
        std::stringstream info;
        info << "[" << thread_name << "] push queue, data=" << data << ", queue_size=" << queue_size << "\n";
        std::cout << info.str();
        usleep(200);
    }
}

void pop_queue(int id, BlockingQueue* queue) {
    std::string thread_name = "pop_" + std::to_string(id);
    while (!quit) {
        int data = queue->pop();
        size_t queue_size = queue->size();
        std::stringstream info;
        info << "[" << thread_name << "] pop queue, data=" << data << ", queue_size=" << queue_size << "\n";
        std::cout << info.str();
        usleep(200);
    }
}

void signalHandler(int signum) {
    quit = true;
} 

int main(int argc, char** argv) {
    quit = false;
    signal(SIGINT, signalHandler);

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
        usleep(200);
    }

    queue.quit();
    for (auto it = threads.begin(); it != threads.end(); ++it) {
        if (it->joinable()) {
            it->join();
        }
    }

    std::cout << "program going to quit\n";

    return 0;
}
