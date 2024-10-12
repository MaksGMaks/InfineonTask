#pragma once

#include <iostream>
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "Threads/Thread1.hpp"
#include "Threads/Thread2.hpp"
#include "Threads/Thread3.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<std::queue<int>> queue, QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onThreadButtonClicked1();
    void onThreadButtonClicked3();

private:
    // Methods
    void setupUi();
    void setupConnections();

    // Threads
    Thread1 *m_thread1;
    Thread2 *m_thread2;
    Thread3 *m_thread3;

    // Ui elements
    // Central Widget
    QWidget *m_centralWidget;

    // Lists
    QListWidget *m_threadNumberList1;
    QListWidget *m_threadNumberList2;
    QListWidget *m_threadNumberList3;

    // Buttons
    QPushButton *m_controlThread1;
    QPushButton *m_controlThread3;

    // Labels
    QLabel *m_threadLabel1;
    QLabel *m_threadLabel2;
    QLabel *m_threadLabel3;

    // Layouts
    QVBoxLayout *m_threadLayout1;
    QVBoxLayout *m_threadLayout2;
    QVBoxLayout *m_threadLayout3;

    QHBoxLayout *m_mainLayout;
};
