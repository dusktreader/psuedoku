#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    board = NULL;
    boardModel = NULL;
    boardDelegate = NULL;
    resetBoardView( 3 );
}

MainWindow::~MainWindow(){
    delete board;
    delete boardModel;
    delete ui;
}

void MainWindow::resetBoardView( int q ){
    if( boardModel != NULL )
        delete boardModel;
    if( boardDelegate != NULL )
        delete boardDelegate;
    if( board != NULL )
        delete board;
    board = new Board( q );
    boardModel = new QBoardModel( board, this );
    boardDelegate = new QBoardDelegate( this );
    ui->boardView->setModel( boardModel );
    ui->boardView->setItemDelegate( boardDelegate );
    int side = ui->boardView->columnWidth( 0 ) * board->getn() + 4;
    ui->boardView->resize( QSize( side, side ) );
    this->setFixedSize( QSize( side + 20, side + ui->statusBar->height() + ui->menuBar->height() + 10 ) );
    ui->boardView->setEnabled( true );
    ui->boardView->clearSelection();
    ui->boardView->repaint();
    ui->menuBar->repaint();
    ui->statusBar->repaint();
}

void MainWindow::on_actionSolve_triggered(){
    if( !ui->boardView->isEnabled() ){
        ui->statusBar->showMessage( "Board already solved.  Reset to edit." );
        return;
    }
    QTime timer;
    timer.start();
    bool success = board->solve();
    double runTime = timer.elapsed() / 1000.0;
    if( !success ){
        ui->statusBar->showMessage( "No solution found" );
        board->reset();
        return;
    }
    ui->boardView->setEnabled( false );
    ui->statusBar->showMessage( QString( "Solved in %1 seconds" ).arg( runTime, 6, 'g', 3, '0' ) );
}

void MainWindow::on_actionClear_triggered(){
    ui->boardView->setEnabled( true );
    board->clear();
}

void MainWindow::on_actionReset_triggered(){
    ui->boardView->setEnabled( true );
    board->reset();
}

void MainWindow::on_actionVerify_triggered()
{
    if( board->verify() )
        ui->statusBar->showMessage( "Puzzle is correct" );
    else
        ui->statusBar->showMessage( "Puzzle is invalid" );
}

void MainWindow::on_action16x16_triggered(){
    resetBoardView( 4 );
}

void MainWindow::on_action25x25_triggered(){
    resetBoardView( 5 );
}

void MainWindow::on_action9x9_triggered(){
    resetBoardView( 3 );
}

void MainWindow::on_action4x4_triggered(){
    resetBoardView( 2 );
}

void MainWindow::on_actionOpen_Puzzle_triggered(){
    QString qf = QFileDialog::getOpenFileName( this, "Open Puzzle File" );
    if( qf == "" )
        return;
    if( !board->read( qf.toStdString() ) )
        ui->statusBar->showMessage( QString( "Couldn't open file %1" ).arg( qf ) );
    else
        ui->statusBar->showMessage( QString( "Opened file %1" ).arg( qf ) );
}

void MainWindow::on_actionSave_Puzzle_triggered(){
    QString qf = QFileDialog::getSaveFileName( this, "Save Puzzle File" );
    if( qf == "" )
        return;
    if( !board->write( qf.toStdString() ) )
        ui->statusBar->showMessage( QString( "Couldn't save to file %1" ).arg( qf ) );
    else
        ui->statusBar->showMessage( QString( "Saved to file %1" ).arg( qf ) );

}
