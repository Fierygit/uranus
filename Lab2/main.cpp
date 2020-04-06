/*
 * @Author: Firefly
 * @Date: 2020-04-05 15:47:14
 * @Descripttion:
 * @LastEditTime: 2020-04-06 13:22:51
 */
#include <time.h>
#include <unistd.h>

#include <iostream>
using namespace std;

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
    http_server(8888, 5);  //默认 8888 端口， 和 5个线程
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
  context accept();
};

context http_server::accept() {
  sleep(1);
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