#include "graph.h"
#include <QString>
#include <QSet>
#include <map>
#include <QDebug>

// Node 
int Node::No = 0;
Node::Node(const QString &name)
    : Name(name)
{
    id = No;
    No++;
}

QString Node::getName() { return Name; }

int Node::getId() const { 
    return id; 
}

void Node::setNext(Node *next) { nexts.push_back(next); }

int Node::getNodeCount() { return Node::No; }

// edge
Edge::Edge()
  : trayName("NULL"),
    node1(0),
    node2(0),
    length(INT16_MAX)
{
}
Edge::Edge(int n1, int n2, int length, const QString &name)
    : trayName(name),
      node1(n1),
      node2(n2),
      length(length)
{
}

Edge &Edge::operator=(const Edge &edge)
{
    trayName = edge.trayName;
    node1 = edge.node1;
    node2 = edge.node2;
    length = edge.length;

    return *this;
}


// 以命名边生成Graph
Graph::Graph(std::vector<Edge> &edges)
{
    // 统计相关节点个数
    QSet<int> nodes;
    for (auto &edge : edges)
    {
        nodes.insert(edge.Node1());
        nodes.insert(edge.Node2());
    }

    int nodeNum = nodes.size();
    std::vector<std::vector<int>> isFilled(nodeNum, std::vector<int>(nodeNum, 0));

    for (int i = 0; i < nodeNum; ++i)
    {
        head.emplace_back(nodeNum, Edge());  // 初始化head
        head[i][i] = Edge(i, i, 0, "NULL");  // 对角元素
    }

    for (auto &edge : edges)
    {
        edgeMap.insert(edge.Name(), &edge);
        int start = edge.Node1();
        int end   = edge.Node2();

        if (!isFilled[start][end])  // 放止重复填充
        {
            head[start][end]     = edge;
            isFilled[start][end] = 1;
            isFilled[end][start] = 1;
        }
    }
}

// 生成按序号命名的边edge1, edge2
Graph::Graph(QList<QList<int>> &edges)
{
    int nodeNum = edges.size();
    int edgeNum = 0;
    head = std::vector<std::vector<Edge>>(nodeNum, std::vector<Edge>(nodeNum, Edge()));  // 初始化二维vector

    for (int i = 0; i < nodeNum; ++i)
    {
        head[i][i] = Edge(i, i, edges[i][i], "NULL");
        for (int j = i + 1; j < nodeNum; ++j)
        {
            QString name;
            if (edges[i][j] == INT16_MAX) {
                name = QString("NULL");
            } else {
                name = QString("edge%1").arg(edgeNum++);
                qDebug() <<"i=" << i
                         << " j=" << j
                         << " name=" << name;
            }
            head[i][j] = Edge(i, j, edges[i][j], name);
            head[j][i] = Edge(i, j, edges[i][j], name);
            if (!edgeMap.contains(name)) edgeMap.insert(name, &head[i][j]);
        }
    }
}

void Graph::addEdge(Edge edge)
{
    int start = edge.Node1();
    int end   = edge.Node2();
    head[start][end] = edge;
    edgeMap.insert(edge.Name(), &edge);
}

std::vector<std::vector<Edge>> Graph::getHead(){ return head; }

unsigned int Graph::size() { return head.size(); }

QString Graph::getTrayName(int start, int end)
{
    return head[start][end].Name();
}

Edge* Graph::getEdge(const QString& name)
{
    if (edgeMap.contains(name))
        return edgeMap[name];
    return nullptr;
}

const QMap<QString, Edge*>& Graph::EdgeMap() { return edgeMap; }
