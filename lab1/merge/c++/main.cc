/*
 * @Author: Firefly
 * @Date: 2020-03-08 13:56:54
 * @Descripttion:
 * @LastEditTime: 2020-03-08 17:35:00
 */

#include "intputfile.h"
#include "threadpool.h"

#include <sys/time.h>
#include <iostream>
#include "sudoku.h"
using namespace std;

// 声明全局变量， 多个 cpp 共享

int len;
int **data;
int **ans;



// 使用最快的方法
bool (*solve)(int *) = solve_one;

//解决第几个 数组 问题
void work(int cnt) {

  int *tmp = new int[81];//注意没有释放 内存
  for (int i = 0; i < 81; i++) tmp[i] = data[cnt][i];
  if (solve(tmp)) {
  } else {
    cout << "no ans\n";
  }
}

int main() {
  intputfile();
  cout << "file input over!!!\n\n" << endl;
  
  ans = (int **)malloc(sizeof(int *) * len);
  for (int i = 0; i < len; i++) ans[i] = (int *)malloc(sizeof(int) * (81));

    cout << "create " << 100 << "threads threadpool****************" << endl;

    Threadpool *pool = new Threadpool(100, len, work);

  pause();
}