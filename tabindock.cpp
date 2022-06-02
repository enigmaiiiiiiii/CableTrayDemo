#include "tabindock.h"
#include <QScreen>

TabInDock::TabInDock(QWidget *parent)
    : QTabWidget{parent}
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

QSize TabInDock::sizeHint() const
{
    int h = screen()->geometry().height();
    int w = screen()->geometry().width();
    return QSize(w / 5, h * 0.83);
}
