#include "dbconnector.h"
#include "graph.h"
#include "route.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFile>
#include <QString>
#include <QSqlError>

DbConnector::DbConnector(const QString& fname)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(fname);
}

DbConnector::~DbConnector()
{
}

QSqlDatabase DbConnector::initDb()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.open()) return QSqlDatabase();
    QSqlQuery query(db);

    if (!db.tables().contains("Node")) {
        query.exec("CREATE TABLE Node(id int NOT NULL PRIMARY KEY,"
                   "x double,"
                   "y double,"
                   "z double,"
                   "description varchar(200))");
    }
    if (!db.tables().contains("Edge")) {
        query.exec("CREATE TABLE Edge(id int NOT NULL PRIMARY KEY,"
                   "name VARCHAR(20),"
                   "node_1 int,"
                   "node_2 int,"
                   "length int,"
                   "capacity int,"
                   "material varchar(20),"
                   "appearence varchar(20),"
                   "description varchar(200),"
                   "FOREIGN KEY (node_1) REFERENCES Node(id),"
                   "FOREIGN KEY (node_2) REFERENCES Node(id))");
    }
    if (!db.tables().contains("GRAPH")) {
        query.exec("CREATE TABLE Graph(id int NOT NULL PRIMARY KEY,"
                   "size int,"
                   "NecessaryEdge varchar(200),"
                   "ForbiddenEdge varchar(200),"
                   "description varchar(200))");
    }
    if (!db.tables().contains("Route")) {
        query.exec("CREATE TABLE Route(id int NOT NULL PRIMARY KEY,"
                   "startEdge varchar(20),"
                   "endEdge varchar(20),"
                   "Length int,"
                   "Path varchar(200),"
                   "description varchar(200))");
                //    "id_se int,"
                //    "id_ee int,"
                //    "FOREIGN KEY(id_se) REFERENCES Edge(id),"
                //    "FOREIGN KEY(id_ee) REFERENCES Edge(id))");
    }
    return db;
}

void DbConnector::addNodeRecord(Node* node, const QString &des)
{
    int id = node->getId();
    double x = node->getPos().x;
    double y = node->getPos().y;
    double z = node->getPos().z;

    QString q = QString("INSERT INTO Node VALUES (%1, %2, %3, %4, '%5')")
                    .arg(id)
                    .arg(x)
                    .arg(y)
                    .arg(z)
                    .arg(des);
    QSqlDatabase db = QSqlDatabase::database();  // Duplicate remove and add
    QSqlQuery query(db);
    query.exec(q);
}

void DbConnector::addEdgeRecord(Edge* edge, const QString &des)
{
    QSqlDatabase db = QSqlDatabase::database();  // Duplicate remove and add
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM Edge");
    query.first();
    int id = query.value(0).toInt() + 1;
    QString name = edge->Name();
    int node1 = edge->Node1();
    int node2 = edge->Node2();
    int length = edge->Length();
    int capacity = edge->Capacity();
    QString material = edge->Material();
    QString apperence = edge->Appearence();
    QString q = QString("INSERT INTO Edge VALUES (%1, '%2', %3, %4, %5, %6, '%7', '%8', '%9')")
                    .arg(id)
                    .arg(name)
                    .arg(node1)
                    .arg(node2)
                    .arg(length)
                    .arg(capacity)  // 6 int
                    .arg(material)
                    .arg(apperence)
                    .arg(des);
    query.exec(q);
}

void DbConnector::addGraphRecord(Graph* graph, const QString &des)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec("select count(*) from Graph");
    int id = query.value(0).toInt() + 1;
    int size = graph->size();
    QString q = QString("INSERT INTO Graph VALUES (%1, %2, '%3','%4', '%5')")
                    .arg(id)
                    .arg(size)
                    .arg(graph->necessaryEdge().join(","))
                    .arg(graph->forbiddenEdge().join(","))
                    .arg(des);
    query.exec(q);
}

void DbConnector::addRouteRecord(Route *route, const QString &des)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM Route");
    query.first();
    int id = query.value(0).toInt() + 1;
    QString startEdge = route->edgeStart()->Name();
    QString endEdge = route->edgeEnd()->Name();
    int length = route->Length();
    QString path = route->Path();


    QString q = QString("INSERT INTO Route VALUES (%1, '%2', '%3', %4, '%5', '%6')")
                    .arg(id)
                    .arg(startEdge)
                    .arg(endEdge)
                    .arg(length)
                    .arg(path)
                    .arg(des);
    query.exec(q);
//    qDebug() << query.lastError().text();
}

void DbConnector::addEdgeRecord(int id, const QString &name,
                                int node_1, int node_2,
                                int Length, int capacity,
                                const QString &material,
                                const QString &appearence,
                                const QString &des)
{
    QString q = QString("INSERT INTO Edge VALUES (%1, '%2', %3, %4, %5, '%6', '%7', '%8', '%9')")
                    .arg(id)
                    .arg(name)
                    .arg(node_1)
                    .arg(node_2)
                    .arg(Length)
                    .arg(capacity)
                    .arg(material)
                    .arg(appearence)
                    .arg(des);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec(q);
}



void DbConnector::addRouteRecord(int id,
                                 const QString &startEdge,
                                 const QString &endEdge,
                                 const QString &path,
                                 int length,
                                 const QString &des )
{
    QString q = QString("INSERT INTO Route VALUES (%1, '%2', '%3', '%4', %5, '%6')")
                    .arg(id)
                    .arg(startEdge)
                    .arg(endEdge)
                    .arg(path)
                    .arg(length)
                    .arg(des);
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec(q);
}


void DbConnector::checkRepeatRecord()
{
}
