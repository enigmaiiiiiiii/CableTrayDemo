#include "route.h"
#include <vector>
#include <QString>
#include <QStack>
#include <QDebug>

Route::Route(Edge* start, Edge *end)
  : startEdge(start), endEdge(end)
{ }

Route::Route(Graph *graph, const QString &start, const QString &end)
  : cableGraph(graph),
    startEdge(graph->getEdge(start)),
    endEdge(graph->getEdge(end))
{
    generateRoute();
}

void Route::setGraph(Graph *graph) {cableGraph = graph;}

Graph* Route::graph(){return cableGraph;}

void Route::generateRoute()
{
    int currentLength = INT16_MAX;
    // 选择最近端
    findShortest(startEdge->Node1(), endEdge->Node1(), currentLength);
    findShortest(startEdge->Node2(), endEdge->Node1(), currentLength);
    findShortest(startEdge->Node1(), endEdge->Node2(), currentLength);
    findShortest(startEdge->Node2(), endEdge->Node2(), currentLength);

    length = currentLength;
}

// dijkstra算法获得最短
void Route::findShortest(int start, int end, int &currentLength)
{
    if (cableGraph == nullptr)
        return;

    int n = cableGraph->size();
    std::vector<int> visit(n, 0);
    std::vector<int> pathVec(n, -1);
    dist = std::vector<int>(n, 0);

    for (int i = 0; i < n; i++)
    {
        dist[i] = cableGraph->getHead()[start][i].Length();
        if (dist[i] < INT16_MAX)
        {
            pathVec[i] = start;
        }
    }

    visit[start] = 1;

    for (unsigned int i = 0; i < cableGraph->size(); ++i)
    {
        int min_dist = INT16_MAX;
        int middle = 0;
        // 找到基准点middle
        for (unsigned int j = 0; j < cableGraph->size(); ++j)
        {
            if (visit[j] == 0 && dist[j] < min_dist)
            {
                min_dist = dist[j];
                middle = j;
            }
        }
        // 找到基准点middle
        for (unsigned int j = 0; j < cableGraph->size(); j++)
        {

            if (visit[j] == 0 && dist[j] > dist[middle] + cableGraph->getHead()[middle][j].Length())
            {
                dist[j] = dist[middle] + cableGraph->getHead()[middle][j].Length();
                pathVec[j] = middle;
            }
            visit[middle] = 1;
        }
    }

    // 当前找到的最短距离是否是最短距离
    if (currentLength > dist[end])
    {
        path.clear();
        currentLength = dist[end];
        QStack<QString> pathList;

        // 生成路径 
        while (end != start)
        {
            pathList.push(cableGraph->getHead()[pathVec[end]][end].Name());
            end= pathVec[end];
        }
        path.append(startEdge->Name() + "->");
        while (!pathList.empty())
        {
            path.append(pathList.pop() + "->");
        }
        path.append(endEdge->Name());
    }
}

int Route::Length() const { return length; }

QString Route::Path()
{
    return path;
}

void Route::addNecessaryPath()
{ }

void Route::addForbiddenPath()
{ }

Edge* Route::edgeStart() const { return startEdge; }

Edge* Route::edgeEnd() const { return endEdge; }

void Route::setEdgeStart(Edge* edge) {startEdge = edge; }

void Route::setEdgeEnd(Edge* edge) {endEdge= edge; }
