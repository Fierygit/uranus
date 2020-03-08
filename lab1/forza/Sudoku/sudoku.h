#ifndef SUDOKU_H
#define SUDOKU_H

const bool DEBUG_MODE = false;
enum { ROW=9, COL=9, N = 81, NEIGHBOR = 20 };
const int NUM = 9;

extern int neighbors[N][NEIGHBOR];
extern int board[N];
extern int spaces[N];
extern int nspaces;
extern int (*chess)[COL];

extern int len;//这是输入数独问题的个数
extern int a[100000][81];//这是储存数独数据的二维数组

void init_neighbors();
void input(const char in[N]);
void init_cache();
int intputfile();

bool available(int guess, int cell);

bool solve_sudoku_basic(int which_space);
bool solve_sudoku_min_arity(int which_space);
bool solve_sudoku_min_arity_cache(int which_space);
bool solve_sudoku_dancing_links(int unused);
bool solved();


#endif
