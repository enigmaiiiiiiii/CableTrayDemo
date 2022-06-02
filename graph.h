#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <QString>
#include <QList>
#include <QMap>


class Node
{
    struct Pos 
    {
        int x;
        int y;
        int z;
    };

public:
    Node(const QString &name);

    static int getNodeCount();

    QString getName();
    int getId() const;
    void setNext(Node *next);
    Pos getPos() const;

private:
    static int No;

    int id;
    QString Name;
    std::vector<Node*> nexts;
    Pos pos;

};

class Edge
{
public:
    Edge();
    Edge(int, int, int, const QString&);
    Edge &operator=(const Edge&);

    QString getName();
    int getNode1() const;
    int getNode2() const;
    int getLength() const;
//    int getRouteCount() const;

private:
    QString TrayName;
    int node1;
    int node2;
    int length;
//    int routeCount;
};

class Graph
{

public:
    Graph(std::vector<Edge>&);
    Graph(QList<QList<int>>&);

    unsigned int size();
    std::vector<std::vector<Edge>> getHead() ;
    QString getTrayName(int start, int end);
    void addEdge(Edge);
    Edge* getEdge(const QString&);
    std::map<QString, Edge*> getEdgeMap();

private:
    std::vector<std::vector<Edge>> head;
    std::map<QString, Edge*> edgeMap;
};

#endif // GRAPH_H
