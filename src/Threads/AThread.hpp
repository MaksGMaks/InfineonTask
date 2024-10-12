#pragma once

#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

#include <QObject>
#include <QListWidget>

class AThread : public QObject {
    Q_OBJECT
public:
    AThread(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    virtual ~AThread();

public slots:
    // Control method
    void doPause();
    void doResume();
    void doStart();
    void doStop();

protected:
    // Worker method
    void doWork();
    virtual void process() = 0;

    // Thread variables
    // Thread
    std::thread m_workerThread;

    // Mutex
    std::mutex m_mutex;

    // Condition variable
    std::condition_variable m_pause;

    // Atomic
    std::atomic<bool> isRunning;
    std::atomic<bool> isPaused;

    // Storage
    // Pointer of queue
    std::shared_ptr<std::queue<int>> m_queue;

    // List
    QListWidget *m_list;
};



