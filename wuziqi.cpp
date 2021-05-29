#include "wuziqi.h"
#include <cmath>
#include <algorithm>

Wuziqi::Wuziqi()
{
}

Wuziqi::Wuziqi(int size, int lever)
{
    this->size = size;
    this->lever = lever;
    this->B = new int*[this->size];
    this->F =new bool*[this->size];
    for(int n=0; n<this->size; n++)
    {
        this->B[n] = new int[this->size];
        this->F[n] = new bool[this->size];
    }

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            this->B[r][c] = 0;
            this->F[r][c] = false;
        }
    }
    this->c5pts = 200000.0;
    this->o4pts = 20000.0;
    this->d3pts = 10000.0;
    this->c4pts = 2000.0;
    this->o3pts = 2000.0;
    this->c3pts = 20.0;
    this->o2pts = 2.0;
    this->c2pts = 1.0;
}

Wuziqi::~Wuziqi()
{
    for(int n=0; n<this->size; n++)
    {
        delete [] this->B[n];
        delete [] this->F[n];
    }
    delete [] this->B;
    delete [] this->F;
}

int Wuziqi::operator()(int row, int col)
{
    return this->B[row][col];
}

bool Wuziqi::inside(int row, int col)
{
    if(row>=0 && row<this->size && col>=0 && col<this->size)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Wuziqi::set_stone(int row, int col, int color)
{
    this->B[row][col] = color;
}

bool Wuziqi::check(int N, int rows[], int cols[], int pattern[])
{
    for(int n=0; n<N; n++)
    {
        int r = rows[n];
        int c = cols[n];
        if(this->B[r][c] != pattern[n])
        {
            return false;
        }
    }
    return true;
}

void Wuziqi::reset_F()
{
    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            this->F[r][c] = false;
        }
    }
}

int Wuziqi::count_open_two(int color)
{
    int a = color;
    int count = 0;

    // 0 0 a a 0
    // 0 a a 0 0
    // patterns
    int p1[5] = {0, 0, a, a, 0};
    int p2[5] = {0, a, a, 0, 0};


    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[5] = {r, r  , r  , r  , r  };
            int hcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r,c+4) && (this->check(5, hrows, hcols, p1) || this->check(5, hrows, hcols, p2)))
            {
                count++;
            }

            // vertical direction
            int vrows[5] = {r, r+1, r+2, r+3, r+4};
            int vcols[5] = {c, c,   c,   c,   c  };
            if(this->inside(r+4,c) && (this->check(5, vrows, vcols, p1) || this->check(5, vrows, vcols, p2)))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[5] = {r, r-1, r-2, r-3, r-4};
            int urcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r-4,c+4) && (this->check(5, urrows, urcols, p1) || this->check(5, urrows, urcols, p2)))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[5] = {r, r+1, r+2, r+3, r+4};
            int lrcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r+4,c+4) && (this->check(5, lrrows, lrcols, p1) || this->check(5, lrrows, lrcols, p2)))
            {
                count++;
            }
        }
    }

    return count;
}

int Wuziqi::count_closed_two(int color)
{
    int count = 0;
    int a, b;
    a = color==1? 1:2; // if color==1, a=1; else, a=2
    b = color==1? 2:1; // if color==1, b=2; else, b=1

    // b a a 0 0 0
    // 0 0 0 a a b
    // patterns
    int p1[5] = {b, a, a, 0, 0};
    int p2[5] = {0, 0, a, a, b};


    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[5] = {r, r,   r,   r,   r  };
            int hcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r,c+4) && (this->check(5, hrows, hcols, p1) || this->check(5, hrows, hcols, p2)))
            {
                count++;
            }

            // vertical direction
            int vrows[5] = {r, r+1, r+2, r+3, r+4};
            int vcols[5] = {c, c,   c,   c,   c  };
            if(this->inside(r+4,c) && (this->check(5, vrows, vcols, p1) || this->check(5, vrows, vcols, p2)))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[5] = {r, r-1, r-2, r-3, r-4};
            int urcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r-4,c+4) && (this->check(5, urrows, urcols, p1) || this->check(5, urrows, urcols, p2)))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[5] = {r, r+1, r+2, r+3, r+4};
            int lrcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r+4,c+4) && (this->check(5, lrrows, lrcols, p1) || this->check(5, lrrows, lrcols, p2)))
            {
                count++;
            }
        }
    }
    return count;
}

int Wuziqi::count_open_three(int color)
{
    int count = 0;
    int p[5] = {0, color, color, color, 0};
    // 0 * * * 0
    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[5] = {r, r,   r,   r,   r  };
            int hcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r,c+4) && this->check(5, hrows, hcols, p))
            {
                count++;
            }

            // vertical direction
            int vrows[5] = {r, r+1, r+2, r+3, r+4};
            int vcols[5] = {c, c,   c,   c,   c  };
            if(this->inside(r+4,c) && this->check(5, vrows, vcols, p))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[5] = {r, r-1, r-2, r-3, r-4};
            int urcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r-4,c+4) && this->check(5, urrows, urcols, p))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[5] = {r, r+1, r+2, r+3, r+4};
            int lrcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r+4,c+4) && this->check(5, lrrows, lrcols, p))
            {
                count++;
            }
        }
    }
    return count;
}

int Wuziqi::count_closed_three(int color)
{
    int count = 0;
    int a, b;
    a = color==1? 1:2; // if color==1, a=1; else, a=2
    b = color==1? 2:1; // if color==1, b=2; else, b=1
    // b a a a 0 0
    // 0 0 a a a b
    int p1[6] = {b, a, a, a, 0, 0};
    int p2[6] = {0, 0, a, a, a, b};

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[6] = {r, r,   r,   r,   r  , r  };
            int hcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r,c+5) && (this->check(6, hrows, hcols, p1) || this->check(6, hrows, hcols, p2)))
            {
                count++;
            }

            // vertical direction
            int vrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int vcols[6] = {c, c,   c,   c,   c  , c  };
            if(this->inside(r+5,c) && (this->check(6, vrows, vcols, p1) || this->check(6, vrows, vcols, p2)))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[6] = {r, r-1, r-2, r-3, r-4, r-5};
            int urcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r-5,c+5) && (this->check(6, urrows, urcols, p1) || this->check(6, urrows, urcols, p2)))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int lrcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r+5,c+5) && (this->check(6, lrrows, lrcols, p1) || this->check(6, lrrows, lrcols, p2)))
            {
                count++;
            }
        }
    }
    return count;
}

int Wuziqi::count_open_four(int color)
{
    int count = 0;
    int p[6]={0, color, color, color, color, 0};

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[6] = {r, r,   r,   r,   r  , r  };
            int hcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r,c+5) && this->check(6, hrows, hcols, p))
            {
                count++;
            }

            // vertical direction
            int vrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int vcols[6] = {c, c,   c,   c,   c  , c  };
            if(this->inside(r+5,c) && this->check(6, vrows, vcols, p))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[6] = {r, r-1, r-2, r-3, r-4, r-5};
            int urcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r-5,c+5) && this->check(6, urrows, urcols, p))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int lrcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r+5,c+5) && this->check(6, lrrows, lrcols, p))
            {
                count++;
            }
        }
    }
    return count;
}

int Wuziqi::count_closed_four(int color)
{
    int count = 0;
    int a, b;
    a = color==1? 1:2; // if color==1, a=1; else, a=2
    b = color==1? 2:1; // if color==1, b=2; else, b=1
    //0 a a a a b
    //b a a a a 0
    int p1[6] = {0, a, a, a, a, b};
    int p2[6] = {b, a, a, a, a, 0};

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[6] = {r, r,   r,   r,   r  , r  };
            int hcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r,c+5) && (this->check(6, hrows, hcols, p1) || this->check(6, hrows, hcols, p2)))
            {
                count++;
            }

            // vertical direction
            int vrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int vcols[6] = {c, c,   c,   c,   c  , c  };
            if(this->inside(r+5,c) && (this->check(6, vrows, vcols, p1) || this->check(6, vrows, vcols, p2)))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[6] = {r, r-1, r-2, r-3, r-4, r-5};
            int urcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r-5,c+5) && (this->check(6, urrows, urcols, p1) || this->check(6, urrows, urcols, p2)))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int lrcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r+5,c+5) && (this->check(6, lrrows, lrcols, p1) || this->check(6, lrrows, lrcols, p2)))
            {
                count++;
            }
        }
    }
    return count;
}



int Wuziqi::count_five(int color)
{
    int count = 0;
    int p[5] = {color, color, color, color, color};

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[5] = {r, r,   r,   r,   r  };
            int hcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r,c+4) && this->check(5, hrows, hcols, p))
            {
                count++;
            }

            // vertical direction
            int vrows[5] = {r, r+1, r+2, r+3, r+4};
            int vcols[5] = {c, c,   c,   c,   c  };
            if(this->inside(r+4,c) && this->check(5, vrows, vcols, p))
            {
                count++;
            }

            // upper right diagonal direction
            int urrows[5] = {r, r-1, r-2, r-3, r-4};
            int urcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r-4,c+4) && this->check(5, urrows, urcols, p))
            {
                count++;
            }

            // lower right diagonal direction
            int lrrows[5] = {r, r+1, r+2, r+3, r+4};
            int lrcols[5] = {c, c+1, c+2, c+3, c+4};
            if(this->inside(r+4,c+4) && this->check(5, lrrows, lrcols, p))
            {
                count++;
            }
        }
    }
    return count;
}

bool Wuziqi::find_open_four(int color, int &row, int &col)
{
    // two patterns
    int p1[6] = {0, 0, color, color, color, 0};
    int p2[6] = {0, color, color, color, 0, 0};

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // horizontal direction
            int hrows[6] = {r, r,   r,   r,   r  , r  };
            int hcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r,c+5))
            {
                    if(this->check(6, hrows, hcols, p1))
                    {
                        row = r;
                        col = c+1;
                        return true;
                    }
                    if(this->check(6, hrows, hcols, p2))
                    {
                        row = r;
                        col = c+4;
                        return true;
                    }
            }


            // vertical direction
            int vrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int vcols[6] = {c, c,   c,   c,   c  , c  };
            if(this->inside(r+5,c))
            {
                if (this->check(6, vrows, vcols, p1))
                {
                    row = r+1;
                    col = c;
                    return true;
                }
                if (this->check(6, vrows, vcols, p2))
                {
                    row = r+4;
                    col = c;
                    return true;
                }
            }

            // upper right diagonal direction
            int urrows[6] = {r, r-1, r-2, r-3, r-4, r-5};
            int urcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r-5,c+5))
            {
                if(this->check(6, urrows, urcols, p1))
                {
                    row = r-1;
                    col = c+1;
                    return true;
                }
                if(this->check(6, urrows, urcols, p2))
                {
                    row = r-4;
                    col = c+4;
                    return true;
                }
            }

            // lower right diagonal direction
            int lrrows[6] = {r, r+1, r+2, r+3, r+4, r+5};
            int lrcols[6] = {c, c+1, c+2, c+3, c+4, c+5};
            if(this->inside(r+5,c+5))
            {
                if(this->check(6, lrrows, lrcols, p1))
                {
                    row = r+1;
                    col = c+1;
                    return true;
                }
                if(this->check(6, lrrows, lrcols, p2))
                {
                    row = r+4;
                    col = c+4;
                    return true;
                }
            }
        }
    }
    // no open four
    row = -1;
    col = -1;
    return false;
}

bool Wuziqi::game_over(int &winner_color)
{
    for(int color=1; color<=2; color++)
    {
        int p[5]={color, color, color, color, color};
        for(int r=0; r<this->size; r++)
        {
            for(int c=0; c<this->size; c++)
            {
                // horizontal direction
                int hrows[5] = {r, r,   r,   r,   r  };
                int hcols[5] = {c, c+1, c+2, c+3, c+4};
                if(this->inside(r,c+4) && this->check(5, hrows, hcols, p))
                {
                    winner_color = color;
                    this->copy_win_row_col(hrows, hcols);
                    return true;
                }

                // vertical direction
                int vrows[5] = {r, r+1, r+2, r+3, r+4};
                int vcols[5] = {c, c,   c,   c,   c  };
                if(this->inside(r+4,c) && this->check(5, vrows, vcols, p))
                {
                    winner_color = color;
                    this->copy_win_row_col(vrows, vcols);
                    return true;
                }

                // upper right diagonal direction
                int urrows[5] = {r, r-1, r-2, r-3, r-4};
                int urcols[5] = {c, c+1, c+2, c+3, c+4};
                if(this->inside(r-4,c+4) && this->check(5, urrows, urcols, p))
                {
                    winner_color = color;
                    this->copy_win_row_col(urrows, urcols);
                    return true;
                }

                // lower right diagonal direction
                int lrrows[5] = {r, r+1, r+2, r+3, r+4};
                int lrcols[5] = {c, c+1, c+2, c+3, c+4};
                if(this->inside(r+4,c+4) && this->check(5, lrrows, lrcols, p))
                {
                    winner_color = color;
                    this->copy_win_row_col(lrrows, lrcols);
                    return true;
                }
            }
        }
    }
    // no winner
    winner_color = -1;
    return  false;
}

float Wuziqi::evaluate_score_greedy(int row, int col)
{
    float k = 1.0 + 10.0*this->lever/100.0;
    // evaluate the point (row, col) for computer
    this->B[row][col] = 2; // computer color number is 2
    int c_o2_cnt = this->count_open_two(2);
    int c_c2_cnt = this->count_closed_two(2);
    int c_o3_cnt = this->count_open_three(2);
    int c_c3_cnt = this->count_closed_three(2);
    int c_o4_cnt = this->count_open_four(2);
    int c_c4_cnt = this->count_closed_four(2);
    int c_c5_cnt = this->count_five(2);
    this->B[row][col] = 0; // reset the point to be empty
    int c_o3_cnt0 = this->count_open_three(2);
    int c_d3_cnt = (c_o3_cnt - c_o3_cnt0)/2;

    float offense_score  = c_o2_cnt * this->o2pts
                         + c_c2_cnt * this->c2pts
                         + c_o3_cnt * this->o3pts
                         + c_c3_cnt * this->c3pts
                         + c_d3_cnt * this->d3pts
                         + c_o4_cnt * this->o4pts
                         + c_c4_cnt * this->c4pts
                         + c_c5_cnt * this->c5pts;

    // evaluate the point (row,col) for player
    this->B[row][col] = 1; // computer color number is 2
    int p_o2_cnt = this->count_open_two(1);
    int p_c2_cnt = this->count_closed_two(1);
    int p_o3_cnt = this->count_open_three(1);
    int p_c3_cnt = this->count_closed_three(1);
    int p_o4_cnt = this->count_open_four(1);
    int p_c4_cnt = this->count_closed_four(1);
    int p_c5_cnt = this->count_five(1);
    this->B[row][col] = 0; // reset the point to be empty
    int p_o3_cnt0 = this->count_open_three(1);
    int p_d3_cnt = (p_o3_cnt - p_o3_cnt0)/2;

    float defense_score  = p_o2_cnt * this->o2pts
                         + p_c2_cnt * this->c2pts
                         + (p_o3_cnt * this->o3pts
                         + p_c3_cnt * this->c3pts
                         + p_d3_cnt * this->d3pts
                         + p_o4_cnt * this->o4pts
                         + p_c4_cnt * this->c4pts
                         + p_c5_cnt * this->c5pts)*k;

   return offense_score + defense_score;
}

bool Wuziqi::find_win_position(int color, int &row, int &col)
{
    int p[5]={0, color, color, color, color};
    int others[5][4]={{1,2,3,4},   // offset = 0
                      {0,2,3,4},   // offset = 1
                      {0,1,3,4},   // offset = 2
                      {0,1,2,4},   // offset = 3
                      {0,1,2,3}};  // offset = 4

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            for(int offset=0; offset<5; offset++)
            {
                int o0 = others[offset][0];
                int o1 = others[offset][1];
                int o2 = others[offset][2];
                int o3 = others[offset][3];
                // horizontal direction
                int hrows[5] = {r, r, r, r, r};
                int hcols[5] = {c+offset, c+o0, c+o1, c+o2, c+o3};
                if(this->inside(r, c+4) && this->check(5, hrows, hcols, p))
                {
                    row = r;
                    col = c+offset;
                    return true;
                }
                // vertical direction
                int vrows[5]={r+offset, r+o0, r+o1, r+o2, r+o3};
                int vcols[5]={c, c, c, c, c};
                if(this->inside(r+4,c) && this->check(5, vrows, vcols, p))
                {
                    row = r+offset;
                    col = c;
                    return true;
                }
                // upper right diagonal direction
                int urrows[5] = {r-offset, r-o0, r-o1, r-o2, r-o3};
                int urcols[5] = {c+offset, c+o0, c+o1, c+o2, c+o3};
                if(this->inside(r-4,c+4) && this->check(5, urrows, urcols, p))
                {
                    row = r-offset;
                    col = c+offset;
                    return true;
                }
                // lower right diagonal direction
                int lrrows[5] = {r+offset, r+o0, r+o1, r+o2, r+o3};
                int lrcols[5] = {c+offset, c+o0, c+o1, c+o2, c+o3};
                if(this->inside(r+4,c+4) && this->check(5, lrrows, lrcols, p))
                {
                    row = r+offset;
                    col = c+offset;
                    return true;
                }
            }
        }
    }
    // no win position
    row = -1;
    col = -1;
    return false;

}

void Wuziqi::get_move_greedy(int &row, int &col)
{
    int c_win_row, c_win_col, p_win_row, p_win_col;
    int c_o4_row, c_o4_col;

    float best_score = -INFINITY;
    int best_row, best_col;

    if(this->find_win_position(2, c_win_row, c_win_col))
    {
        row = c_win_row;
        col = c_win_col;
        return;
    }
    if(this->find_win_position(1, p_win_row, p_win_col))
    {
        row = p_win_row;
        col = p_win_col;
        return;
    }
    if(this->find_open_four(2, c_o4_row, c_o4_col))
    {
        row = c_o4_row;
        col = c_o4_col;
        return;
    }

    // initialize best row and best col with -1
    best_row = -1;
    best_col = -1;
    /*
    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // if the current position is already taken, go to the next position.
            if(this->B[r][c] != 0)
            {
                continue;
            }

            float score = this->evaluate_score_greedy(r, c);
            if(score > best_score)
            {
                best_score = score;
                best_row = r;
                best_col = c;
            }
        }
    }
    */
    std::vector<int> nearby_rows;
    std::vector<int> nearby_cols;
    this->find_nearby_moves(nearby_rows, nearby_cols, 3);
    for(unsigned int n=0; n<nearby_rows.size(); n++)
    {
        int row = nearby_rows[n];
        int col = nearby_cols[n];
        float score = this->evaluate_score_greedy(row,col);
        if(score > best_score)
        {
            best_score = score;
            best_row = row;
            best_col = col;
        }
    }
    row = best_row;
    col = best_col;
    return;
}

void Wuziqi::find_nearby_moves(std::vector<int> &rows, std::vector<int> &cols, int neighbor_size)
{
    if(rows.empty()==false)
    {
        rows.clear();
    }
    if(cols.empty()==false)
    {
        cols.clear();
    }

    // reset the flag array for selecting the nearby moves
    this->reset_F();

    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            // if the current position is already taken, check its neighbors
            if(this->B[r][c]>0)
            {
                int row_max = std::min(r + neighbor_size, this->size-1);
                int col_max = std::min(c + neighbor_size, this->size-1);
                int row_min = std::max(r - neighbor_size, 0);
                int col_min = std::max(c - neighbor_size, 0);

                for(int rr=row_min; rr<=row_max; rr++)
                {
                    for(int cc=col_min; cc<=col_max; cc++)
                    {
                        if(this->B[rr][cc]==0 && this->F[rr][cc]==false) //the current position (rr,cc) is not taken, not selected
                        {
                            rows.push_back(rr);     // save row number
                            cols.push_back(cc);     // save column number
                            this->F[rr][cc] = true; // the current position is selected
                        }
                    }
                }
            }
        }
    }
    return;
}

void Wuziqi::get_win_row_col(int n, int &row, int &col)
{
    row = this->win_rows[n];
    col = this->win_cols[n];
    return;
}

void Wuziqi::copy_win_row_col(int win_rows[], int win_cols[])
{
    for(int n=0; n<5; n++)
    {
        this->win_rows[n] = win_rows[n];
        this->win_cols[n] = win_cols[n];
    }
    return;
}

float Wuziqi::evaluate_score_minimax()
{
    // computer
    int c_o2_cnt = this->count_open_two(2);
    int c_c2_cnt = this->count_closed_two(2);
    int c_o3_cnt = this->count_open_three(2);
    int c_c3_cnt = this->count_closed_three(2);
    int c_o4_cnt = this->count_open_four(2);
    int c_c4_cnt = this->count_closed_four(2);
    int c_c5_cnt = this->count_five(2);

    float c_score  = c_o2_cnt * this->o2pts
                   + c_c2_cnt * this->c2pts
                   + c_o3_cnt * this->o3pts
                   + c_c3_cnt * this->c3pts
                   + c_o4_cnt * this->o4pts
                   + c_c4_cnt * this->c4pts
                   + c_c5_cnt * this->c5pts;

    // player
    int p_o2_cnt = this->count_open_two(2);
    int p_c2_cnt = this->count_closed_two(2);
    int p_o3_cnt = this->count_open_three(2);
    int p_c3_cnt = this->count_closed_three(2);
    int p_o4_cnt = this->count_open_four(2);
    int p_c4_cnt = this->count_closed_four(2);
    int p_c5_cnt = this->count_five(2);

    float k = 1.0 + 10.0*this->lever/100.0;

    float p_score  = p_o2_cnt * this->o2pts
                   + p_c2_cnt * this->c2pts
                   + (p_o3_cnt * this->o3pts
                   + p_c3_cnt * this->c3pts
                   + p_o4_cnt * this->o4pts
                   + p_c4_cnt * this->c4pts
                   + p_c5_cnt * this->c5pts) * k;
    return c_score - p_score;
}

std::string Wuziqi::get_board_code()
{
    std::string code;
    for(int r=0; r<this->size; r++)
    {
        for(int c=0; c<this->size; c++)
        {
            code += std::to_string(this->B[r][c]);
        }
    }
    return code;
}
