#pragma once

#include "AThread.hpp"

/**
 * First thread, that push values into queue
 */
class Thread1 : public AThread {
    Q_OBJECT

public:
    explicit Thread1(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread1() = default;

signals:
    void pushToQ();                 // report receiver that value was pushed into queue (receiver is thread 2)

private:
    void process() override;        // overrides to push value into queue

    int value = 0;                  // start value
};