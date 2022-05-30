#include "mainwindow.h"
#include "route.h"
#include "infowindow.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QMimeDatabase>
#include <QStringDecoder>
#include <QStandardItemModel>
#include <QTableView>
#include <QDockWidget>
#include <QTreeWidget>
#include <QInputDialog>
#include <QTabWidget>
#include <QMetaObject>

const QString rsrcPath = ":/image";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    headers << "起点"
            << "终点"
            << "长度"
            << "路径";
    QList<QList<int>> graphInit = {
        {0, 2, INT16_MAX, 5, INT16_MAX, INT16_MAX},
        {2, 0, 1, 6, 4, INT16_MAX},
        {INT16_MAX, INT16_MAX, 0, INT16_MAX, 5, 8},
        {5, 6, INT16_MAX, 0, 2, INT16_MAX},
        {INT16_MAX, 4, 5, 2, 0, 6},
        {INT16_MAX, INT16_MAX, 8, INT16_MAX, 6, 0}
    };
    graph = new Graph(graphInit);

    // TabWidget
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);

    setCentralWidget(tabWidget);
    tabWidget->setVisible(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::tabClose);
    showMaximized();

    // 文件动作
    setupFileActions();
    setupEditActions();
    setupDebugActions();
    setupContextMenuActions();
    setupDockWindow();

    setCurrentFileName(QString());

    setupRouteActions();
}

MainWindow::~MainWindow() {}

// 文件操作相关的Action
void MainWindow::setupFileActions()
{
    QToolBar *tb = addToolBar("File");
    QMenu *menu = menuBar()->addMenu("&File");

    // 新建文件
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
    auto a = menu->addAction(newIcon, "&New", this, &MainWindow::fileNew); // menu添加
    a->setShortcut(QKeySequence::New);
    a->setPriority(QAction::LowPriority);
    tb->addAction(a); // 工具栏添加

    // 打开文件
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png")); // 后备方案
    a = menu->addAction(openIcon, "&Open...", this, &MainWindow::fileOpen);
    a->setShortcut(QKeySequence::Open);
    tb->addAction(a);

    // 分隔线
    menu->addSeparator();

    // 保存文件
    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
    actionSave = menu->addAction(saveIcon, tr("&Save"), this, &MainWindow::fileSave);
    actionSave->setShortcut(QKeySequence::Save);
    //    actionSave->setEnabled(false);
    tb->addAction(actionSave);

    // 退出
    a = menu->addAction(tr("&Quit"), this, &QWidget::close);
    a->setShortcut(Qt::CTRL | Qt::Key_Q);
}

void MainWindow::setupEditActions()
{
    QToolBar *tb = addToolBar(tr("Edit Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&Edit"));

    // 查找
    QAction *findAction = menu->addAction(tr("&Find"), this, &MainWindow::find);
    findAction->setShortcut(QKeySequence::Find);

    actionFindNext = menu->addAction(tr("Find &Next"), this, &MainWindow::findNext);
    actionFindNext->setShortcuts(QKeySequence::FindNext);
    actionFindPrevious = menu->addAction(tr("Find &Previous"), this, &MainWindow::findPrevious);
    actionFindPrevious->setShortcuts(QKeySequence::FindPrevious);
    actionFindNext->setEnabled(!matchedIndexList.isEmpty());
    actionFindPrevious->setEnabled(!matchedIndexList.isEmpty());

    menu->addSeparator();

    // 撤销
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
    //    actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
    actionUndo = menu->addAction(undoIcon, tr("&Undo"));
    actionUndo->setShortcut(QKeySequence::Undo);
    tb->addAction(actionUndo);

    // 取消撤销
    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
    //    actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
    actionRedo = menu->addAction(redoIcon, tr("&Redo"));
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    tb->addAction(actionRedo);

    menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
    //    actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
    actionCut = menu->addAction(cutIcon, tr("Cu&t"));
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    tb->addAction(actionCut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
    //    actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
    actionCopy = menu->addAction(copyIcon, tr("&Copy"));
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    tb->addAction(actionCopy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
    //    actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
    actionPaste = menu->addAction(pasteIcon, tr("&Paste"));
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    tb->addAction(actionPaste);
    if (const QMimeData *md = QGuiApplication::clipboard()->mimeData()) // 获得剪切板的mimeData()
        actionPaste->setEnabled(md->hasText());                         // 设置粘贴action是否可用
#endif
}

void MainWindow::setupRouteActions()
{
    QMenu *menu = menuBar()->addMenu(tr("路径"));

    QIcon icongraph = QIcon(rsrcPath + "/smallicon.png");
    QAction *a = new QAction(icongraph, tr("导入csv"));
    connect(a, &QAction::triggered, this, &MainWindow::buildGraphfromCsv);
    menu->addAction(a);

    QAction *b = new QAction(icongraph, tr("生成路径"));
    connect(b, &QAction::triggered, this, &MainWindow::generateRoutes);
    menu->addAction(b);

}

void MainWindow::setupContextMenuActions()
{
    actionInsertRow = new QAction(tr("Insert row"), this);
    connect(actionInsertRow, &QAction::triggered, this, &MainWindow::insertRow);
}

void MainWindow::fileNew()
{
    QTableView *tableView = new QTableView(tabWidget);
    QStandardItemModel *model = new QStandardItemModel(tabWidget);

    model->setHorizontalHeaderLabels(headers);
    tableView->setModel(model);

    int cur = tabWidget->addTab(tableView, "");
    tabWidget->setCurrentIndex(cur);
    setCurrentFileName("");
}

// 打开文件
void MainWindow::fileOpen()
{
    QFileDialog fileDialog(this, tr("Open File..."));

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    fileDialog.setMimeTypeFilters({"text/csv"});
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().constFirst();
    if (loadRoutes(fn))
    {
        statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
    }
    else
        statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
}

// 保存文件数据到成员dataModel
bool MainWindow::loadRoutes(const QString &f)
{
    if (!QFile::exists(f))
        return false;

    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    // 通过文件名f和data联合推断mime类型, 文件扩展名优先于内容
    QByteArray data = file.readAll();
    QMimeDatabase db;
    const QString &mimeTypeName = db.mimeTypeForFileNameAndData(f, data).name();

    // 读内容到Model
    if (mimeTypeName == u"text/csv") // 判断类型是否为csv
    {
        QTableView *curView;
        QStandardItemModel *curModel;
        if (f == fileName) {  // 重新打开
            curView = static_cast<QTableView *>(tabWidget->currentWidget());
            curModel = static_cast<QStandardItemModel *>(curView->model());
        } else {  //
            curModel = new QStandardItemModel(tabWidget);
            curModel->setHorizontalHeaderLabels(headers);

            curView = new QTableView(tabWidget);
            curView->setModel(curModel);
        }

        QList<QByteArray> lines = data.split('\n');
        for (int r = 1; r < lines.size(); ++r)
        {
            QList<QByteArray> dataList = lines.at(r).split(',');
            QString data;
            int flag = 0;
            for (int c = 0; c < dataList.size(); ++c)
            {
                data = dataList.at(c).trimmed();
                if (data.contains("\"") && flag == 0)
                {
                    flag = 1;
                    data += ",";
                }
                else if (flag == 1)
                {
                    if (!data.contains("\""))
                    {

                        data += dataList.at(c) + ',';
                    }
                    else
                    {
                        data += dataList.at(c);
                        QStandardItem *item = new QStandardItem(data.replace("\"", ""));
                        curModel->setItem(r - 1, c, item);
                        flag = 0;
                    }
                }
                else
                {
                    QStandardItem *item = new QStandardItem(data);
                    curModel->setItem(r - 1, c, item);
                }
            }
        }

        // 添加标签
        int cur = tabWidget->addTab(curView, QFileInfo(f).baseName()); // tab index
        tabWidget->setCurrentIndex(cur);
    }

    setCurrentFileName(f);
    file.close();
    return true;
}

// 设置当前文件名
void MainWindow::setCurrentFileName(const QString &f)
{
    fileName = f;

    // textEdit->document()->setModified(false);
    QString shownName = f;
    if (fileName.isEmpty())
        shownName = "untitled";
    else
        shownName = QFileInfo(fileName).baseName();

    int cur = tabWidget->currentIndex();
    tabWidget->setTabText(cur, shownName);
    setWindowModified(false);
}

// 保存文件
bool MainWindow::fileSave()
{
    if (fileName.isEmpty() || fileName.startsWith(u":/"))
        return fileSaveAs(); // 另存

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        statusBar()->showMessage(tr("Could not write to file %1")
                                     .arg(QDir::toNativeSeparators(fileName)));
        return false;
    }

    QTableView *tableView = static_cast<QTableView *>(tabWidget->currentWidget());
    QStandardItemModel *model = static_cast<QStandardItemModel *>(tableView->model());

    QTextStream out(&file);

    // 保存 首行
    for (int i = 0; i < model->columnCount(); ++i)
    {
        QString header = model->headerData(i, Qt::Horizontal).toString();
        out << header;
        if (i < model->columnCount() - 1)
            out << ",";
        else
            out << "\n";
    }

    for (int r = 0; r < model->rowCount(); ++r)
    {
        for (int c = 0; c < model->columnCount(); ++c)
        {
            QVariant item = model->data(model->index(r, c)).toString();
            if (item.toBool())
            {
                if (item.toString().contains(','))
                {
                    out << "\"" << item.toString() << "\"";
                }
                else
                {
                    out << item.toString();
                }
            }
            if (c < model->columnCount() - 1) {
                out << ",";
            } else {
                if (r < model->rowCount() - 1)
                {
                    out << "\r\n";
                }
            }
        }
    }
    statusBar()->showMessage(tr("Saved \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    return true;
}

// 是否保存
bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

bool MainWindow::fileSaveAs()
{
    QFileDialog fileDialog(this, tr("save as..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList mimeTypes("text/plain", "text/csv");
    fileDialog.setMimeTypeFilters(mimeTypes);

    if (fileDialog.exec() != QDialog::Accepted)
        return false;

    const QString fn = fileDialog.selectedFiles().constFirst();

    setCurrentFileName(fn);
    return fileSave();
}

void MainWindow::tabClose(const int &index)
{
    if (index == -1)
    {
        return;
    }

    QWidget *tabItem = tabWidget->widget(index);
    // Removes the tab at position index from this stack of widgets.
    // The page widget itself is not deleted.
    tabWidget->removeTab(index);

    delete tabItem;
}

void MainWindow::tabCreate()
{
    QTableView *tableView = new QTableView(tabWidget);
    QStandardItemModel *model = new QStandardItemModel(tabWidget);
    model->setHorizontalHeaderLabels(headers);
    tableView->setModel(model);

    int cur = tabWidget->addTab(tableView, fileName); // tab index
    tabWidget->setCurrentIndex(cur);
}

void MainWindow::setupDebugActions()
{
    QMenu *menu = menuBar()->addMenu("Debug");
    QToolBar *tb = addToolBar("debug");
    QAction *action = new QAction(QIcon(rsrcPath + "/smallicon.png"), tr("Debug"), this);

    action->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(action, &QAction::triggered, this, &MainWindow::GraphView);
    menu->addAction(action);
    tb->addAction(action);
}

void MainWindow::GraphView()
{
    InfoWindow *graphInfo = new InfoWindow();

    graphInfo->show();
}

void MainWindow::setupDockWindow()
{
    QDockWidget *dock = new QDockWidget(tr("特性"), this);
    QTreeWidget *dockTree = new QTreeWidget(this);

    QStringList headers;
    headers << "attr"
            << "val";
    dockTree->setHeaderLabels(headers);
    dock->setWidget(dockTree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->hide();

    QAction *a = dock->toggleViewAction();
    a->setShortcut(Qt::ALT | Qt::Key_1);
    addAction(a);
}

void MainWindow::find()
{
    QInputDialog inputDialog(this);

    inputDialog.setWindowTitle(tr("Find"));
    inputDialog.setLabelText(tr("text"));

    // Accepted = 1, Rejected = 0
    if (inputDialog.exec() != QDialog::Accepted)
    {           // 显示dialog, 并阻塞
        return; // return if dialog is rejected
    }
    const QString value = inputDialog.textValue().trimmed();
    if (value.isEmpty())
        return;

    matchedIndexList.clear();
    findIndex = 0;

    QTableView *tableView = static_cast<QTableView *>(tabWidget->currentWidget());
    QStandardItemModel *model = static_cast<QStandardItemModel *>(tableView->model());

    const QList<QStandardItem *> items =
        model->findItems(value, Qt::MatchContains |
                                    Qt::MatchRecursive |
                                    Qt::MatchFixedString);
    for (auto &item : items)
    {
        matchedIndexList.append(model->indexFromItem(item));
    }
    statusBar()->showMessage(tr("%1 items found").arg(matchedIndexList.size()));
    updateFindActions();
    if (!matchedIndexList.isEmpty())
    {
        selectAndGoTo(matchedIndexList.constFirst()); // constFirst list的第一个元素
    }
}

void MainWindow::findNext()
{
    findIndex++;
    if (findIndex >= matchedIndexList.size())
    {
        statusBar()->showMessage(tr("返回首个找到的items"), 2000);
        findIndex = 0;
    }
    if (findIndex < matchedIndexList.size())
    {
        selectAndGoTo(matchedIndexList.at(findIndex));
    }
}

void MainWindow::findPrevious()
{
    findIndex--;
    if (findIndex < 0)
    {
        findIndex = matchedIndexList.size() - 1;
    }
    if (findIndex >= 0)
    {
        selectAndGoTo(matchedIndexList.at(findIndex));
    }
}

void MainWindow::updateFindActions()
{
    const bool findNextPreviousEnabled = !matchedIndexList.isEmpty();
    actionFindNext->setEnabled(findNextPreviousEnabled);
    actionFindPrevious->setEnabled(findNextPreviousEnabled);
}

// 跳转选择
void MainWindow::selectAndGoTo(const QModelIndex &index)
{
    QTableView *tableView = static_cast<QTableView*>(tabWidget->currentWidget());

    tableView->scrollTo(index, QAbstractItemView::PositionAtCenter);
    tableView->setCurrentIndex(index); // 并被selected, 如果可以
}

void MainWindow::buildGraphfromCsv()
{
    QFileDialog fileDialog(this, tr("Open File..."));

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    fileDialog.setMimeTypeFilters({"text/csv"});
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().constFirst();

    QFile file(fn);
    if (!file.open(QFile::ReadOnly))
        return;

    QList<QList<int>> graphList;
    while (!file.atEnd())
    {
        graphList.append(QList<int>());
        QList<QByteArray> row = file.readLine().split(',');
        for (auto &i : row)
        {
            graphList.last().append(i.toInt());
        }
    }
    graph = new Graph(graphList);
    return;
}

void MainWindow::generateRoutes()
{
    QTableView *tableView = static_cast<QTableView *>(tabWidget->currentWidget());
    QStandardItemModel *model = static_cast<QStandardItemModel*>(tableView->model());

    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString edge1 = model->data(model->index(i, 0)).toString();
        QString edge2 = model->data(model->index(i, 1)).toString();

        Edge* startEdge = graph->getEdge(edge1);
        Edge* endEdge = graph->getEdge(edge2);
        if (!startEdge || !endEdge) continue;

        Route *route = new Route(startEdge, endEdge);

        route->setGraph(graph);
        route->generateRoute();

        model->setData(model->index(i, 2), route->getLength());
        model->setData(model->index(i, 3), route->getPath());
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(actionInsertRow);
    menu.exec(event->globalPos());
}

void MainWindow::insertRow()
{
    QTableView *tableView = static_cast<QTableView *>(tabWidget->currentWidget());
    QStandardItemModel *model = static_cast<QStandardItemModel*>(tableView->model());
    model->insertRow(model->rowCount());
}

//void MainWindow::addIndexComboBox(const QModelIndex &index)
//{
//    if (index.column() < 2)
//    {
//        QComboBox *edgeBox = new QComboBox(this);
//        edgeBox->addItems(graph->getEdgeMap()->keys());

//        QTableView *curView = static_cast<QTableView*>(tabWidget->currentWidget());
//        QStandardItemModel *curModel = static_cast<QStandardItemModel*>(curView->model());
//        curView->setIndexWidget(index, edgeBox);
//        curModel->setData(index, edgeBox->currentData());
//    }

//}
