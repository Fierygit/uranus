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
using namespace std;
#define WEBHEAD_MAX 100
#define BUFFER_SIZE 256
#define MYPORT 9998 /*定义用户连接端口*/
#define BACKLOG 10 /*多少等待连接控制*/

bool HttpRespHeaderPrase(const std::string& strHeader)
{
    int count=0;
if ( strHeader.empty() )
{
return false;
}
// 行缓冲区
char szLineBuf[BUFFER_SIZE] = {0};

// 通过流来提取报文头中内容
std::stringstream ssHeader(strHeader.c_str());

// 提取第一行内容
ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
cout<<szLineBuf<<endl;
// 获取HTTP版本
char szHttpVersion[BUFFER_SIZE] = {0};
char szHttpVersion1[BUFFER_SIZE] = {0};
sscanf(szLineBuf, "%s", szHttpVersion);
    cout<<strlen(szHttpVersion)<<endl;
    int i=strlen(szHttpVersion);
    for(int j=strlen(szHttpVersion); szLineBuf[j]!='\n'; j++) {
        szLineBuf[j]=szLineBuf[j-i];
    }

sscanf(szLineBuf, "%s", szHttpVersion1);
cout<<"HTTP版本是:"<<szHttpVersion<<"到此为止..."<<endl;
cout<<"HTTP版本是1:"<<szHttpVersion<<"1到此为止..."<<endl;
// 获取状态码
strtok(szLineBuf," ");
int iStatusCode = atoi(strtok(NULL, " "));

// 逐行提取余下的报文参数名值对
std::string strParamName;
std::string strParamValue;
for (;;)
{
// 提取一行内容
ssHeader.getline(szLineBuf, sizeof(szLineBuf), '\r');
// 跳过参数名之前的无效字符
// 有效起始地址
char* start = szLineBuf + count;

// 解析完毕
if ( 0 == strlen(start) )
{
break;
}
// 查找冒号出现的位置
char* p = strchr(start, ':');
if ( p - start > 0 )
{
strParamName.assign(start, p - start);
}
else
{
// 解析出错
return false;
}

// 跳过无效字符
count = strspn(p, ": ");
strParamValue.assign(p + count);
}

return true;
}



/* recv()只能读写套接字，而不能是一般的文件描述符 */
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1)
    {

        int ret = recv(sockfd, buf, len, MSG_PEEK); // 设置标志位后读取后不清除缓冲区
        if (ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}

// // 在readline函数中，我们先用recv_peek”偷窥“ 一下现在缓冲区有多少个字符并读取到bufp，
// 然后查看是否存在换行符'\n'。如果存在，则使用readn连
// // 通换行符一起读取（清空缓冲区）；如果不存在，也清空一下缓冲区, 且移动bufp的位置，
// 回到while循环开头，再次窥看。注意，当我们调用readn读
// // 取数据时，那部分缓冲区是会被清空的，因为readn调用了read函数。还需注意一点是，
// 如果第二次才读取到了'\n'，则先用count保存了第一次读取的
// // 字符个数，然后返回的ret需加上原先的数据大小。
// // 使用 readline函数也可以认为是解决粘包问题的一个办法，即以'\n'为结尾当作一条消息
// 。对于服务器端来说可以在前面的fork程序的基础上把do_service函数更改如下：

/* 读到'\n'就返回，加上'\n' 一行最多为maxline个字符 */
ssize_t readline(int sockfd, char *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = maxline;
    int count = 0;

    while (1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if (ret < 0)
            return ret; // 返回小于0表示失败
        else if (ret == 0)
            return ret; //返回0表示对方关闭连接了

        nread = ret;
        int i;
        for (i = 0; i < nread; i++)
        {
            if (bufp[i] == '\n')
            {
                ret = read(sockfd, bufp, i + 1);
                if (ret != i + 1)
                    exit(EXIT_FAILURE);
                
                return ret + count;
            }
        }
        if (nread > nleft)
            exit(EXIT_FAILURE);
        nleft -= nread;
        ret = read(sockfd, bufp, nread);
        if (ret != nread)
            exit(EXIT_FAILURE);

        bufp += nread;
        count += nread;
    }

    return -1;
}
void do_echoser(int conn) {
    char recvbuf[1024];
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = readline(conn, recvbuf, 1024);
        if (ret == -1)
            exit(ret);
        else if (ret  == 0)   //客户端关闭
        {
            printf("client close\n");
        }
        for(int i=0; i<ret; i++) {
            cout<<recvbuf[i];
        }
        cout<<endl<<endl;
//      fputs(recvbuf, stdout);
        HttpRespHeaderPrase(recvbuf);
        write(conn, recvbuf, strlen(recvbuf));
		close(conn);
	
}

main()
{
int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd */
struct sockaddr_in my_addr; /* my address information */
struct sockaddr_in their_addr; /* connector's address information */
int sin_size;
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
perror("socket");
exit(1);
}
my_addr.sin_family = AF_INET; /* host byte order */
my_addr.sin_port = htons(MYPORT); /* short, network byte order */
my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
bzero(&(my_addr.sin_zero),0); /* zero the rest of the struct */

if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) {
perror("bind");
exit(1);
}
if (listen(sockfd, BACKLOG) == -1) {
perror("listen");
exit(1);
}

 

while(1) { /* main accept() loop */
socklen_t sin_size = sizeof(struct sockaddr_in);
if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
    
perror("accept");
continue;
}


do_echoser(new_fd);
break;
// char * buf = new char[WEBHEAD_MAX];
// //定义处理recv接收数据的变量
// char * recvBuf = new char[WEBHEAD_MAX];
// int nRet = 0;
// memset(buf, 0, WEBHEAD_MAX);
// memset(recvBuf, 0, WEBHEAD_MAX);
// cout<<"start waiting..."<<endl;
// while (true)
// {
// 	nRet = recv(new_fd, recvBuf, WEBHEAD_MAX, 0);
// 	if (nRet > 0){
// 		strncat(buf, recvBuf, nRet);
// 		//判断如HTTP头中第一个字符是 P 那么就认为这是一个POST请求
// 		//那么接下来判断最后4个字符是否为 \r\n\r\n, 如果不是说明整个头未接收完成
// 		// if (buf[0] == 'P' && recvBuf[nRet - 1] == '\n' && recvBuf[nRet - 2] == '\r' && recvBuf[nRet - 3] == '\n' && recvBuf[nRet - 4] == '\r'){
// 		// 	continue;
// 		// }
// 		// //下面是判断GET是否完整的接收
// 		// if (buf[0] == 'G' && recvBuf[nRet - 1] != '\n' && recvBuf[nRet - 2] != '\r' && recvBuf[nRet - 3] != '\n' && recvBuf[nRet - 4] != '\r'){
// 		// 	cout<<buf;
// 		 	cout<<recvBuf;
// 			continue;
// 		// }

// 	}
//     else {
//         break;
//     }
		
// }

// // //printf("server: got connection from %s\n",inet_note(their_addr.sin_addr));
// // if (!fork()) { /* this is the child process */
// // if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
// // perror("send");
// // close(new_fd);
// // exit(0);
// // }

// close(new_fd); /* parent doesn't need this */
// cout<<"total recvie:"<<endl;
// cout<<buf;
// //while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */







}
return 0;
}