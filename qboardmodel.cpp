#include "qboardmodel.h"

using namespace std;

int QBoardModel::rowCount( const QModelIndex &parent ) const{
    return board->getn();
}

int QBoardModel::columnCount( const QModelIndex &parent ) const{
    return board->getn();
}

QVariant QBoardModel::data( const QModelIndex &index, int role ) const{
    if( !index.isValid() )
        return QVariant();
    if( index.row() > board->getn() || index.column() > board->getn() )
        return QVariant();
    if( role == Qt::DisplayRole ){
        int value = board->getCellValue( index.row(), index.column() );
        if( value == NO_VALUE )
            return "-";
        else
            return value;
    }
    else if( role == Qt::BackgroundRole ){
        int q = board->getq();
        if( ( ( index.row() / q ) & 1 ) == ( ( index.column() / q ) & 1 ) )
            return QColor( Qt::white );
        else
            return QColor( 225, 225, 225, 255 );
    }
    else
        return QVariant();
}

vector<int> QBoardModel::getCandidates( const QModelIndex &index ) const{
    if( !index.isValid() || index.row() > board->getn() ||
                            index.column() > board->getn() )
        return vector<int>();
    return board->getCandidates( index.row(), index.column() );
}

int QBoardModel::getValue( const QModelIndex &index ) const{
    if( !index.isValid() || index.row() > board->getn() ||
                            index.column() > board->getn() )
        return NO_VALUE;
    return board->getCellValue( index.row(), index.column() );
}

QVariant QBoardModel::headerData( int section, Qt::Orientation orientation, int role ) const{
    if( role != Qt::DisplayRole )
        return QVariant();
    if( orientation == Qt::Horizontal )
        return QString( "Column %1" ).arg( section );
    else
        return QString( "Row %1" ).arg( section );
}

Qt::ItemFlags QBoardModel::flags( const QModelIndex &index ) const{
    if( !index.isValid() )
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags( index ) | Qt::ItemIsEditable;
}

bool QBoardModel::setData( const QModelIndex &index, const QVariant &value, int role ){
    int newValue;
    bool convOK = true;
    newValue = value.toInt( &convOK );
    if( !convOK || newValue > board->getn() )
            return false;
    if( index.isValid() && role == Qt::EditRole ){
        board->setCellValue( index.row(), index.column(), newValue );
        emit dataChanged( index, index );
        return true;
    }
    return false;
}
