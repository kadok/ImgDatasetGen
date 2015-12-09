#include "mainwindow.h"
#include <QApplication>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Image Dataset Generator");

    MainWindow mainW;
    mainW.setWindowTitle("Image Dataset Generator");
    mainW.show();

    return app.exec();
}
