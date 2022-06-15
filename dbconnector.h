#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H
#include <QString>
#include <QSqlDatabase>

class Node;
class Edge;
class Graph;
class Route;

class DbConnector
{
public:
    DbConnector(const QString& fname = ":memory:");
    ~DbConnector();

    QSqlDatabase initDb();
    void addNodeRecord(Node* node, const QString &des = QString());
    void addEdgeRecord(Edge* edge, const QString &des = QString());
    void addGraphRecord(Graph *graph, const QString &des = QString());
    void addRouteRecord(Route *route, const QString &des = QString());

    // void addNodeRecord();
    void addEdgeRecord(int id, const QString &name, int node_1, int node_2,
                       int Length, int capacity,
                       const QString &material = QString(),
                       const QString &appearence = QString(),
                       const QString &des = QString());
                    
    void addRouteRecord(int id,
                        const QString &startEdge,
                        const QString &endEdge,
                        const QString &path,
                        int length,
                        const QString &des = QString());

    void checkRepeatRecord();

};

#endif // DBCONNECTOR_H
