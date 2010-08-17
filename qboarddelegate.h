#ifndef QBOARDDELEGATE_H
#define QBOARDDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>
#include <vector>
#include "qboardmodel.h"

class QBoardDelegate : public QItemDelegate{
private:
    QObject* parent;
public:
    QBoardDelegate( QObject* parent );
    QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    void setEditorData( QWidget* editor, const QModelIndex &index ) const;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex &index ) const;
    void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};

#endif // QBOARDDELEGATE_H
