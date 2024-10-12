#include "MainWindow.hpp"

MainWindow::MainWindow(std::shared_ptr<std::queue<int>> queue, QWidget *parent)
: QMainWindow(parent) {
    setupConstantsFile();
    setupUi();

    m_thread1 = new Thread1(queue, m_threadNumberList1);
    m_thread2 = new Thread2(queue, m_threadNumberList2);
    m_thread3 = new Thread3(queue, m_threadNumberList3);

    setupConnections();

    m_thread1->doStart();
    m_thread1->doPause();

    m_thread2->doStart();
    m_thread2->doPause();

    m_thread3->doStart();
    m_thread3->doPause();
}

void MainWindow::setupConnections() {
    connect(m_controlThread1, &QPushButton::clicked, this, &MainWindow::onThreadButtonClicked1);
    connect(m_controlThread3, &QPushButton::clicked, this, &MainWindow::onThreadButtonClicked3);

    connect(m_thread1, &Thread1::pushToQ, m_thread2, &Thread2::onQUpdate);
    connect(m_thread1, &Thread1::pushToQ, this, &MainWindow::onItemAddedToList1);

    connect(m_thread3, &Thread3::popFromQ, m_thread2, &Thread2::onQUpdate);
    connect(m_thread3, &Thread3::popFromQ, this, &MainWindow::onItemAddedToList3);
}

void MainWindow::onThreadButtonClicked1() {
    m_controlThread1->setFlat(!m_controlThread1->isFlat());
    if(m_controlThread1->isFlat()) {
        m_controlThread1->setText("Stop");
        m_thread1->doResume();
    } else {
        m_controlThread1->setText("Start");
        m_thread1->doPause();
    }
}

void MainWindow::onThreadButtonClicked3() {
    m_controlThread3->setFlat(!m_controlThread3->isFlat());
    if(m_controlThread3->isFlat()) {
        m_controlThread3->setText("Stop");
        m_thread3->doResume();
    } else {
        m_controlThread3->setText("Start");
        m_thread3->doPause();
    }
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

    m_controlThread1 = new QPushButton("Start");
    m_controlThread1->setFixedHeight(constants::buttonsSize::HEIGHT);
    m_controlThread3 = new QPushButton("Start");
    m_controlThread3->setFixedHeight(constants::buttonsSize::HEIGHT);

    m_threadLabel1 = new QLabel("Thread 1");
    m_threadLabel2 = new QLabel("Thread 2");
    m_threadLabel3 = new QLabel("Thread 3");

    m_threadLayout1 = new QVBoxLayout();
    m_threadLayout1->addWidget(m_threadLabel1);
    m_threadLayout1->addWidget(m_threadNumberList1);
    m_threadLayout1->addWidget(m_controlThread1);

    m_threadLayout2 = new QVBoxLayout();
    m_threadLayout2->addWidget(m_threadLabel2);
    m_threadLayout2->addWidget(m_threadNumberList2);

    m_threadLayout3 = new QVBoxLayout();
    m_threadLayout3->addWidget(m_threadLabel3);
    m_threadLayout3->addWidget(m_threadNumberList3);
    m_threadLayout3->addWidget(m_controlThread3);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addLayout(m_threadLayout1);
    m_mainLayout->addLayout(m_threadLayout2);
    m_mainLayout->addLayout(m_threadLayout3);

    m_centralWidget->setLayout(m_mainLayout);

    setCentralWidget(m_centralWidget);
}