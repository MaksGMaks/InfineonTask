#pragma once

#include "AThread.hpp"

/**
 * Second thread that control data amount in queue
 */
class Thread2 : public AThread {
    Q_OBJECT

public:
    explicit Thread2(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread2() = default;

public slots:
    void onQUpdate();                   // reveive signals from senders, that change data in queue

private:
    void process() override;            // overrides to update queue

};