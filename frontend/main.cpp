#include "request.h"
#include "helper.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <string>
#include <curl/curl.h>




int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
