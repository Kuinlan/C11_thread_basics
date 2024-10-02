//
// Created by morgen on 2024/10/2.
//

/*
 *  针对有返回函数处理
 *  函数模板， 用于异步执行一个函数，返回一个 std::future 对象，表示异步操作结果
 *  避免手动创建线程和管理线程的麻烦
 */

#include <iostream>
#include <thread>
#include <future>

int func() {
  int i = 0;
  for (; i < 1000; ++i) {
    ++i;
  }
  return i;
}

void func2(std::promise<int>& f) {
  f.set_value(1000);
}

int main() {
// 使用多个线程并行执行独立的，较为耗时的操作
//  std::thread t1(func);
//  std::thread t2(func);
//  t1.join();
//  t2.join();

//  // 使用 std::future
//  std::future<int> future_res = std::async(std::launch::async, func); // 函数在传入后，后台开始运行
//  std::cout << func() << std::endl;
//  std::cout << future_res.get() << std::endl; // 获得函数结果

//  // packaged_task, 类模板，将一个可调用对象，封装成一个异步操作，返回一个 std::future 对象，作为结果
//  std::packaged_task<int()> task(func); // 包装函数，但是不运行
//  auto future_result = task.get_future();
//
//  std::thread t1(std::move(task)); // 使用移动语义，std::packaged_task 和 std::thread 管理着底层资源（如线程句柄或任务状态），这些资源的语义是唯一的，不能简单地通过拷贝来赋值
//  std::cout << func() << std::endl;
//
//  // 获取异步执行结果
//  t1.join();
//  std::cout << future_result.get() << std::endl;

  // promise 是一个类模板，用于在一个线程中产生一个值，并在另一个线程中获取这个值。
  // promise 通常与 future 和 async 一起使用
  std::promise<int> f;
  auto future_result = f.get_future();

  std::thread t1(func2, std::ref(f));
  t1.join();

  std::cout << future_result.get() << std::endl;
  return 0;
}