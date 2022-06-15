#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <QString>
#include <QList>
#include <QMap>


class Node
{
    struct Pos;
public:
    Node(const QString &name, 
         const double& x = 0.0,
         const double& y = 0.0,
         const double& z = 0.0);

    QString getName();
    int getId() const;
    void Next(Node *next);
    Pos getPos() const {return pos;}
    static int NodeCount() { return id; }

private:
    static int id;
    int No;
    QString Name;

    struct Pos
    {
        double x;
        double y;
        double z;
    } pos;

    QList<Node*> nexts;
};

class Edge
{
public:
    Edge();
    Edge(const QString& , int, int, int);
    Edge(const Edge&);
    Edge &operator=(const Edge&); // 拷贝赋值

    inline QString Name() const {return trayName;}
    inline int Node1() const { return node1; }
    inline int Node2() const { return node2; }
    inline int Length() const { return length; }
    inline int Capacity() const { return capacity; }
    inline QString Material() const { return material; }
    inline QString Appearence() const { return appearence; }
private:
    QString trayName;
    int node1;
    int node2;
    int length;

    int capacity;  // 容量
    QString material;  // 材料
    QString appearence; // 外观
};

class Graph
{

public:
    Graph(QList<Edge>&);
    Graph(QList<QList<int>>&);

    unsigned int size();
    std::vector<std::vector<Edge>> getHead() ;
    QString getTrayName(int start, int end);
    void addEdge(Edge);
    Edge* getEdge(const QString&);
    const QMap<QString, Edge*>& EdgeMap();

    void addNecessaryEdge(const QString &) const;
    void addForbiddenEdge(const QString &) const;
    void cancelNecessaryEdge(const QString &) const;
    void cancelForbiddenEdge(const QString &) const;
    QStringList necessaryEdge() const;
    QStringList forbiddenEdge() const;

private:
    std::vector<std::vector<Edge>> head;
    QMap<QString, Edge*> edgeMap;

    QStringList necessaryEdgeList;
    QStringList forbiddenEdgeList;
};

#endif // GRAPH_H
