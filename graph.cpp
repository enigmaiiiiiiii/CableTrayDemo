#include "graph.h"
#include <QString>
#include <QSet>

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
  : TrayName("NULL"),
    node1(0),
    node2(0),
    length(INT16_MAX)
{
}
Edge::Edge(int n1, int n2, int length, const QString &name)
    : TrayName(name),
      node1(n1),
      node2(n2),
      length(length)
{
}

Edge &Edge::operator=(const Edge &edge)
{
    TrayName = edge.TrayName;
    node1 = edge.node1;
    node2 = edge.node2;
    length = edge.length;

    return *this;
}

QString Edge::getName() {return TrayName;}
int Edge::getNode1() const { return node1; }
int Edge::getNode2() const { return node2; }
int Edge::getLength() const { return length; }

// 以命名边生成Graph
Graph::Graph(std::vector<Edge> &edges)
{
    // 统计相关节点个数
    QSet<int> nodes;
    for (auto &edge : edges)
    {
        nodes.insert(edge.getNode1());
        nodes.insert(edge.getNode2());
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
        edgeMap.insert(edge.getName(), &edge);
        int start = edge.getNode1();
        int end   = edge.getNode2();

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
            }

            head[i][j] = Edge(i, j, edges[i][j], name);
            head[j][i] = Edge(i, j, edges[i][j], name);
            if (edgeMap.find(name) == edgeMap.end()) edgeMap.insert(name, &head[i][j]);
        }
    }
}

void Graph::addEdge(Edge edge)
{
    int start = edge.getNode1();
    int end   = edge.getNode2();
    head[start][end] = edge;
    edgeMap.insert(edge.getName(), &edge);
}

std::vector<std::vector<Edge>> Graph::getHead(){ return head; }

unsigned int Graph::size() { return head.size(); }

QString Graph::getTrayName(int start, int end)
{
    return head[start][end].getName();
}

Edge* Graph::getEdge(const QString& name)
{
    return edgeMap[name];
}

QMap<QString, Edge*> Graph::getEdgeMap()
{
    return edgeMap;
}
