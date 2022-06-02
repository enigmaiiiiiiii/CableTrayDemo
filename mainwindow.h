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
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool loadRoutes(const QString &f);
    Graph* getGraph();
public slots:
    void fileNew();

private slots:
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
    void buildGraphfromCsv();
    void setupDockWindow();

    void GraphView();
private:
    void setupFileActions();
    void setupEditActions();
    void setupDebugActions();
    void setupTabWidget();
    void setupContextMenuActions();

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
    void getTabModel();

    // data
    void setupRouteActions();
    void generateRoutes();

    // event
    void contextMenuEvent(QContextMenuEvent *event) override;
    void insertRow();
    void addIndexComboBox(const QModelIndex &index);

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
    QTextEdit *textEdit;
    QModelIndexList matchedIndexList;
    int findIndex;
    QTabWidget *tabWidget;

    QTableView *routeTableView;
    QStandardItemModel *exampleModel;
    QStringList headers;
    Graph *graph;

};
#endif // MAINWINDOW_H
