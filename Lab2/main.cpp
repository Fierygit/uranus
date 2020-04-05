/*
 * @Author: Firefly
 * @Date: 2020-04-05 15:47:14
 * @Descripttion:
 * @LastEditTime: 2020-04-05 16:11:08
 */
#include <iostream>
using namespace std;

using response = void (*)(context);
// 线程池接口
class thread_pool {
 public:
  bool add_task(response, context);  // 队列满的时候返回false，否则放入队列
  thread_pool(int);                  // 队列大小， 线程数
};

struct context { // 
  int& fd;
  string& method;
  string& url;
};

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
  static response res;  // 陈杰实现
};

int main() {
  http_server* server = new http_server();
  server->start();
  return 0;
}