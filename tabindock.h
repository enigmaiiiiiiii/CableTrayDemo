#ifndef TABINDOCK_H
#define TABINDOCK_H

#include <QTabWidget>

class TabInDock : public QTabWidget
{
    Q_OBJECT
public:
    TabInDock(QWidget* parent = nullptr);
    QSize sizeHint() const;
};

#endif // TABINDOCK_H
