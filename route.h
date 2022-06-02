#ifndef CABLEROUTE_H
#define CABLEROUTE_H

#include "graph.h"

class Route
{
public:
    Route(Edge *start, Edge *end);
    ~Route() = default;
    Route()
    void setGraph(Graph *graph);
    void generateRoute();
    QString getPath();
    int getLength() const;
    void addNecessaryPath();
    void addForbiddenPath();
    QString getCableStartName() const;
    QString getCableEndName() const;

private:
    void findShortest(int, int, int&);

private:
    Graph   *cableGraph;
    QString  path;
    int      length;
    Edge*    cableStart;
    Edge*    cableEnd;
    std::vector<int> dist;
};

#endif // CABLEROUTE_H
