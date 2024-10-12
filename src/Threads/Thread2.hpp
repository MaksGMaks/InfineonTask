#pragma once

#include "AThread.hpp"

class Thread2 : public AThread {
    Q_OBJECT

public:
    explicit Thread2(std::shared_ptr<std::queue<int>> queue, QListWidget *list, QObject *parent = nullptr);
    ~Thread2() = default;

public slots:
    void onQUpdate();

private:
    void process() override;

};