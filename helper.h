#ifndef HELPER_H
#define HELPER_H

#include "graph.h"
#include "dbconnector.h"
#include <QSqlDatabase>

class QStandardItemModel;


class Helper
{
public:
    Helper();
    static void GenerateRoute(Graph &graph, int start, int end);
    static bool isOverLoad();
    static void routeLength();
    static void GenerateRandomGraph();
    static void saveAsCsv(QStandardItemModel *model, const QString &fileName);
    static void graphSaveAsCsv(Graph *graph, const QString &fileName);
    static QList<QList<int>> listForGraph(int nodeCount);
    static void buildRandomDataBase(DbConnector *dbConn);
};

#endif // HELPER_H
