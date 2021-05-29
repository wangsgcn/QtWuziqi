#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMouseEvent>
#include <QPaintEvent>
#include "wuziqi.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_spinBoxSize_valueChanged(int arg1);

    void on_radioButtonPlayer_clicked();

    void on_radioButtonComputer_clicked();

    void on_horizontalScrollBar_valueChanged(int value);


    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_pushButton_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    Wuziqi *game;

// Gomoku
private:
    int game_size;
    bool player_first;
    int lever;
    QString algorithm;
    int top_left_x;
    int top_left_y;
    int board_size;
    int grid_size;
    int *X, *Y;
    int left_x, right_x, top_y, bottom_y;
    int stone_radius;
    int **board; // 2d array, board, 0 - empty
    int win_rows[5];
    int win_cols[5];
    bool draw_winner_flag;
    int game_status; // 0: off, 1: on, 2.
    int computer_move_row; // last move of computer
    int computer_move_col; // loas move of computer
private:
    void setup_board();
    void draw_board();
    bool get_row_col(int x, int y, int &row, int &col);
    void get_x_y(int row, int col, int &x, int &y);
    void reset_board();
    void disable_input();
    void enable_input();
    void delete_board();
    void initialize_game();

};
#endif // MAINWINDOW_H
