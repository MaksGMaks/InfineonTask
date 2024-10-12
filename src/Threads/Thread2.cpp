#include "Thread2.hpp"

Thread2::Thread2(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent)
: AThread(queue, list, parent) {}

void Thread2::onQUpdate() {
    doResume();
}

void Thread2::process() {
    m_list->clear();
    std::queue<int> temp;
    {
        std::lock_guard lock(m_mutex);
        if(m_queue->size() > 20) {
            m_queue->pop();
        }
        temp = *m_queue;
    }

    for(; !temp.empty(); temp.pop()) {
        m_list->addItem(QString::number(temp.front()));
    }
    doPause();
}