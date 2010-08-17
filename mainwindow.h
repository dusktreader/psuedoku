#pragma once

#include <QtGui/QMainWindow>
#include "qboardmodel.h"
#include "qboarddelegate.h"
#include "board.h"
#include <QTime>
#include <QFileDialog>
#include "local.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QBoardModel* boardModel;
    QBoardDelegate* boardDelegate;
    Board* board;
    bool boardLock;
    void resetBoardView( int q );

private slots:
    void on_actionSave_Puzzle_triggered();
    void on_actionOpen_Puzzle_triggered();
    void on_action4x4_triggered();
    void on_action9x9_triggered();
    void on_action25x25_triggered();
    void on_action16x16_triggered();
    void on_actionVerify_triggered();
    void on_actionReset_triggered();
    void on_actionClear_triggered();
    void on_actionSolve_triggered();
};
