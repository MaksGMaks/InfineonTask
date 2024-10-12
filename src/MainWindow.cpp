#include "MainWindow.hpp"

MainWindow::MainWindow(std::shared_ptr<std::queue<int>> queue, QWidget *parent)
: QMainWindow(parent) {
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
    connect(m_thread3, &Thread3::popFromQ, m_thread2, &Thread2::onQUpdate);
}

void MainWindow::onThreadButtonClicked1() {
    std::cout << "[MainWindow::onThreadButtonClicked1] == m_controlThread1 is clicked" << std::endl;
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
    std::cout << "[MainWindow::onThreadButtonClicked3] == m_controlThread3 is clicked" << std::endl;
    m_controlThread3->setFlat(!m_controlThread3->isFlat());
    if(m_controlThread3->isFlat()) {
        m_controlThread3->setText("Stop");
        m_thread3->doResume();
    } else {
        m_controlThread3->setText("Start");
        m_thread3->doPause();
    }
}

void MainWindow::setupUi() {
    resize(800, 600);
    m_centralWidget = new QWidget();
    
    m_threadNumberList1 = new QListWidget();
    m_threadNumberList2 = new QListWidget();
    m_threadNumberList3 = new QListWidget();

    m_controlThread1 = new QPushButton("Start");
    m_controlThread3 = new QPushButton("Start");

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