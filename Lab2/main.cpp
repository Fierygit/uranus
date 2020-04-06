/*
 * @Author: Firefly
 * @Date: 2020-04-05 15:47:14
 * @Descripttion:
 * @LastEditTime: 2020-04-06 13:22:51
 */
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "string.h"
#include <iostream>
using namespace std;

#define WEBHEAD_MAX 1000

struct context {  //
  int& fd;
  string& method;
  string& url;
};

//全局函数都可修改
// using response = void (*)(context);
typedef void (*response)(context);

// 线程池接口-------------------------------------------
class thread_pool {
 public:
  //注意处理response 应该是要static的， 创建线程的时调用的方法应该是 静态的，
  //不要属于 线程程池对象的
  bool add_task(response, context) { return true; }
  // 队列满的时候返回false，否则放入队列
  thread_pool(int a) {}  // 队列大小， 线程数
};
//-------------------------------------------------------------second fiveplus
 
//---------------------------------- 参数要什么在 context 写, chenjie
void static res(context c) { cout << c.url << endl; }
//------------------------------------------------------------

class http_server {
 public:
  http_server() {
    http_server(9998, 1);  //默认 8888 端口， 和 5个线程
  }
  http_server(int port, int thread_num) {
    this->port = port;
    this->thread_num = thread_num;
    pool = new thread_pool(thread_num);
  }

  void start() {
    while (true) {
      if (!pool->add_task(res, accept())) {  // accept函数里面阻塞式接受
        cout << "request will be reject!!!" << endl;
      }
    }
  }

 private:
  int port;
  int thread_num;
  thread_pool* pool;
  int fd;  // socket 句柄， 具体什么类型修改,
  // 接受函数
  int sock;
  context accept();
};

context http_server::accept() {
  sleep(1);

//定义接收数据的变量
char * buf = new char[WEBHEAD_MAX];
//定义处理recv接收数据的变量
char * recvBuf = new char[20];
int nRet = 0;
cout<<"start waiting..."<<endl;

	nRet = recv(sock, recvBuf, 20, 0);
	if (nRet > 0){
		strncat(buf, recvBuf, nRet);
		//判断如HTTP头中第一个字符是 P 那么就认为这是一个POST请求
		//那么接下来判断最后4个字符是否为 \r\n\r\n, 如果是说明整个头未接收完成
		if (buf[0] == 'P' && recvBuf[nRet - 1] == '\n' && recvBuf[nRet - 2] == '\r' && recvBuf[nRet - 3] == '\n' && recvBuf[nRet - 4] == '\r'){
			cout<<recvBuf<<endl;
		}
		//下面是判断GET是否完整的接收
		if (buf[0] == 'G' && recvBuf[nRet - 1] != '\n' && recvBuf[nRet - 2] != '\r' && recvBuf[nRet - 3] != '\n' && recvBuf[nRet - 4] != '\r'){
			cout<<recvBuf<<endl;
		}
		
	}

delete[]recvBuf;



  string a = "method";
  string b = "url"; 
  int c = 1;
  cout << "accept" << endl;
  return context{c, a, b};//! error 不要引用局部指针
}

int main() {
  http_server* server = new http_server();
  server->start();
  return 0;
}