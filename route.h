#ifndef CABLEROUTE_H
#define CABLEROUTE_H

#include "graph.h"

class Route
{
public:
    Route(Edge *start, Edge *end);
    void setGraph(Graph *graph);
    void generateRoute();
    void findShortest(int, int, int&);
    QString getPath();
    int getLength() const;
    void addNecessaryPath();
    void addForbiddenPath();

private:
    Graph   *cableGraph;
    QString  path;
    int      length;
    Edge*    cableStart;
    Edge*    cableEnd;
    std::vector<int> dist;
};

#endif // CABLEROUTE_H
