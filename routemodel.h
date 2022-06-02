#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QAbstractItemModel>

class RouteModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RouteModel(QObject *parent = nullptr);

};

#endif // ROUTEMODEL_H
