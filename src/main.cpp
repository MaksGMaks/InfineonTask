#include <iostream>
#include <memory>
#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // separated from threads queue
    std::unique_ptr<std::queue<int>> queue = std::make_unique<std::queue<int>>();

    // main window
    MainWindow *mainWindow = new MainWindow(std::move(queue));
    mainWindow->show();

    return app.exec();
}