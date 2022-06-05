#include "routemodel.h"

#include "route.h"
#include <QModelIndex>

AttrModel::AttrModel(Route *root, QObject *parent)
    : QStandardItemModel(parent),
      route(root)
{
}

RouteModel::RouteModel(QObject *parent)
  : QStandardItemModel{parent}
{

}

// 获取数据
QVariant RouteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QVariant(routeList[index.row()]->edgeStart()->Name());
        case 1:
            return QVariant(routeList[index.row()]->edgeEnd()->Name());
        case 2:
            return QVariant(routeList[index.row()]->Length());
        case 3:
            return QVariant(routeList[index.row()]->Path());
        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool RouteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    Route *route = routeList[index.row()];
    Graph *graph = route->graph();

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0:
            route->setEdgeStart(graph->getEdge(value.toString()));
            break;
        case 1:
            route->setEdgeEnd(graph->getEdge(value.toString()));
            break;
        default:
            return false;
        }
        // 根据新起点, 新终点更新路径
        routeList[index.row()]->generateRoute();
        emit dataChanged(index, createIndex(index.row(), 3));
        return true;
    }
    return false;
}

QVariant RouteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return QString("起点");
        case 1:
            return QString("终点");
        case 2:
            return QString("长度");
        case 3:
            return QString("路径");
        }
    }
    return QVariant();
}

// 直接插入route数据
bool RouteModel::addRoute(Route *item)
{
    int row = routeList.size();
    routeList << item;

    QModelIndex c0 = createIndex(row, 0);
    QModelIndex c1 = createIndex(row, 1);
    QModelIndex c2 = createIndex(row, 2);
    QModelIndex c3 = createIndex(row, 3);

    setData(c0, item->edgeStart()->Name());
    setData(c1, item->edgeEnd()->Name());
    setData(c2, item->Length());
    setData(c3, item->Path());

    return true;
}
