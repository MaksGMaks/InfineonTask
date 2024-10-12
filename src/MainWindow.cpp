#include "MainWindow.hpp"

MainWindow::MainWindow(std::shared_ptr<std::queue<int>> queue, QWidget *parent)
: QMainWindow(parent) {
    setupConstantsFile();
    setupUi();

    m_thread1 = new Thread1(queue, m_threadNumberList1);
    m_thread2 = new Thread2(queue, m_threadNumberList2);
    m_thread3 = new Thread3(queue, m_threadNumberList3);

    setupConnections();
}

void MainWindow::setupConnections() {
    connect(m_startThread1, &QPushButton::clicked, this, &MainWindow::onStartThreadClicked1);
    connect(m_controlThread1, &QPushButton::clicked, this, &MainWindow::onThreadButtonClicked1);
    connect(m_stopThread1, &QPushButton::clicked, this, &MainWindow::onStopThreadClicked1);

    connect(m_startThread2, &QPushButton::clicked, this, &MainWindow::onStartThreadClicked2);
    connect(m_stopThread2, &QPushButton::clicked, this, &MainWindow::onStopThreadClicked2);

    connect(m_startThread3, &QPushButton::clicked, this, &MainWindow::onStartThreadClicked3);
    connect(m_controlThread3, &QPushButton::clicked, this, &MainWindow::onThreadButtonClicked3);
    connect(m_stopThread3, &QPushButton::clicked, this, &MainWindow::onStopThreadClicked3);

    connect(m_thread1, &Thread1::pushToQ, m_thread2, &Thread2::onQUpdate);
    connect(m_thread1, &Thread1::pushToQ, this, &MainWindow::onItemAddedToList1);

    connect(m_thread3, &Thread3::popFromQ, m_thread2, &Thread2::onQUpdate);
    connect(m_thread3, &Thread3::popFromQ, this, &MainWindow::onItemAddedToList3);
}

void MainWindow::onStartThreadClicked1() {
    m_thread1->doStart();
    m_startThread1->setEnabled(false);
    m_controlThread1->setEnabled(true);
    m_stopThread1->setEnabled(true);
}

void MainWindow::onThreadButtonClicked1() {
    m_controlThread1->setFlat(!m_controlThread1->isFlat());
    if(m_controlThread1->isFlat()) {
        m_controlThread1->setText("Pause");
        m_thread1->doResume();
    } else {
        m_controlThread1->setText("Resume");
        m_thread1->doPause();
    }
}

void MainWindow::onStopThreadClicked1() {
    m_thread1->doStop();
    m_thread1->clearValue();
    m_startThread1->setEnabled(true);
    m_controlThread1->setEnabled(false);
    m_stopThread1->setEnabled(false);
}

void MainWindow::onStartThreadClicked2() {
    m_thread2->doStart();
    m_startThread2->setEnabled(false);
    m_stopThread2->setEnabled(true);
}

void MainWindow::onStopThreadClicked2() {
    m_thread2->doStop();
    m_startThread2->setEnabled(true);
    m_stopThread2->setEnabled(false);
}

void MainWindow::onStartThreadClicked3() {
    m_thread3->doStart();
    m_startThread3->setEnabled(false);
    m_controlThread3->setEnabled(true);
    m_stopThread3->setEnabled(true);
}

void MainWindow::onThreadButtonClicked3() {
    m_controlThread3->setFlat(!m_controlThread3->isFlat());
    if(m_controlThread3->isFlat()) {
        m_controlThread3->setText("Pause");
        m_thread3->doResume();
    } else {
        m_controlThread3->setText("Resume");
        m_thread3->doPause();
    }
}

void MainWindow::onStopThreadClicked3() {
    m_thread3->doStop();
    m_startThread3->setEnabled(true);
    m_controlThread3->setEnabled(false);
    m_stopThread3->setEnabled(false);
}

void MainWindow::onItemAddedToList1() {
    if(m_threadNumberList1->count() > maxListSize1) {
        delete m_threadNumberList1->takeItem(0);
    }
    m_threadNumberList1->scrollToBottom();
}

void MainWindow::onItemAddedToList3() {
    if(m_threadNumberList3->count() > maxListSize3) {
        delete m_threadNumberList3->takeItem(0);
    }
    m_threadNumberList3->scrollToBottom();
}

void MainWindow::setupConstantsFile() {
    QString path = QDir::current().filePath("Settings.txt");
    m_constants = new QFile(path);

    if(!m_constants->exists()) {
        if(m_constants->open(QIODevice::WriteOnly)) {
            QTextStream fileOut(m_constants);
            fileOut << "[MaxSizeOfListForThread1]:" << constants::DEFAULT_LIST_SIZE << "\n";
            fileOut << "[MaxSizeOfListForThread3]:" << constants::DEFAULT_LIST_SIZE << "\n";
            m_constants->close();
        } else {
            std::cout << "Can't create settings file. All settings set to default values" << std::endl;
            maxListSize1 = constants::DEFAULT_LIST_SIZE;
            maxListSize3 = constants::DEFAULT_LIST_SIZE;
            return;
        }
    }

    maxListSize1 = constants::DEFAULT_LIST_SIZE;
    maxListSize3 = constants::DEFAULT_LIST_SIZE;

    if(m_constants->open(QIODevice::ReadOnly)) {
        QTextStream fileInp(m_constants);
        QStringList settingThread1 = fileInp.readLine().split(":");
        if(settingThread1[0] == "[MaxSizeOfListForThread1]")
            maxListSize1 = settingThread1[1].toInt();
        
        QStringList settingThread3 = fileInp.readLine().split(":");
        if(settingThread3[0] == "[MaxSizeOfListForThread3]")
            maxListSize3 = settingThread3[1].toInt();
    }
}

void MainWindow::setupUi() {
    resize(constants::mainWindowSize::WIDTH, constants::mainWindowSize::HEIGHT);
    m_centralWidget = new QWidget();
    
    m_threadNumberList1 = new QListWidget();
    m_threadNumberList2 = new QListWidget();
    m_threadNumberList3 = new QListWidget();

    m_startThread1 = new QPushButton("Start");
    m_startThread1->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_controlThread1 = new QPushButton("Pause");
    m_controlThread1->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_controlThread1->setFlat(true);
    m_stopThread1 = new QPushButton("Stop");
    m_stopThread1->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_startThread2 = new QPushButton("Start");
    m_startThread2->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_stopThread2 = new QPushButton("Stop");
    m_stopThread2->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_startThread3 = new QPushButton("Start");
    m_startThread3->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_controlThread3 = new QPushButton("Pause");
    m_controlThread3->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_controlThread3->setFlat(true);
    m_stopThread3 = new QPushButton("Stop");
    m_stopThread3->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_threadLabel1 = new QLabel("Thread 1");
    m_threadLabel2 = new QLabel("Thread 2");
    m_threadLabel3 = new QLabel("Thread 3");

    m_threadLayout1 = new QVBoxLayout();
    m_threadLayout1->addWidget(m_threadLabel1);
    m_threadLayout1->addWidget(m_threadNumberList1);
    m_threadLayout1->addWidget(m_startThread1);
    m_threadLayout1->addWidget(m_controlThread1);
    m_threadLayout1->addWidget(m_stopThread1);

    m_threadLayout2 = new QVBoxLayout();
    m_threadLayout2->addWidget(m_threadLabel2);
    m_threadLayout2->addWidget(m_threadNumberList2);
    m_threadLayout2->addWidget(m_startThread2);
    m_threadLayout2->addWidget(m_stopThread2);

    m_threadLayout3 = new QVBoxLayout();
    m_threadLayout3->addWidget(m_threadLabel3);
    m_threadLayout3->addWidget(m_threadNumberList3);
    m_threadLayout3->addWidget(m_startThread3);
    m_threadLayout3->addWidget(m_controlThread3);
    m_threadLayout3->addWidget(m_stopThread3);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_threadLayout1);
    m_mainLayout->addLayout(m_threadLayout2);
    m_mainLayout->addLayout(m_threadLayout3);

    m_centralWidget->setLayout(m_mainLayout);

    setCentralWidget(m_centralWidget);
}