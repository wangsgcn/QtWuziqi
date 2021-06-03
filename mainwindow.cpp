#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QCursor>
#include <QPalette>
#include <QPixmap>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->createActions();
    this->createMenus();

    // default values
    this->player_first = true;
    this->game_size = 19;
    this->strategy_switch = 1;
    this->algorithm = "Greedy";
    this->search_depth = 1;

    this->X = NULL;
    this->Y = NULL;
    this->board = NULL;
    this->computer_move_row = -1;
    this->computer_move_col = -1;
    // set mouse tracking
    this->ui->centralwidget->setMouseTracking(true);
    this->setMouseTracking(true);

    this->game_status = GAME_OFF;
    this->setup_board();
    this->init_game();
}

MainWindow::~MainWindow()
{
    delete ui;
    this->delete_board();
}

void MainWindow::createMenus()
{
    this->gameMenu = menuBar()->addMenu(tr("Game"));
    this->gameMenu->addAction(this->actNewGame);
    this->gameMenu->addAction(this->actSetup);
}

void MainWindow::createActions()
{
    this->actSetup   = new QAction("Setup Game");
    this->actSetup->setMenuRole(QAction::NoRole);
    this->actSetup->setShortcut(QKeySequence::Save);

    this->actNewGame = new QAction("New Game");
    this->actNewGame->setShortcut(QKeySequence::New);

    this->connect(this->actSetup,   SIGNAL(triggered()), this, SLOT(setupGame()));
    this->connect(this->actNewGame, SIGNAL(triggered()), this, SLOT(newGame()));
}

void MainWindow::newGame()
{
    this->game_status = GAME_ON;
    this->game = new Wuziqi(this->game_size, this->strategy_switch);
    this->init_game();
    this->setup_board();
    this->update();
}

void MainWindow::setupGame()
{
    Dialog *setupDiag = new Dialog;
    setupDiag->exec();
    setupDiag->get_game_setup(this->game_size,
                              this->search_depth,
                              this->player_first,
                              this->strategy_switch,
                              this->algorithm);
    this->game_status = GAME_ON;
    this->game = new Wuziqi(this->game_size, this->strategy_switch);
    this->init_game();
    this->setup_board();
    this->update();

}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int row, col, x, y;
    x = event->pos().x();
    y = event->pos().y();

    if(this->game_status == GAME_ON            &&
       this->get_row_col(x, y, row, col)==true &&  //
       this->board[row][col]==0)                   // current point is not taken
    {
        this->ui->centralwidget->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        this->ui->centralwidget->setCursor(Qt::ArrowCursor);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // when game is running, respond to mouse press
    if(this->game_status==GAME_ON)
    {
        int x, y, row, col, move_row, move_col, winer_color;
        x = event->pos().x();
        y = event->pos().y();
        if(this->get_row_col(x, y, row, col)==true && this->board[row][col]==0) // the current position is not taken
        {
            this->board[row][col] = 1; // player move
            this->game->set_stone(row, col, 1);
            this->update();
            if(this->game->game_over(winer_color))
            {
                this->game_status = GAME_OFF;
                for(int n=0; n<5; n++)
                {
                    int wr, wc;
                    this->game->get_win_row_col(n, wr, wc);
                    this->win_rows[n] = wr;
                    this->win_cols[n] = wc;
                }
                this->draw_winner_flag = true;
                this->ui->statusbar->showMessage("Player win!");
            }
            else
            {
                // choose AI algorithm for computing the next move
                if(this->algorithm == "Greedy")
                {
                    this->game->get_move_greedy(move_row, move_col);
                }
                else if(this->algorithm == "Minimax")
                {
                    this->game->get_move_minimax(move_row, move_col);
                }
                else if(this->algorithm == "Minimax with Alpha Beta")
                {
                    this->game->get_move_minimax_ab(move_row, move_col);
                }
                else if(this->algorithm == "Greedy Minimax")
                {
                    this->game->get_move_minimax_ab_greedy(move_row, move_col);
                }

                this->board[move_row][move_col] = 2;             // this is for drawing board
                this->game->set_stone(move_row, move_col, 2);    // this is for the Wuziqi class

                // record the last move of computer
                this->computer_move_row = move_row;
                this->computer_move_col = move_col;
                if(this->game->game_over(winer_color))
                {
                    this->game_status = GAME_OFF;
                    for(int n=0; n<5; n++)
                    {
                        int wr, wc;
                        this->game->get_win_row_col(n, wr, wc);
                        this->win_rows[n] = wr;
                        this->win_cols[n] = wc;
                    }
                    this->draw_winner_flag = true;
                    this->ui->statusbar->showMessage("Computer win!");
                }
            }
            this->update();
        }
    }
}

void MainWindow::setup_board()
{
    // setup board
    int window_width  = this->size().rwidth();
    // window height - the height of status bar
    int window_height = this->size().rheight() -  this->ui->statusbar->size().rheight();
    // board edge = one grid width
    float board_ratio = this->game_size/(this->game_size + 2.0);
    if(window_height<window_width)
    {
        this->board_size = (int)(window_height * board_ratio);
    }
    else
    {
        this->board_size = (int)(window_width * board_ratio);
    }

    this->grid_size = this->board_size/(this->game_size-1);

    this->top_left_y = (window_height - this->grid_size*(this->game_size-1))/2;
    this->top_left_x = (window_width  - this->grid_size*(this->game_size-1))/2; // adjust top left x to make the board being always in the center of window.


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
    this->setup_board();
    this->draw_board();
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

void MainWindow::init_game()
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
