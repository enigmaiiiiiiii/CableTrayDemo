#ifndef CABLEROUTE_H
#define CABLEROUTE_H

#include "graph.h"

class Route
{
public:
    Route();
    Route(Edge *start, Edge *end);
    Route(Graph*, const QString&, const QString&);
    ~Route() = default;
    void setGraph(Graph *graph);
    Graph* graph();
    void generateRoute();
    QString Path() const;
    int Length() const;
    void setEdgeStart(Edge*);
    void setEdgeEnd(Edge*);
    Edge* edgeStart() const;
    Edge* edgeEnd() const;


private:
    void findShortest(int, int, int&);

private:
    Graph   *cableGraph;
    QString  path;
    int      length;
    Edge*    startEdge;
    Edge*    endEdge;
    std::vector<int> dist;
};

#endif // CABLEROUTE_H
