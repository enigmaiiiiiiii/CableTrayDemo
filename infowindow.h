#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QWidget>
#include "graph.h"

class QTableView;
class QTableWidget;
class QStandardItemModel;

class InfoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWindow(QWidget *parent = nullptr);
    void setupDockWindows();

signals:

private:
    Graph *cableGraph;
    QTableView *graphTableView;
    QTableWidget *dockTable;
    QStandardItemModel *dataModel;
};


#endif // INFOWINDOW_H
