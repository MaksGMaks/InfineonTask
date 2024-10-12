#pragma once

#include "AThread.hpp"

class Thread3 : public AThread {
    Q_OBJECT

public:
    explicit Thread3(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread3() = default;

signals:
    void popFromQ();

private:
    void process() override;

};