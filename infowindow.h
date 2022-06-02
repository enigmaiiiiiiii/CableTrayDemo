#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QWidget>
#include <QWindow>
#include "graph.h"

class QTableView;
class QTableWidget;
class QStandardItemModel;
class TabInDock;
class QSpacerItem;

class InfoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWindow(QWidget *parent = nullptr);
    void setupDockWindows();
    QTableWidget* getEdgeTable();

signals:

private:
    void setupEdgeTab();
    void setupLogTab();
    QSize sizeHint() const;

private:

    TabInDock *infoTab;
    Graph *cableGraph;

    QTableWidget *dockTable;
    QStandardItemModel *dataModel;
    QSpacerItem *HSpacer;
};


#endif // INFOWINDOW_H
