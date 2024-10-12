#include "AThread.hpp"

AThread::AThread(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent)
: QObject(parent)
, m_list(list)
, m_queue(queue) {
}

AThread::~AThread() {
    doStop();
}

void AThread::doStart() {
    isRunning = true;
    m_workerThread = std::thread(&AThread::doWork, this);
}

void AThread::doStop() {
    {
        std::lock_guard lock(m_mutex);
        isRunning = false;
    }
    doResume();
    if(m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void AThread::doPause() {
    std::lock_guard lock(m_mutex);
    isPaused = true;
}

void AThread::doResume() {
    {
        std::lock_guard lock(m_mutex);
        isPaused = false;
    }
    m_pause.notify_all();
}

void AThread::doWork() {
    while (isRunning)
    {
        {
            std::unique_lock lock(m_mutex);
            m_pause.wait(lock, [this](){ return !isPaused || !isRunning; });
        }

        if(!isRunning)
            break;

        process();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
}