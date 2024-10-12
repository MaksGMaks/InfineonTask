#pragma once

#include "AThread.hpp"

/**
 * Thrird thread that pop value from queue
 */
class Thread3 : public AThread {
    Q_OBJECT

public:
    explicit Thread3(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread3() = default;

signals:
    void popFromQ();                    // report receiver that value was poped from queue (receiver is thread 2)

private:
    void process() override;            // overrides to pop value from queue

};