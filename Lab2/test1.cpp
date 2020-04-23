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
    string s = "POST /Post_show HTTP/1.1
Host: 127.0.0.1:8888
User-Agent: curl/7.58.0
Accept: */*
Content-Length: 19
Content-Type: application/x-www-form-urlencoded

Name=HNU&ID=CS06142POST /Post_show HTTP/1.1
Host: 127.0.0.1:8888
User-Agent: curl/7.58.0
Accept: */*
Content-Length: 25
Content-Type: application/x-www-form-urlencoded

Name=HNUU&ID=201708010321";


cout<<szLineBuf<<endl;
//  // 获取HTTP版本
//  char  szHttpVersion[BUFFER_SIZE] = {0};
//  char szHttpVersion1[BUFFER_SIZE] = {0};
//  char szHttpVersion2[BUFFER_SIZE] = {0};
//  sscanf(szLineBuf,  "%[^ ]", szHttpVersion);
//  sscanf(szLineBuf, "%*[^ ] %[^ ]", szHttpVersion1);
//   sscanf(szLineBuf, "%*[^H]%*[^T]%*[^P]%*[^/]/%[^ ]", szHttpVersion2);
//  cout<<"方法是:"<<endl<<szHttpVersion<<endl;
//  cout<<"请求的url是:"<<endl<<szHttpVersion1<<endl;
//  cout<<"HTTP版本号是:"<<endl<<szHttpVersion2<<endl;
    return 0;
}