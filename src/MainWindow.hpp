#pragma once

#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>

#include "Constants.hpp"
#include "Threads/Thread1.hpp"
#include "Threads/Thread2.hpp"
#include "Threads/Thread3.hpp"

/**
 * Main window with Ui to observe threads' work
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<std::queue<int>> queue, QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onStartThreadClicked1();               // start thread 1
    void onThreadButtonClicked1();              // pause and resume thread 1
    void onStopThreadClicked1();                // stop thread 1

    void onStartThreadClicked2();               // start thread 1
    void onStopThreadClicked2();                // stop thread 1

    void onStartThreadClicked3();               // start thread 1
    void onThreadButtonClicked3();              // pause and resume thread 3
    void onStopThreadClicked3();                // stop thread 1

    void onItemAddedToList1();                  // prevent stack overflow by deleting old data from list 1; scroll down
    void onItemAddedToList3();                  // prevent stack overflow by deleting old data from list 3; scroll down

private:
    // Methods
    void setupConnections();                    // connect all elements together
    void setupConstantsFile();                  // read data from settings file; set to default if file corrupted; create file with default data if does't exists
    void setupUi();                             // create all Ui elements, set its size, position, text 

    // Threads
    Thread1 *m_thread1;
    Thread2 *m_thread2;
    Thread3 *m_thread3;

    // Ui elements
    // Central Widget
    QWidget *m_centralWidget;                   // central widget for main window

    // Lists
    QListWidget *m_threadNumberList1;
    QListWidget *m_threadNumberList2;
    QListWidget *m_threadNumberList3;

    // Buttons
    QPushButton *m_startThread1;
    QPushButton *m_controlThread1;
    QPushButton *m_stopThread1;
    
    QPushButton *m_startThread2;
    QPushButton *m_stopThread2;

    QPushButton *m_startThread3;
    QPushButton *m_controlThread3;
    QPushButton *m_stopThread3;

    // Labels
    QLabel *m_threadLabel1;
    QLabel *m_threadLabel2;
    QLabel *m_threadLabel3;

    // Layouts
    QVBoxLayout *m_threadLayout1;               // layout for thread 1 elements to make it structurized
    QVBoxLayout *m_threadLayout2;               // layout for thread 2 elements to make it structurized
    QVBoxLayout *m_threadLayout3;               // layout for thread 3 elements to make it structurized

    QHBoxLayout *m_mainLayout;                  // layout for central widget with all elements

    // Constants
    QFile *m_constants;
    int maxListSize1;                           // maximum size of items in list1 to prevent oveflow
    int maxListSize3;                           // maximum size of items in list3 to prevent oveflow
};
