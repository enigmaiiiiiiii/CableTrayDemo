#include "helper.h"
#include <QStandardItemModel>
#include <QTextStream>
#include <QRandomGenerator>
#include <QFile>
#include <vector>
#include <QDebug>
#include <QString>
#include <QIODevice>

Helper::Helper()
{ }

// 随机生成无向图
Graph Helper::GenerateRandomGraph(int nodeNum)
{

    std::vector<Edge> edges;
    for (int i = 0; i < nodeNum; ++i)
    {
        for (int j = 0; j < nodeNum; ++j)
        {
            if (i == j)
            {
                edges.emplace_back(i, j, 0, "NULL");
            }

            QString name1 = QString("edge%1-%2").arg(i).arg(j);
            QString name2 = QString("edge%1-%2").arg(j).arg(i);
            if (QRandomGenerator::global()->generate() < INT_MAX / 3)
            {
                int randLength = rand() % 100;
                edges.emplace_back(i, j, randLength, name1);
                edges.emplace_back(j, i, randLength, name2);
            }
        }
    }

    qDebug() << edges.size();
    return Graph(edges);
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
