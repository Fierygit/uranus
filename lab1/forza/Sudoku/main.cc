#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "sudoku.h"
#include <iostream>
using namespace std;
int64_t now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char* argv[])
{
int ii=0;
  init_neighbors();
char name[20]={0};
FILE *fp;
FILE *fpa; 
fpa=fopen("Result.txt","w");  
if(fpa==NULL) 
{  
printf("File cannot open! " );  
exit(0);  
}
cout<<"Chose the algorithm you want to use:"<<endl;
cout<<"1:sudoku_min_arity"<<endl;
cout<<"2:sudoku_min_arity_cache"<<endl;
cout<<"3:sudoku_dancing_links"<<endl;
cout<<"enter 1 2 or 3 to chose:"<<endl;
cin>>ii;
  char puzzle[128];
  int total_solved = 0;
  int sum_total_solved=0;
  int total = 0;
  bool (*solve)(int) = solve_sudoku_basic;
    if (ii == 1)
      solve = solve_sudoku_min_arity;
    else if (ii == 2)
      solve = solve_sudoku_min_arity_cache;
    else if (ii == 3)
      solve = solve_sudoku_dancing_links;
	cout<<"enter the filename:"<<endl;
	getchar();
  int64_t start,end;
  double sec,sum;

while(std::cin.getline(name, 20)) {
if(name[0]=='q'&&name[1]=='u'&&name[2]=='i'&&name[3]=='t')
	{break;}
fp = fopen(name, "r");
if(fp==NULL) 
{  
printf("File cannot open!not good!" );  
exit(0);  
}

start = now(); //计算输入一个文件后的开始时间


  while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
    if (strlen(puzzle) >= N) {
      ++total;
      input(puzzle);
      init_cache();
      //if (solve_sudoku_min_arity_cache(0)) {
      //if (solve_sudoku_min_arity(0))
      //if (solve_sudoku_basic(0)) {
      if (solve(0)) {
        ++total_solved;
        if (!solved())
          assert(0);
      }
      else {
        printf("No: %s", puzzle);
      }
    }
for(int i=0; i<N; i++) {  //该循环的作用是输入文件后在屏幕
if(i%9==0) {		  //上打印出计算出来的结果，即一个9*9的数字矩阵
printf("\n");
}       //如果只需要将结果输入到Result.txt中，不需要打印到屏幕上则将该循环删除
printf("%d|",board[i]);
}
printf("\n");


for(int i=0; i<N; i++) {  //该循环的作用是将每个计算的结果输入到Result.txt中
//fprintf(fp,"%d ", i);
fprintf(fpa,"%d",board[i]);
}
fprintf(fpa,"\n");
  }


fclose(fp);
//cout<<ii++<<endl;

	end=now();  //计算输入一个文件并将所有结果计算出来后的结束时间

	sec = (end-start)/1000000.0;
	printf("total time: %f sec;\neach  time: %f sec;\ntotal solved:%d;\n", sec, sec/total, total_solved);
	sum+=sec;
	sum_total_solved+=total_solved;
	total_solved = 0;
	total = 0;
cout<<"enter the filename:         ----enter 'quit' to stop"<<endl;
}



fclose(fpa);

  printf("total time: %f sec;\neach  time: %f sec;\ntotal solved: %d;\n",
	 sum, sum/sum_total_solved, sum_total_solved);

  return 0;
}

















