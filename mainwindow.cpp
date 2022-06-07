#include "mainwindow.h"
#include "routemodel.h"
#include "graph.h"
#include "route.h"
#include "infowindow.h"
#include "helper.h"
#include "comboboxdelegate.h"

#include <QStandardItemModel>
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
#include <QTableView>
#include <QDockWidget>
#include <QTreeWidget>
#include <QInputDialog>
#include <QTabWidget>
#include <QMetaObject>
#include <QSplitter>
#include <QTableWidgetItem>

const QString rsrcPath = ":/image";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // qssfile
//    QFile *qssFile = new QFile(":/qss/style.qss", this);
//    qssFile->open(QFile::ReadOnly);
//    QString styleSheet = tr(qssFile->readAll());
//    setStyleSheet(styleSheet);
//    qssFile->close();

    // graph
    QList<QList<int>> graphInit = Helper::listForGraph(30);
    graph = new Graph(graphInit);

    // 主窗口tab
    mainTab = new QTabWidget(this);
    mainTab->setTabsClosable(true);
    mainTab->setVisible(true);
    setCentralWidget(mainTab);
    connect(mainTab, &QTabWidget::tabCloseRequested, this, &MainWindow::tabClose);

    // 主窗口动作
    setupFileActions();
    setupEditActions();
    setupDockWindow();
    setupContextMenuActions();
    setCurrentFileName(QString());
    readEdgeInfo();
    statusBar();

    showMaximized();

    // 功能动作
    setupRouteActions();
    headers << "起点"
            << "终点"
            << "长度"
            << "路径";
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
    a->setStatusTip(tr("打开csv文件"));
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

    QAction *b = new QAction(icongraph, tr("csv生成路径"));
    connect(b, &QAction::triggered, this, &MainWindow::generateRoutesFromCsv);
    b->setStatusTip(tr("打开csv文件并生成路径"));
    menu->addAction(b);

    QAction *c = new QAction(icongraph, tr("计算路径"));
    connect(c, &QAction::triggered, this, &MainWindow::caculateRoutes);
    menu->addAction(c);
}

void MainWindow::setupContextMenuActions()
{
    actionInsertRow = new QAction(tr("Insert row"), this);
    connect(actionInsertRow, &QAction::triggered, this, &MainWindow::insertRow);
}

void MainWindow::setupDockWindow()
{
    // 左侧
    QDockWidget *attrDock = new QDockWidget(tr("特性"), this);
    dockTree = new QTreeWidget(this);

    QStringList attrHeaders;
    attrHeaders << "attr"
                << "val";
    dockTree->setHeaderLabels(attrHeaders);
    attrDock->setWidget(dockTree);
    addDockWidget(Qt::LeftDockWidgetArea, attrDock);

    QAction *a = attrDock->toggleViewAction();
    a->setShortcut(Qt::CTRL | Qt::Key_1);
    addAction(a);

    // 右侧停靠窗口 
    QDockWidget *infoDock = new QDockWidget(tr(""), this);
    addDockWidget(Qt::RightDockWidgetArea, infoDock);
    infoWidget = new InfoWindow(this);
    infoDock->setWidget(infoWidget);

    QAction *b = infoDock->toggleViewAction();
    b->setShortcut(Qt::CTRL | Qt::Key_2);
    addAction(b);

//    resizeDocks({attrDock, infoDock}, {geometry().width(), geometry().width()},Qt::Horizontal);
}

void MainWindow::fileNew()
{
    QTableView *tableView = new QTableView(mainTab);
    QStandardItemModel *model = new QStandardItemModel(mainTab);

    tableView->setModel(model);

    int cur = mainTab->addTab(tableView, "");
    mainTab->setCurrentIndex(cur);
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
    if (mimeTypeName != u"text/csv") { // 判断类型是否为csv 
        return false;
    }

    QTableView *curView;
    QStandardItemModel *curModel = new QStandardItemModel(mainTab);;
    curModel->setHorizontalHeaderLabels(headers);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(curModel);
    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, &MainWindow::readRouteAttr);


    if (f != fileName) {  // 重新打开
        curView = new QTableView(mainTab);
        ComboBoxDelegate *cboDelegate = new ComboBoxDelegate(graph->EdgeMap().keys());
        curView->setItemDelegateForColumn(0, cboDelegate);
        curView->setItemDelegateForColumn(1, cboDelegate);
        int cur = mainTab->addTab(curView, QFileInfo(f).fileName()); // tab index
        mainTab->setCurrentIndex(cur);
    } else {
        curView = static_cast<QTableView*>(mainTab->currentWidget());
    }

    curView->setModel(curModel);
    curView->setSelectionModel(selectionModel);

    QList<QByteArray> lines = data.split('\n');
    for (int r = 1; r < lines.size(); ++r) {
        QList<QByteArray> dataList = lines.at(r).split(',');
        QString data;
        int flag = 0;
        for (int c = 0; c < dataList.size(); ++c) {
            data = dataList.at(c).trimmed();
            if (data.contains("\"") && flag == 0) {
                flag = 1;
                data += ",";
            }
            else if (flag == 1) {
                if (!data.contains("\"")) {
                    data += dataList.at(c) + ',';
                } else {
                    data += dataList.at(c);
                    QStandardItem *item = new QStandardItem(data.replace("\"", ""));
                    curModel->setItem(r - 1, c, item);
                    flag = 0;
                }
            } else {
                QStandardItem *item = new QStandardItem(data);
                curModel->setItem(r - 1, c, item);
            }
        }
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

    int cur = mainTab->currentIndex();
    mainTab->setTabText(cur, shownName);
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

    QTableView *tableView = static_cast<QTableView *>(mainTab->currentWidget());
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
                    out << "\r\n";  // 非最后一行后加换行
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

    QWidget *tabItem = mainTab->widget(index);
    // Removes the tab at position index from this stack of widgets.
    // The page widget itself is not deleted.
    mainTab->removeTab(index);

    delete tabItem;
}

//	void MainWindow::tabCreate()
//	{
//	    QTableView *tableView = new QTableView(mainTab);
//	    QStandardItemModel *model = new QStandardItemModel(mainTab);
//	    model->setHorizontalHeaderLabels(headers);
//	    tableView->setModel(model);

//	    int cur = mainTab->addTab(tableView, fileName); // tab index
//	    mainTab->setCurrentIndex(cur);
//	}

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

    QTableView *tableView = static_cast<QTableView *>(mainTab->currentWidget());
    QStandardItemModel *model = static_cast<QStandardItemModel*>(tableView->model());

    for (int j = 0; j < model->columnCount(); ++j)
    {
        const QList<QStandardItem *> items =
            model->findItems(value, Qt::MatchContains |
                                        Qt::MatchRecursive |
                                        Qt::MatchFixedString, j);
        for (auto &item : items)
        {
            matchedIndexList.append(model->indexFromItem(item));
        }

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
    QTableView *tableView = static_cast<QTableView*>(mainTab->currentWidget());

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
    readEdgeInfo();
    return;
}

void MainWindow::generateRoutesFromCsv()
{
    QFileDialog fileDialog(this, tr("打开路径文件..."));

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters({"text/csv"});

    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().constFirst();

    QFile file(fn);
    if (!file.open(QFile::ReadOnly))
        return;

    QByteArray data = file.readAll();
    QMimeDatabase db;
    const QString &mimeTypeName = db.mimeTypeForFileNameAndData(fn, data).name();

    if (mimeTypeName != u"text/csv")
    {
        return;
    }

    QTableView *curView = new QTableView();
    curView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStandardItemModel *curModel = new QStandardItemModel(mainTab);
    curModel->setHorizontalHeaderLabels(headers);
    QItemSelectionModel *selectionModel = new QItemSelectionModel(curModel);
    ComboBoxDelegate *cboDelegate = new ComboBoxDelegate(graph->EdgeMap().keys());

    curView->setModel(curModel);
    curView->setSelectionModel(selectionModel);
    curView->setItemDelegateForColumn(0, cboDelegate);
    curView->setItemDelegateForColumn(1, cboDelegate);
    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, &MainWindow::readRouteAttr);

    QList<QByteArray> lines = data.split('\n');
    // 只读前两列
    for (int r = 1; r < lines.size(); ++r)
    {
        QList<QByteArray> dataList = lines.at(r).split(',');

        QString edgeName_1 = dataList.at(0).trimmed();
        QString edgeName_2 = dataList.at(1).trimmed();
        if (!graph->EdgeMap().contains(edgeName_1) ||
            !graph->EdgeMap().contains(edgeName_2)) {
            continue;
        }

        Route *route = new Route(graph, edgeName_1, edgeName_2);
        QStandardItem *item1 = new QStandardItem(edgeName_1);
        QStandardItem *item2 = new QStandardItem(edgeName_2);
        QStandardItem *item3 = new QStandardItem(QString("%1").arg(route->Length()));
        QStandardItem *item4 = new QStandardItem(route->Path());

        curModel->setItem(r - 1, 0, item1);
        curModel->setItem(r - 1, 1, item2);
        curModel->setItem(r - 1, 2, item3);
        curModel->setItem(r - 1, 3, item4);
    }

    // 添加标签
    int cur = mainTab->addTab(curView, QFileInfo(fn).baseName()); // tab index
    mainTab->setCurrentIndex(cur);

    setCurrentFileName(fn);
    file.close();
}

// 生成最短路径和长度
void MainWindow::caculateRoutes()
{
    QTableView *tableView = static_cast<QTableView *>(mainTab->currentWidget());
    if (!tableView) return;
    QStandardItemModel *model = static_cast<QStandardItemModel*>(tableView->model());
    if (!model) return;

    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString edge1 = model->data(model->index(i, 0)).toString();
        QString edge2 = model->data(model->index(i, 1)).toString();

        Edge* startEdge = graph->getEdge(edge1);
        Edge* endEdge = graph->getEdge(edge2);  //
        if (!startEdge || !endEdge) continue;

        Route route(startEdge, endEdge);

        route.setGraph(graph);
        route.generateRoute();

        model->setData(model->index(i, 2), QString("%1").arg(route.Length()));
        model->setData(model->index(i, 3), route.Path());
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget *child = static_cast<QTableView*>(childAt(event->globalPos()));
    if (child->inherits("QTableView")) {
        QMenu menu(this);
        menu.addAction(actionInsertRow);
        menu.exec(event->globalPos());  // 阻塞
    }
}

void MainWindow::insertRow()
{
    QTableView *tableView = static_cast<QTableView *>(mainTab->currentWidget());
    if (!tableView) return;

    const QModelIndex index = tableView->selectionModel()->currentIndex();
    QAbstractItemModel *model = tableView->model();
    if (!model->insertRow(index.row() + 1))
        return;

    if (model) {
        model->insertRow(model->rowCount());
    }
}

Graph* MainWindow::getGraph()
{
    return graph;
}

void MainWindow::readEdgeInfo()
{
    int r = 0;
    QTableWidget *table = infoWidget->getEdgeTable();
    table->setRowCount(graph->EdgeMap().size() - 1);

    QMap<QString, Edge*>::const_iterator i = graph->EdgeMap().constBegin();
    while (i != graph->EdgeMap().constEnd()) {
        QString name = i.key();
        if (name == "NULL")  {
            ++i;
            continue;
        }
        int length = i.value()->Length();
        infoWidget->getEdgeTable()->setItem(r, 0, new QTableWidgetItem(name));
        infoWidget->getEdgeTable()->setItem(r++, 1, new QTableWidgetItem(QString("%1").arg(length)));
        ++i;
    }

}

void MainWindow::readRouteAttr()
{
    dockTree->clear();
    QTableView *curTable = static_cast<QTableView *>(mainTab->currentWidget());
    QStandardItemModel *curModel = static_cast<QStandardItemModel*>(curTable->model());

    QString edge1 = curModel->data(curModel->index(curTable->currentIndex().row(), 0)).toString();
    QString edge2 = curModel->data(curModel->index(curTable->currentIndex().row(), 1)).toString();

    QTreeWidgetItem *item1 = new QTreeWidgetItem(dockTree);
    item1->setText(0, tr("起点"));
    item1->setText(1, edge1);
    QTreeWidgetItem *item2 = new QTreeWidgetItem(dockTree);
    item2->setText(0, tr("终点"));
    item2->setText(1, edge2);
    QTreeWidgetItem *item3 = new QTreeWidgetItem(dockTree);
    item3->setText(0, tr("其它特性"));
    QTreeWidgetItem *item4 = new QTreeWidgetItem(item3);
    item4->setText(0, tr("长度"));
    item4->setText(1, curModel->data(curModel->index(curTable->currentIndex().row(), 2)).toString());
    QTreeWidgetItem *item5 = new QTreeWidgetItem(item3);
    item5->setText(0, tr("路径"));
    item5->setText(1, curModel->data(curModel->index(curTable->currentIndex().row(), 3)).toString());
}
