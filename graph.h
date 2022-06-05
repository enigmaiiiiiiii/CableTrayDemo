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

    inline QString Name() const {return trayName;}
    inline int Node1() const { return node1; }
    inline int Node2() const { return node2; }
    inline int Length() const { return length; }

private:
    QString trayName;
    int node1;
    int node2;
    int length;
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
    const QMap<QString, Edge*>& EdgeMap();

private:
    std::vector<std::vector<Edge>> head;
    QMap<QString, Edge*> edgeMap;
};

#endif // GRAPH_H
