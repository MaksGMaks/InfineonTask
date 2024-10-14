#include "MainWindow.hpp"

MainWindow::MainWindow(std::unique_ptr<std::queue<int>> queue, QWidget *parent)
: QMainWindow(parent)
, m_queue(std::move(queue)) {
    setupUi();
    setupConnections();

    isUpdateThreadPaused = false;
    isUpdateThreadRunning = true;
    m_updateThread = std::thread(&MainWindow::updateValuesInQueue, this);
}

MainWindow::~MainWindow() {
    onStopPushThreadClicked();
    onStopPopThreadClicked();
    {
        std::lock_guard lock(m_mutex);
        isUpdateThreadRunning = false;
        isUpdateThreadPaused = false;
    }
    m_updateThreadState.notify_all();
    if(m_updateThread.joinable())
        m_updateThread.join();
}

void MainWindow::setupConnections() {
    connect(m_startPushThread, &QPushButton::clicked, this, &MainWindow::onStartPushThreadClicked);
    connect(m_stopPushThread, &QPushButton::clicked, this, &MainWindow::onStopPushThreadClicked);

    connect(m_startPopThread, &QPushButton::clicked, this, &MainWindow::onStartPopThreadClicked);
    connect(m_stopPopThread, &QPushButton::clicked, this, &MainWindow::onStopPopThreadClicked);
}

    // Push thread slots

void MainWindow::onStartPushThreadClicked() {
    isPushThreadRunning = true;
    m_pushThread = std::thread(&MainWindow::pushValuesToQueue, this);

    m_startPushThread->setEnabled(false);
    m_stopPushThread->setEnabled(true);
}

void MainWindow::onStopPushThreadClicked() {
    {
        std::lock_guard lock(m_mutex);
        isPushThreadRunning = false;
    }
    if(m_pushThread.joinable())
        m_pushThread.join();
    
    m_startPushThread->setEnabled(true);
    m_stopPushThread->setEnabled(false);
}

    // Pop thread slots

void MainWindow::onStartPopThreadClicked() {
    isPopThreadRunning = true;
    m_popThread = std::thread(&MainWindow::popValuesFromQueue, this);

    m_startPopThread->setEnabled(false);
    m_stopPopThread->setEnabled(true);
}

void MainWindow::onStopPopThreadClicked() {
    {
        std::lock_guard lock(m_mutex);
        isPopThreadRunning = false;
    }

    if(m_popThread.joinable())
        m_popThread.join();
    
    m_startPopThread->setEnabled(true);
    m_stopPopThread->setEnabled(false);
}

void MainWindow::pushValuesToQueue() {
    int value = 0;
    while(isPushThreadRunning) {
        value++;
        {
            std::lock_guard lock(m_mutex);
            m_queue->push(value);
        }
        
        m_pushThreadNumberList->addItem(QString::number(value));

        if(m_pushThreadNumberList->count() > constants::DEFAULT_LIST_SIZE) {
            delete m_pushThreadNumberList->takeItem(0);
        }
        m_pushThreadNumberList->scrollToBottom();
        {
            std::lock_guard lock(m_mutex);
            isUpdateThreadPaused = false;
        }
        m_updateThreadState.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(constants::DEFAULT_SLEEP_TIME));
    }
}

void MainWindow::updateValuesInQueue() {
    while(isUpdateThreadRunning) {
        m_updateThreadNumberList->clear();
        std::queue<int> temp;
        {
            std::lock_guard lock(m_mutex);
            while(m_queue->size() > 20) {
                m_queue->pop();
            }
            temp = *m_queue;
        }

        for(; !temp.empty(); temp.pop()) {
            m_updateThreadNumberList->addItem(QString::number(temp.front()));
        }
        {
            std::lock_guard lock(m_mutex);
            isUpdateThreadPaused = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(constants::DEFAULT_SLEEP_TIME));
    }
}

void MainWindow::popValuesFromQueue() {
    while(isPopThreadRunning) {
        int value = 0;
        {
            std::lock_guard lock(m_mutex);
            if(!m_queue->empty()) {
                value = m_queue->front();
                m_queue->pop();
            }
        }
        if(value != 0) {
            m_popThreadNumberList->addItem(QString::number(value));
            if(m_popThreadNumberList->count() > constants::DEFAULT_LIST_SIZE) {
                delete m_popThreadNumberList->takeItem(0);
            }
            m_popThreadNumberList->scrollToBottom();
            {
                std::lock_guard lock(m_mutex);
                isUpdateThreadPaused = false;
            }
            m_updateThreadState.notify_all();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(constants::DEFAULT_SLEEP_TIME));
    }
}

void MainWindow::setupUi() {
    resize(constants::mainWindowSize::WIDTH, constants::mainWindowSize::HEIGHT);
    m_centralWidget = new QWidget();
    
    m_pushThreadNumberList = new QListWidget();
    m_updateThreadNumberList = new QListWidget();
    m_popThreadNumberList = new QListWidget();

    m_startPushThread = new QPushButton("Start");
    m_startPushThread->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_stopPushThread = new QPushButton("Stop");
    m_stopPushThread->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_stopPushThread->setEnabled(false);

    m_startPopThread = new QPushButton("Start");
    m_startPopThread->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_stopPopThread = new QPushButton("Stop");
    m_stopPopThread->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_stopPopThread->setEnabled(false);

    m_pushThreadLabel = new QLabel("Push Thread");
    m_updateThreadLabel = new QLabel("Update Thread");
    m_popThreadLabel = new QLabel("Pop Thread");

    m_pushThreadLayout = new QVBoxLayout();
    m_pushThreadLayout->addWidget(m_pushThreadLabel);
    m_pushThreadLayout->addWidget(m_pushThreadNumberList);
    m_pushThreadLayout->addWidget(m_startPushThread);
    m_pushThreadLayout->addWidget(m_stopPushThread);

    m_updateThreadLayout = new QVBoxLayout();
    m_updateThreadLayout->addWidget(m_updateThreadLabel);
    m_updateThreadLayout->addWidget(m_updateThreadNumberList);

    m_popThreadLayout = new QVBoxLayout();
    m_popThreadLayout->addWidget(m_popThreadLabel);
    m_popThreadLayout->addWidget(m_popThreadNumberList);
    m_popThreadLayout->addWidget(m_startPopThread);
    m_popThreadLayout->addWidget(m_stopPopThread);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_pushThreadLayout);
    m_mainLayout->addLayout(m_updateThreadLayout);
    m_mainLayout->addLayout(m_popThreadLayout);

    m_centralWidget->setLayout(m_mainLayout);

    setCentralWidget(m_centralWidget);
}