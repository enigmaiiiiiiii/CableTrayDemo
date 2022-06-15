#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "graph.h"


QT_BEGIN_NAMESPACE
class QTableView;
class QAction;
class QTextEdit;
class QStandardItemModel;
class QTabWidget;
class InfoWindow;
class QTreeWidget;
class QSqlTableModel;
class QComboBox;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool loadRoutes(const QString &f);
    Graph* getGraph();

private slots:
    void fileNew();
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void buildGraphfromCsv();
    void setupDockWindow();

private:
    void setupFileActions();
    void setupEditActions();
    void setupTabWidget();
    void setupContextMenuActions();
    void setupTestAction();

    // ui
    void setCurrentFileName(const QString &fileName);
    void find();
    void findNext();
    void findPrevious();
    void updateFindActions();
    void selectAndGoTo(const QModelIndex &index);
    bool maybeSave();
    void tabClose(const int&);
    void tabCreate();

    // data
    void setupRouteActions();
    void generateRoutesFromCsv();
    void caculateRoutes();
    void readEdgeInfo();
    void generateTestData();

    // event
    void contextMenuEvent(QContextMenuEvent *event) override;
    void insertRow();
    void addIndexComboBox(const QModelIndex &index);
    void readRouteAttr();

private:
    QAction *actionSave;
    QAction *actionFind;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionFindNext;
    QAction *actionFindPrevious;
    QAction *actionInsertRow;

    QString fileName;
    QModelIndexList matchedIndexList;
    int findIndex;
    QTabWidget *mainTab;
    QTreeWidget *dockTree;
    InfoWindow *infoWidget;
    QStandardItemModel *fileModel;
    QSqlTableModel *sqlModel;
    QStringList headers;
    Graph *graph;
    QTableView *fileView;
    QTableView *sqlView;
    QComboBox *tableCbx;
};
#endif // MAINWINDOW_H
