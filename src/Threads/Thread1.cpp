#include "Thread1.hpp"

Thread1::Thread1(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent)
: AThread(queue, std::move(list), parent) {}

void Thread1::process() {
    value++;
    {
        std::lock_guard lock(m_mutex);
        m_queue->push(value);
    }
    
    m_list->addItem(QString::number(value));

    emit pushToQ();
}