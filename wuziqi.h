#ifndef WUZIQI_H
#define WUZIQI_H
#include <vector>
#include <string>
#include <map>

const int GAME_ON  = 1;
const int GAME_OFF = 0;

class Wuziqi
{
private:
    int size;
    int lever;
    int **B;
    bool **F;    // selected flag, used in find nearby moves
    int win_rows[5];  // for recording the row numbers of the winning five
    int win_cols[5];  // for recording the column numbers of the wining five
    float c5pts;
    float o4pts;
    float d3pts;
    float c4pts;
    float o3pts;
    float c3pts;
    float o2pts;
    float c2pts;
    // minimax part
    std::map<std::string, float> minimax_catch;
public:
    Wuziqi();
    Wuziqi(int size, int lever);
    ~Wuziqi();
    int operator()(int row, int col);
    void get_move_greedy(int &move_row, int &move_col);
    void get_move_minimax(int &move_row, int &move_col);
    void get_move_minimax_ab(int &move_row, int &move_col);
    void get_move_minimax_ab_greedy(int &move_row, int &move_col);
    void set_stone(int row, int col, int color);
    bool game_over(int &winner_color);
    bool game_over(); // used in minimax to check terminal status
    void get_win_row_col(int n, int &row, int &col);
private:
    bool inside(int row, int col);
    bool check(int N, int rows[], int cols[], int pattern[]);
    void reset_F();
    int count_open_two(int color);
    int count_closed_two(int color);
    int count_open_three(int color);
    int count_closed_three(int color);
    int count_disjointed_three(int color);
    int count_open_four(int color);
    int count_closed_four(int color);
    int count_disjointed_four(int color);
    int count_five(int color);
    bool find_open_four(int color, int &row, int &col);
    bool find_win_position(int color, int &row, int &col);
    void find_nearby_moves(std::vector<int> &rows, std::vector<int> &cols, int neighbor_size);
    void find_better_moves(std::vector<int> &rows, std::vector<int> &cols, int neighbor_size, int better_size);
    float evaluate_score_greedy(int row, int col);
    float evaluate_score_minimax();
    void copy_win_row_col(int win_rows[], int win_cols[]);
    std::string get_board_code();
    float minimax(bool maximize_computer, int current_depth, int maximum_depth);
    float minimax_ab(bool maximize_computer, int current_depth, int maximum_depth, float alpha, float beta);
    float minimax_ab_greedy(bool maximize_computer, int current_depth, int maximum_depth, float alpha, float beta, int better_size);
 };

#endif // WUZIQI_H
