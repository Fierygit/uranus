/*
 * @Author: Firefly
 * @Date: 2020-04-06 23:49:33
 * @Descripttion:
 * @LastEditTime: 2020-04-06 23:59:31
 */
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
using namespace std;
#define WEBHEAD_MAX 100
#define MYPORT 9998 /*定义用户连接端口*/
#define BACKLOG 10  /*多少等待连接控制*/
main() {
  cout << "hey" << endl;
  int sockfd, new_fd;         /* listen on sock_fd, new connection on new_fd */
  struct sockaddr_in my_addr; /* my address information */
  struct sockaddr_in their_addr; /* connector's address information */
  int sin_size;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  my_addr.sin_family = AF_INET;         /* host byte order */
  my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
  bzero(&(my_addr.sin_zero), 0);        /* zero the rest of the struct */

  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) ==
      -1) {
    perror("bind");
    exit(1);
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  while (1) { /* main accept() loop */
    socklen_t sin_size = sizeof(struct sockaddr_in);
    if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) ==
        -1) {
      perror("accept");
      continue;
    }

    char *buf = new char[WEBHEAD_MAX];
    //定义处理recv接收数据的变量
    char *recvBuf = new char[WEBHEAD_MAX];
    int nRet = 0;
    memset(buf, 0, WEBHEAD_MAX);
    memset(recvBuf, 0, WEBHEAD_MAX);
    cout << "start waiting..." << endl;
    while (true) {
      cout << "\n\n\n\n****************************************************\n";
      nRet = recv(new_fd, recvBuf, WEBHEAD_MAX, 0);
      if (nRet > 0) {
        strncat(buf, recvBuf, nRet);
        //判断如HTTP头中第一个字符是 P 那么就认为这是一个POST请求
        //那么接下来判断最后4个字符是否为 \r\n\r\n, 如果不是说明整个头未接收完成
        // if (buf[0] == 'P' && recvBuf[nRet - 1] == '\n' && recvBuf[nRet - 2]
        // == '\r' && recvBuf[nRet - 3] == '\n' && recvBuf[nRet - 4] == '\r'){
        // 	continue;
        // }
        // //下面是判断GET是否完整的接收
        // if (buf[0] == 'G' && recvBuf[nRet - 1] != '\n' && recvBuf[nRet - 2]
        // != '\r' && recvBuf[nRet - 3] != '\n' && recvBuf[nRet - 4] != '\r'){
        // 	cout<<buf;
        cout << recvBuf;
        continue;
        // }

      } else {
        break;
      }
    }

    // //printf("server: got connection from
    // %s\n",inet_note(their_addr.sin_addr)); if (!fork()) { /* this is the
    // child process */ if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
    // perror("send");
    // close(new_fd);
    // exit(0);
    // }

    close(new_fd); /* parent doesn't need this */
    cout << "total recvie:" << endl;
    cout << buf;
    // while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
  }
  return 0;
}