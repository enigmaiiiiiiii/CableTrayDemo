#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QAbstractItemModel>

class RouteModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RouteModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVarian headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    int rc;
    int cc;
    QList<Route*> routesList;
};

#endif // ROUTEMODEL_H
