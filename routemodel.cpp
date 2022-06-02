#include "routemodel.h"
#include "route.h"

RouteModel::RouteModel(QObject *parent)
    : QAbstractItemModel{parent},
{
}


QModelIndex RouteModel::index(int row, int column, const QModelIndex &parent) const 
{
    Q_UNUSED(parent);
    return createIndex(row, column);
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
            return QString("%1").arg(index.row());
            case 1:
                return QVariant(routes[index.row()]->geCableStartName());
            case 2:
                return QVariant(routes[index.row()]->geCableEndName());
            case 3:
                return QVariant(routes[index.row()]->geCableLength());
            case 4:
                return QVariant(routes[index.row()]->gePath());
            default:
                return QVariant();
        }
    }
    return QVariant();
}
int RouteModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return routes.size();
}

int RouteModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

Qt::ItemFlags RouteModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

bool Route::addRoute(const Route *value)
{
    row = RoutesList.size();
    RouteList.append(value);

    QModelIndex c0 = createIndex(row, 0);
    QModelIndex c1 = createIndex(row, 1);
    QModelIndex c2 = createIndex(row, 2);
    QModelIndex c3 = createIndex(row, 3);
    QModelIndex c4 = createIndex(row, 4);

    setData(c0, c0.row(), role);
    setData(c1, value.getCableStart());
    setData(c2, value.getCableEndName());
    setData(c3, value.getLength());
    setData(c4, value.getPath());

    return true;
}