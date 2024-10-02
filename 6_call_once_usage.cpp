//
// Created by morgen on 2024/9/30.
//

/*
 * 单例模式是一种常见设计模式，用于确保某个类智能创建一个实例。
 * 在多线程环境中使用单例模式时，需要考虑线程安全问题
 *
 * 使用 std::call_once，用于确保某个函数只会被调用一次
 */

#include <iostream>
#include <thread>
#include <string>
#include <mutex>


std::once_flag once;
// 单例类
class Log {
 public:
  static Log* log;
  Log(const Log& log) = delete;
  Log& operator=(const Log& other) = delete;

  static Log& getInstance() {
//    static Log log;  // 懒汉模式，懒加载，即在第一次使用的时候才去初始化, 静态局部是线程安全的

    std::call_once(once, Init); // call_once 不能在 main 函数中使用

    return *log;
  }

  void printLog(const std::string& msg) {
    std::cout << __TIME__ << " | "<< msg << std::endl;
  }

 private:
  Log() {};
  static void Init() {
    if (log == nullptr) log = new Log();
  }
};
Log* Log::log = nullptr;

void print_error() {
  Log::getInstance().printLog("Printing a log");
}


int main() {
  std::thread t1(print_error);
  std::thread t2(print_error);
  t1.join();
  t2.join();
}
