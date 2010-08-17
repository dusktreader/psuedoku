#include "qboarddelegate.h"

using namespace std;

QBoardDelegate::QBoardDelegate( QObject* parent ){
    this->parent = parent;
}

QWidget* QBoardDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const{
    return new QComboBox( parent );
}

void QBoardDelegate::setEditorData( QWidget* editor, const QModelIndex &index ) const{
    const QBoardModel* boardModel = static_cast<const QBoardModel*>( index.model() );
    vector<int> candidates = boardModel->getCandidates( index );
    int value = boardModel->getValue( index );
    QComboBox* comboBox = static_cast<QComboBox*>( editor );
    comboBox->addItem( "-", NO_VALUE );
    for( int i=0; i<(int)candidates.size(); i++ ){
        comboBox->addItem( QString::number( candidates[i] ), candidates[i] );
    }
    comboBox->setCurrentIndex( comboBox->findData( value ) );
}

void QBoardDelegate::setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex &index ) const{
    QComboBox* comboBox = static_cast<QComboBox*>( editor );
    QVariant value = comboBox->itemData( comboBox->currentIndex() );
    model->setData( index, value, Qt::EditRole );
}

void QBoardDelegate::updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const{
    editor->setGeometry( option.rect );
}
