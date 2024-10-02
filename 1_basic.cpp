#include <iostream>
#include <string>
#include <thread>

void printHelloWorld(const std::string& msg) {
  std::cout << msg << std::endl;
  return;
}

void printInt() {
  for (int i = 0; i < 100000; ++i) {
    std::cout << i << std::endl;
  }
}

int main() {
// 创建线程
  std::thread thread1(printInt); // 注意这个传递参数方式
//  thread1.join(); // 主线程等待该线程结束，再执行主线程
  bool IsJoin = thread1.joinable(); // 判断子线程是否可以调用 join(), 否则可能会报错
  if (IsJoin) {
    thread1.join();
  }
  std::cout << "Over" << std::endl;
//  thread1.detach(); // 主线程和子线程同步运行, 有可能会打印出 message，有可能不会，原因？
  // 可以理解为：
  // join 是店家（主线程）等待顾客（子程序）吃完饭然后结账（继续运行)，顾客吃完时会打招呼(print)
  // detach 是店家在顾客开始吃饭后，直接结账（继续主线程），主线程结束时（打烊），也不会管顾客吃没吃完
  // 因此会出现观察到 msg 或者没观察到的现象


  return 0;
}
