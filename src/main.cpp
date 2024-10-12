#include <iostream>
#include <memory>
#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // separated from threads queue
    std::shared_ptr<std::queue<int>> queue = std::make_shared<std::queue<int>>();

    // main window
    MainWindow *mainWindow = new MainWindow(queue);
    mainWindow->show();

    return app.exec();
}