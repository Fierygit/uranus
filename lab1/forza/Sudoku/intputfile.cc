#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "sudoku.h"
#include <iostream>
#include<vector>
int len=0;
int a[100000][81]={0};
using namespace std;
int intputfile() {
//	int a[6]={1,2,3,4,5,6};
	vector<vector<char> > A;
	vector<char> B(81,0);
	char puzzle[128];
	FILE *fp;
//	for(int i=0;i<=5;i++)
//	b.push_back(a[i]);
    char name[20]={0};
    char nam;
  cout<<"输入文件名，多个文件名用换行符分隔，输入quit结束输入："<<endl;
  while(std::cin.getline(name, 20)) {
  if(name[0]=='q'&&name[1]=='u'&&name[2]=='i'&&name[3]=='t')
	{break;}
		fp = fopen(name, "r");
	if(fp==NULL) 
	{  
	printf("File cannot open!not good!" );  
	exit(0);  
	}
	while (fgets(puzzle, sizeof puzzle, fp) != NULL) {
    	if (strlen(puzzle) >= 81) {
    			B.clear();
    		for(int i=0; i<81; i++) {
    			B.push_back(puzzle[i]);
//				cout<<puzzle[i];
			}
//				cout<<endl;
				A.push_back(B);
    		}
	}
	fclose(fp);
  }

  len = A.size();
  for(int i=0; i<len; i++) {
	for(int j=0; j<81; j++) {
		a[i][j]=A[i][j]-48;
	}
  }

	return 0;
}
