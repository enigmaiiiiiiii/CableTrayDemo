#include "helper.h"
#include "dbconnector.h"
#include "route.h"

#include <QStandardItemModel>
#include <QTextStream>
#include <QRandomGenerator>
#include <QFile>
#include <vector>
#include <QDebug>
#include <QString>
#include <QIODevice>
#include <QSqlDatabase>
#include <QSqlQuery>

Helper::Helper()
{ }

// 随机生成无向图
void Helper::GenerateRandomGraph()
{
}

void Helper::saveAsCsv(QStandardItemModel *model, const QString &fileName)
{    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < model->columnCount(); ++i)
    {
        QString header = model->headerData(i, Qt::Horizontal).toString();
        out << header;
        if (i < model->columnCount() - 1)
            out << ",";
        else
            out << "\n";
    }

    for (int r = 0; r < model->rowCount(); ++r)
    {
        for (int c = 0; c < model->columnCount(); ++c)
        {
            qDebug() << model->data(model->index(r, c)).toString();
            QVariant item = model->data(model->index(r, c)).toString();
            if (item.toBool()) {
                if (item.toString().contains(',')) {
                    out << "\"" <<item.toString() << "\"";
                } else {
                    out << item.toString();
                }
            }
            if (c < model->columnCount() - 1) {
                out << ",";
            } else {
                out << "\r\n";
            }
        }
    }
//    load(fileName);
    return;
}

QList<QList<int>> Helper::listForGraph(int nodeCount)
{
    QList<QList<int>> ret(nodeCount, QList<int>(nodeCount, INT16_MAX));
    for (int i = 0; i < nodeCount; ++i) {
        ret[i][i] = 0;
        if (i < nodeCount - 1) {
            ret[i][i + 1] = QRandomGenerator::global()->generate() % 100;
            ret[i + 1][i] = ret[i][i + 1];
        }
        int n = 2;
        for (int j = i + 1; j < nodeCount; ++j ) {
            if (n == 0) break;
            if (QRandomGenerator::global()->generate() % 12 == 0) {
                ret[i][j] = QRandomGenerator::global()->generate() % 100;
                ret[j][i] = ret[i][j];
                n--;
            }
        }
    }
    return ret;
}

void Helper::buildRandomDataBase(DbConnector *dbConn)
{
    QList<QList<int>> graphList = listForGraph(40);

    for (int i = 0; i < graphList.size(); i++) {
        QString name = QString("Node%1").arg(i);
        Node node = Node(name);
        dbConn->addNodeRecord(&node);
    }

    QList<Edge> edges;
    int count = 0;

    for (int i = 0; i < graphList.size(); i++) {
        for (int j = i + 1; j < graphList.size(); j++) {
            if (graphList[i][j] != INT16_MAX) {
                Edge edge = Edge(QString("Edge%1").arg(count++), i, j, graphList[i][j]);
                dbConn->addEdgeRecord(&edge);
                edges.append(edge);
            }
        }
    }

    Graph graph(edges);
    dbConn->addGraphRecord(&graph);

    for (int i = 0; i < graphList.size(); i++) {
        int start = QRandomGenerator::global()->generate() % graphList.size();
        int end = QRandomGenerator::global()->generate() % graphList.size();
        if (start != end)
        {
            Edge startEdge = edges[start];
            Edge endEdge = edges[end];
            Route route(&graph, startEdge.Name(), endEdge.Name());
            dbConn->addRouteRecord(&route);
        } else {
            i--;
        }
    }
}
