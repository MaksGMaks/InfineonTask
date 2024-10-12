#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

#include <QListWidget>
#include <QObject>

/** 
 * Abstract class for threads with basic logic. It provides threads with control methods (start, stop, pause, resume).
 * Working method doWork has loop, which is controlled by methods. To make new thread with unique method just override process method
 * in inherited class.
 */
class AThread : public QObject {
    Q_OBJECT
public:
    AThread(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    virtual ~AThread();

public slots:
    // Control method
    void doPause();                                     // pause thread's work but don't stop it
    void doResume();                                    // resume thread's work after pause
    void doStart();                                     // start thread. should be called first
    void doStop();                                      // stop thread by joining it

protected:
    // Worker method
    void doWork();                                      // working method with loop
    virtual void process() = 0;                         // unique method for thread. called inside working method, requires overriding

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
    std::shared_ptr<std::queue<int>> m_queue;           // shared queue

    // List
    QListWidget *m_list;                                // pointer to own list on Ui to show information
};



