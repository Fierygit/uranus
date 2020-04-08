/*
 * @Author: Firefly
 * @Date: 2020-04-06 12:47:51
 * @Descripttion:
 * @LastEditTime: 2020-04-06 13:45:51
 */
#include <iostream>
using namespace std;

class test {
 public:
  int a;
  string *b;
  test(){cout << "???";}
  test(int a, string *b) : a(a), b(b) { cout << "ctor\n"; }
  test(test&& t){cout << "copy1\n";}
  test(test& t){cout << "copy2\n";}
  test operator=(const test& t){cout << "=\n";}
};
test get() {
  int a = 1;
  string *b = new string("fdsaf");  // 要返回全局的指针
  return test{a, b};
}

int main() {
  test t = get();
  cout << t.a << " " << *t.b << endl;
  test tt = t;
  return 0;
}