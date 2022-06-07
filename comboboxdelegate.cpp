#include "comboboxdelegate.h"
#include <QStringList>
#include <QComboBox>
#include <QCompleter>

ComboBoxDelegate::ComboBoxDelegate(QStringList edges, QObject *parent)
    : QStyledItemDelegate{parent},
      edgeList(edges)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItems(edgeList);
    QCompleter *completer = new QCompleter(edgeList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    editor->setCompleter(completer);
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    comboBox->setCurrentIndex(comboBox->findText(value));
}                                  

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    QString val = comboBox->currentText();
    model->setData(index, val, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}
