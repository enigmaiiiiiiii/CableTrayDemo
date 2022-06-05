#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H


#include <QStandardItem>

class Route;
class RouteModel;
// class Edge;

class AttrModel : public QStandardItemModel
{
    Q_OBJECT
public:
    AttrModel(Route*, QObject* parent = nullptr);

private:
    Route *route;
};

class RouteModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit RouteModel(QObject *parent = nullptr);

    // reimplement
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // data 
    bool addRoute(Route *value);

private:
    QList<Route*> routeList;  // 每一个节点代表一个树

};

#endif // ROUTEMODEL_H
