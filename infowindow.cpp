#include "infowindow.h"
#include "graph.h"
#include "route.h"
#include "helper.h"
#include "mainwindow.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QDockWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLayout>

InfoWindow::InfoWindow(QWidget *parent)
    : QWidget{parent}
{
     resize(800, 600);
     setWindowFlag(Qt::Popup);
     graphTableView = new QTableView{this};
     graphTableView->move(300, 300);
     graphTableView->resize(600, 600);

     QStandardItemModel *model = new QStandardItemModel(this);
     QStringList headers;
     MainWindow *w = static_cast<MainWindow*>(parent);
     Graph *sampleGraph = w->getGraph();
     for (unsigned int i = 0; i < sampleGraph->size(); ++i)
     {
         headers << QString("node%1").arg(i);
     }
     model->setHorizontalHeaderLabels(headers);
     model->setVerticalHeaderLabels(headers);

     for (unsigned int i = 0; i < sampleGraph->size(); ++i)
     {
         for (unsigned int j = 0; j < sampleGraph->size(); ++j)
         {
             QStandardItem *item = new QStandardItem(QString("%1").arg(sampleGraph->getHead()[i][j].getLength()));
             model->setItem(i, j, item);
         }
     }
     graphTableView->setModel(model);

}
