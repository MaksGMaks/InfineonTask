#include "Thread3.hpp"

Thread3::Thread3(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent)
: AThread(queue, list, parent) {}

void Thread3::process() {
    int value = 0;
    {
        std::lock_guard lock(m_mutex);
        if(!m_queue->empty()) {
            value = m_queue->front();
            m_queue->pop();
        }
    }
    if(value != 0) {
        m_list->addItem(QString::number(value));
        emit popFromQ();
    } 
}