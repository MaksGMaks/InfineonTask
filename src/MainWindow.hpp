#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <queue>

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

#include "Constants.hpp"

/**
 * Main window with Ui to observe threads' work
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::unique_ptr<std::queue<int>> queue, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartPushThreadClicked();               // start push thread
    void onStopPushThreadClicked();                // stop push thread

    void onStartPopThreadClicked();               // start pop thread
    void onStopPopThreadClicked();                // stop pop thread

private:
    // Methods
    void setupConnections();                    // connect all elements together
    void setupUi();                             // create all Ui elements, set its size, position, text 

    // Thread's methods
    void pushValuesToQueue();                             // push value into queue
    void updateValuesInQueue();                             // update queue
    void popValuesFromQueue();                            // pop value from queue

    // Thread's variables
    // Condition variables
    std::condition_variable m_updateThreadState;

    // Atomic
    std::atomic<bool> isPushThreadRunning;
    std::atomic<bool> isUpdateThreadRunning;
    std::atomic<bool> isUpdateThreadPaused;
    std::atomic<bool> isPopThreadRunning;

    // Mutex
    std::mutex m_mutex;

    // Threads
    std::thread m_pushThread;
    std::thread m_updateThread;
    std::thread m_popThread;

    // Storage queue
    std::unique_ptr<std::queue<int>> m_queue;   // queue for threads

    // Ui elements
    // Central Widget
    QWidget *m_centralWidget;                   // central widget for main window

    // Lists
    QListWidget *m_pushThreadNumberList;
    QListWidget *m_updateThreadNumberList;
    QListWidget *m_popThreadNumberList;

    // Buttons
    QPushButton *m_startPushThread;
    QPushButton *m_stopPushThread;

    QPushButton *m_startPopThread;
    QPushButton *m_stopPopThread;

    // Labels
    QLabel *m_pushThreadLabel;
    QLabel *m_updateThreadLabel;
    QLabel *m_popThreadLabel;

    // Layouts
    QVBoxLayout *m_pushThreadLayout;               // layout for thread 1 elements to make it structurized
    QVBoxLayout *m_updateThreadLayout;               // layout for thread 2 elements to make it structurized
    QVBoxLayout *m_popThreadLayout;               // layout for thread 3 elements to make it structurized

    QHBoxLayout *m_mainLayout;                  // layout for central widget with all elements
};
