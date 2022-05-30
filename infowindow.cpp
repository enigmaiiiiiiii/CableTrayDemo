#include "infowindow.h"
#include "graph.h"
#include "route.h"
#include "helper.h"
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
    // graphTableView = new QTableView{this};
    // graphTableView->resize(600, 600);

    // QStandardItemModel *model = new QStandardItemModel(this);
    // QStringList headers;
    // Graph sampleGraph = Helper::GenerateRandomGraph(10);
    // for (unsigned int i = 0; i < sampleGraph.size(); ++i)
    // {
    //     headers << QString("node%1").arg(i);
    // }
    // model->setHorizontalHeaderLabels(headers);
    // model->setVerticalHeaderLabels(headers);

    // for (unsigned int i = 0; i < sampleGraph.size(); ++i)
    // {
    //     for (unsigned int j = 0; j < sampleGraph.size(); ++j)
    //     {
    //         QStandardItem *item = new QStandardItem(QString("%1").arg(sampleGraph.getHead()[i][j].getLength()));
    //         model->setItem(i, j, item);
    //     }
    // }
    // graphTableView->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *graphBtn = new QPushButton("Graph");
    QPushButton *edgeBtn= new QPushButton("Edge");
    QPushButton *nodeBtn = new QPushButton("Node");
    layout->addWidget(graphBtn);
    layout->addWidget(edgeBtn);
    layout->addWidget(nodeBtn);

    setLayout(layout);
}
