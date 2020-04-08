#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <iostream>
#include <sstream>
#define BUFFER_SIZE 256
using namespace std;
int main() {
    char szLineBuf[BUFFER_SIZE] = "GET /index.html HTTP/1.1 \0";


cout<<szLineBuf<<endl;
 // 获取HTTP版本
 char  szHttpVersion[BUFFER_SIZE] = {0};
 char szHttpVersion1[BUFFER_SIZE] = {0};
 char szHttpVersion2[BUFFER_SIZE] = {0};
 sscanf(szLineBuf,  "%[^ ]", szHttpVersion);
 sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
  sscanf(szLineBuf, "%*[^H]%*[^T]%*[^P]%*[^/]/%[^ ]", szHttpVersion2);
 cout<<"方法是:"<<endl<<szHttpVersion<<endl;
 cout<<"请求的url是:"<<endl<<szHttpVersion1<<endl;
 cout<<"HTTP版本号是:"<<endl<<szHttpVersion2<<endl;
    return 0;
}