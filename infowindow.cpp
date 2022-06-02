#include "infowindow.h"
#include "graph.h"
#include "route.h"
#include "helper.h"
#include "mainwindow.h"
#include "tabindock.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QDockWidget>
#include <QPushButton>
#include <QLayout>
#include <QScreen>
#include <QTextEdit>
#include <QTableWidget>

InfoWindow::InfoWindow(QWidget *parent)
    : QWidget{parent}
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    infoTab = new TabInDock(this);  //
    infoTab->setTabPosition(QTabWidget::South);

    setupLogTab();
    setupEdgeTab();

    infoTab->show();
}

QSize InfoWindow::sizeHint() const
{
    int h = screen()->geometry().height();
    int w = screen()->geometry().width();
    QSize s(w/ 5, h);
    return s;
}

void InfoWindow::setupLogTab()
{
    QWidget *logInfo= new QWidget();
    logInfo->resize(400, 800);
    logInfo->setContentsMargins(0, 0, 0, 15);
    
    // 添加垂直布局，布局在QWidget logInfo中
    QVBoxLayout *widgetLayout = new QVBoxLayout(logInfo);
    widgetLayout->setContentsMargins(0, 0, 0, 0);

    // 添加控件QTextEdit, 尺寸由layout决定
    QTextEdit *logText = new QTextEdit(logInfo);
    logText->setReadOnly(true);
    logText->setObjectName(QString::fromUtf8("logText"));
    widgetLayout->addWidget(logText);

    // 水平布局，放置按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *btnLogClear = new QPushButton(tr("清空日志"), logInfo);
    QPushButton *btnLogSave = new QPushButton(tr("保存日志"), logInfo);

    connect(btnLogClear, &QPushButton::clicked, logText, &QTextEdit::clear);
    connect(btnLogSave, &QPushButton::clicked, logText, &QTextEdit::clear);

    HSpacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    btnLayout->addWidget(btnLogClear);
    btnLayout->addWidget(btnLogSave);

    btnLayout->addItem(HSpacer);

    widgetLayout->addLayout(btnLayout);
    infoTab->addTab(logInfo, tr("日  志"));

}

void InfoWindow::setupEdgeTab()
{
    dockTable = new QTableWidget(this);
    dockTable->setColumnCount(3);
    infoTab->addTab(dockTable, tr("段号信息"));
    QStringList headers;
    headers << "名称"
            << "长度"
            << "路径个数";
    dockTable->setHorizontalHeaderLabels(headers);
    dockTable->show();
}

QTableWidget* InfoWindow::getEdgeTable()
{
    return dockTable;
}
