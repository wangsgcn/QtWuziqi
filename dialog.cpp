#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // default values
    this->setWindowTitle("Set up game parameters");
    this->game_size = 19;
    this->algorithm = "Greedy";
    this->search_depth = 1;
    this->strategy_switch = 1;
    this->player_first = true;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_spinBoxGameSize_valueChanged(int arg1)
{
    this->game_size = arg1;
}


void Dialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    this->algorithm = arg1;
}


void Dialog::on_horizontalSliderAI_valueChanged(int value)
{
    this->strategy_switch = value;
}


void Dialog::on_radioButtonPlayer_clicked(bool checked)
{
    if(checked)
    {
        this->player_first = true;
    }
}


void Dialog::on_radioButtonComputer_clicked(bool checked)
{
    if(checked)
    {
        this->player_first = false;
    }
}

void Dialog::get_game_setup(int &game_size, int &search_depth, bool &player_first, int &strategy_switch, QString &algorithm)
{
    game_size = this->game_size;
    search_depth = this->search_depth;
    player_first = this->player_first;
    strategy_switch = this->strategy_switch;
    algorithm = this->algorithm;
}

