#pragma once

#include "AThread.hpp"

class Thread1 : public AThread {
    Q_OBJECT

public:
    explicit Thread1(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread1() = default;

signals:
    void pushToQ();

private:
    void process() override;

    int value = 0;
};