#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void get_game_setup(int &game_size,
                        int &search_depth,
                        bool &player_first,
                        int &strategy_switch,
                        QString &algorithm);
private slots:

    void on_spinBoxGameSize_valueChanged(int arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_horizontalSliderAI_valueChanged(int value);

    void on_radioButtonPlayer_clicked(bool checked);

    void on_radioButtonComputer_clicked(bool checked);

private:
    Ui::Dialog *ui;
    int game_size;
    int search_depth;
    int strategy_switch;
    QString algorithm;
    bool player_first;
};

#endif // DIALOG_H
