#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QPalette>
#include <QPixmap>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->player_first = true;
    this->game_size = 15;
    this->algorithm = this->ui->comboBox->currentText();
    this->lever = 0;
    this->X = NULL;
    this->Y = NULL;
    this->board = NULL;
    this->computer_move_row = -1;
    this->computer_move_col = -1;
    // set mouse tracking
    this->ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);

    this->game_status = 0;
    this->setup_board();
    this->reset_board();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_spinBoxSize_valueChanged(int arg1)
{
    // delete previous board before create a new board.
    this->delete_board();

    if(arg1<10)
    {
        this->ui->spinBoxSize->setValue(10);
        this->game_size = 10;
    }
    else if(arg1>25)
    {
        this->ui->spinBoxSize->setValue(25);
        this->game_size = 25;
    }
    else
    {
        this->game_size = arg1;
    }

    this->initialize_game();
    this->update();
}




void MainWindow::on_radioButtonPlayer_clicked()
{
    this->player_first = true;
}


void MainWindow::on_radioButtonComputer_clicked()
{
    this->player_first = false;
}



void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    this->lever = value; // range: 0-99
}



void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    this->algorithm = arg1;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int row, col, x, y;
    x = event->x();
    y = event->y();
    if(this->get_row_col(x, y, row, col)==true)
    {
        this->ui->centralwidget->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        this->ui->centralwidget->setCursor(Qt::ArrowCursor);
    }
    //this->ui->statusbar->showMessage(QString::number(event->x()) + "," + QString::number(event->y()));

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // when game is running, respond to mouse press
    if(this->game_status==1)
    {
        int x, y, row, col, move_row, move_col, winer_color;
        x = event->x();
        y = event->y();
        if(this->get_row_col(x, y, row, col)==true)
        {
            this->board[row][col] = 1; // player move
            this->game->set_stone(row, col, 1);
            this->update();
            if(this->game->game_over(winer_color))
            {
                this->game_status = 0;
                for(int n=0; n<5; n++)
                {
                    int wr, wc;
                    this->game->get_win_row_col(n, wr, wc);
                    this->win_rows[n] = wr;
                    this->win_cols[n] = wc;
                }
                this->draw_winner_flag = true;
                this->ui->statusbar->showMessage("Player win!");
                this->enable_input();
            }
            else
            {
                this->game->get_move_greedy(move_row, move_col);
                this->board[move_row][move_col] = 2;
                this->game->set_stone(move_row, move_col, 2);
                // record the last move of computer
                this->computer_move_row = move_row;
                this->computer_move_col = move_col;
                if(this->game->game_over(winer_color))
                {
                    this->game_status = 0;
                    for(int n=0; n<5; n++)
                    {
                        int wr, wc;
                        this->game->get_win_row_col(n, wr, wc);
                        this->win_rows[n] = wr;
                        this->win_cols[n] = wc;
                    }
                    this->draw_winner_flag = true;
                    this->ui->statusbar->showMessage("Computer win!");
                    this->enable_input();
                }
            }
            this->update();
        }
        //this->ui->statusbar->showMessage(QString::number(row) + "," + QString::number(col));
        //this->update();
    }
}

void MainWindow::setup_board()
{
    // setup board
    int window_width = this->size().rwidth();
    int window_height = this->size().rheight()-160;
    int board_margin = 30;
    if(window_height<window_width)
    {
        this->board_size = window_height - 2*board_margin;
    }
    else
    {
        this->board_size = window_width - 2*board_margin;
    }



    this->grid_size = this->board_size/(this->game_size-1);

    this->top_left_y = 160;// + (window_height - this->grid_size*(this->game_size-1))/2;
    this->top_left_x = (window_width - this->grid_size*(this->game_size-1))/2; // adjust top left x to make the board being always in the center of window.


    if(this->X != NULL)
    {
        delete[] this->X;
    }

    this->X = new int[this->game_size];
    if(this->Y != NULL)
    {
        delete[] this->Y;
    }
    this->Y = new int[this->game_size];


    for(int n=0; n<this->game_size; n++)
    {
        this->X[n] = this->top_left_x + n*this->grid_size;
        this->Y[n] = this->top_left_y + n*this->grid_size;
    }

    this->left_x = this->X[0];
    this->right_x = this->X[this->game_size-1];
    this->top_y = this->Y[0];
    this->bottom_y = this->Y[this->game_size-1];
    this->stone_radius = this->grid_size*0.4;
}

void MainWindow::draw_board()
{
    int x1, y1, x2, y2;
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setRenderHint(QPainter::HighQualityAntialiasing);

    // horizontal lines
    for(int n=0; n<this->game_size; n++)
    {
        if(n==0 || n==this->game_size-1)
        {
            painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
        }
        else
        {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
        }
        x1 = this->left_x;
        x2 = this->right_x;
        y1 = this->Y[n];
        y2 = this->Y[n];
        painter.drawLine(x1,y1,x2,y2);
    }
    // vertical lines
    for(int n=0; n<this->game_size; n++)
    {
        if(n==0 || n==this->game_size-1)
        {
            painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
        }
        else
        {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
        }
        x1 = this->X[n];
        x2 = this->X[n];
        y1 = this->top_y;
        y2 = this->bottom_y;
        painter.drawLine(x1,y1,x2,y2);
    }


    QPoint center;
    int rx, ry;
    rx = this->stone_radius;
    ry = this->stone_radius;

    //stones
    for(int r=0; r<this->game_size; r++)
    {
        for(int c=0; c<this->game_size; c++)
        {
            // player stone
            if(this->board[r][c] == 1)
            {
                center.setX(this->X[c]);
                center.setY(this->Y[r]);
                if(this->player_first)
                {
                    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
                    painter.setBrush(QBrush(QColor(Qt::black)));
                }
                else
                {
                    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
                    painter.setBrush(QBrush(QColor(Qt::white)));
                }

                painter.drawEllipse(center, rx, ry);
            }
            else if(this->board[r][c] == 2)
            {
                center.setX(this->X[c]);
                center.setY(this->Y[r]);
                if(this->player_first)
                {
                    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
                    painter.setBrush(QBrush(QColor(Qt::white)));
                }
                else
                {
                    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
                    painter.setBrush(QBrush(QColor(Qt::black)));
                }

                painter.drawEllipse(center, rx, ry);
            }
        }
    }

    // mark the last move of computer
    if(this->computer_move_row!=-1 && this->computer_move_row != -1)
    {
        // draw the last move of computer again, with red boundary.
        if(this->player_first)
        {
            painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
            painter.setBrush(QBrush(QColor(Qt::white)));
        }
        else
        {
            painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
            painter.setBrush(QBrush(QColor(Qt::black)));
        }
        center.setX(this->X[this->computer_move_col]);
        center.setY(this->Y[this->computer_move_row]);
        painter.drawEllipse(center, rx, ry);
    }

    // mark winner positions
    if(this->draw_winner_flag)
    {
        painter.setPen(QPen(Qt::cyan, 1, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(QBrush(QColor(Qt::cyan)));
        for(int n=0; n<5; n++)
        {
            int wr = this->win_rows[n];
            int wc = this->win_cols[n];
            int marker_radius = this->stone_radius/4;
            center.setX(this->X[wc]);
            center.setY(this->Y[wr]);
            painter.drawEllipse(center, marker_radius, marker_radius);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    //if(this->game_status==1)
    //{
        this->setup_board();
        this->draw_board();
    //}
}

bool MainWindow::get_row_col(int x, int y, int &row, int &col)
{
    for(int r=0; r<this->game_size; r++)
    {
        for(int c=0; c<this->game_size; c++)
        {
            int xc = this->X[c];
            int yr = this->Y[r];
            float d = std::sqrt((x-xc)*(x-xc) + (y-yr)*(y-yr));
            if(d < this->stone_radius/2)
            {
                row = r;
                col = c;
                return true;
            }
        }
    }
    row = -1;
    col = -1;
    return false;
}

void MainWindow::get_x_y(int row, int col, int &x, int &y)
{
    x = this->X[col];
    y = this->Y[row];
}

void MainWindow::reset_board()
{
    // allocae memory for B
    this->board = new int*[this->game_size];
    for(int r=0; r<this->game_size; r++)
    {
        this->board[r] = new int[this->game_size];

    }
    // initialize B to be 0, empty
    for(int r=0; r<this->game_size; r++)
    {
        for(int c=0; c<this->game_size; c++)
        {
            this->board[r][c] = 0;  // 0 - empty, 1 - player, 2 - computer
        }
    }
}

void MainWindow::delete_board()
{
    // if B has been allocated, delete its memory
    if(this->board != NULL)
    {
        for(int n=0; n<this->game_size; n++)
        {
            delete[] this->board[n];
        }
        delete[] this->board;
    }
}

void MainWindow::on_pushButton_clicked()
{
    // reset last move of computer
    this->computer_move_row = -1;
    this->computer_move_col = -1;
    // set game status
    this->game_status = 1;
    this->game = new Wuziqi(this->game_size, this->lever);
    this->setup_board();
    //this->reset_board();
    this->initialize_game();
    this->disable_input();
    this->update();
}

void MainWindow::disable_input()
{
    this->ui->comboBox->setDisabled(true);
    this->ui->spinBoxSize->setDisabled(true);
    this->ui->horizontalScrollBar->setDisabled(true);
    this->ui->radioButtonComputer->setDisabled(true);
    this->ui->radioButtonPlayer->setDisabled(true);
}

void MainWindow::enable_input()
{
    this->ui->comboBox->setDisabled(false);
    this->ui->spinBoxSize->setDisabled(false);
    this->ui->horizontalScrollBar->setDisabled(false);
    this->ui->radioButtonComputer->setDisabled(false);
    this->ui->radioButtonPlayer->setDisabled(false);
}

void MainWindow::initialize_game()
{
    // allocae memory for B
    this->board = new int*[this->game_size];
    for(int r=0; r<this->game_size; r++)
    {
        this->board[r] = new int[this->game_size];

    }
    // initialize B to be 0, empty
    for(int r=0; r<this->game_size; r++)
    {
        for(int c=0; c<this->game_size; c++)
        {
            this->board[r][c] = 0;  // 0 - empty, 1 - player, 2 - computer
        }
    }
    // set draw winner flag to false
    this->draw_winner_flag = false;
    this->computer_move_row = -1;
    this->computer_move_col = -1;
}
