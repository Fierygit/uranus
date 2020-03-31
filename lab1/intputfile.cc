/*
 * @Author: Firefly
 * @Date: 2020-03-08 13:52:30
 * @Descripttion:
 * @LastEditTime: 2020-03-28 20:14:48
 */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <vector>


using namespace std;

#include "intputfile.h"



void intputfile() {
  //	int a[6]={1,2,3,4,5,6};
  vector<vector<char> > A;
  vector<char> B(81, 0);
  char puzzle[128];
  FILE *fp;
  //	for(int i=0;i<=5;i++)
  //	b.push_back(a[i]);
  string prefix = "";
  string name;
  char nam;
  cout << "输入文件名，多个文件名用换行符分隔，输入 ctrl_ d (注意是dddddd) "
          "结束输入："
       << endl;
  while (getline(cin, name)) {
    char *tmp_name = new char[40];
    name = prefix + name;
    strcpy(tmp_name, name.c_str());
    fp = fopen(tmp_name, "r");
    if (fp == NULL) {
      printf("File cannot open!not good!\n");
      exit(0);
    }
    while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
      if (strlen(puzzle) >= 81) {
        B.clear();
        for (int i = 0; i < 81; i++) {
          B.push_back(puzzle[i]);
          //				cout<<puzzle[i];
        }
        //				cout<<endl;
        A.push_back(B);
      }
    }
    fclose(fp);
  }

  std::cout << "input over" << endl;

  len = A.size();
 
  data = (int **)malloc(sizeof(int *) * len);
  for (int i = 0; i < len; i++) data[i] = (int *)malloc(sizeof(int) * (81));

  for (int i = 0; i < len; i++) {
    for (int j = 0; j < 81; j++) {
      data[i][j] = A[i][j] - 48;
//	cout<<data[i][j];
    }
//	cout<<endl;
  }
}
