#ifndef QBOARDMODEL_H
#define QBOARDMODEL_H

#include <QAbstractTableModel>
#include <QColor>

#include <iostream>
#include "board.h"
#include "cell.h"

class QBoardModel : public QAbstractTableModel
{
    Q_OBJECT

private:
    Board* board;

public:
    QBoardModel( Board* board, QObject* parent=0 ) : QAbstractTableModel(parent), board(board){}
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    std::vector<int> getCandidates( const QModelIndex &index ) const;
    int getValue( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role=Qt::DisplayRole ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    bool setData( const QModelIndex &index, const QVariant &value, int role=Qt::EditRole );
};

#endif // QBOARDMODEL_H
