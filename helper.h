#ifndef HELPER_H
#define HELPER_H

#include "graph.h"

class QStandardItemModel;


class Helper
{
public:
    Helper();
    static void GenerateRoute(Graph &graph, int start, int end);
    static bool isOverLoad();
    static void routeLength();
    static Graph GenerateRandomGraph(int nodeNum);
    static void saveAsCsv(QStandardItemModel *model, const QString &fileName);
    static void graphSaveAsCsv(Graph *graph, const QString &fileName);
    static QList<QList<int>> listForGraph(int nodeCount);
};

#endif // HELPER_H
