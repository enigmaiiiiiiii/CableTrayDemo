#include "mainwindow.h"
#include "graph.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     MainWindow w;
     w.show();
     return a.exec();

}
