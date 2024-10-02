//
// Created by morgen on 2024/9/30.
//

/*
 * lock_guard
 *   std::lock_guard 是标准库中的一种封装类，用于保护共享数据
 *   防止数据竞争
 *     构造函数被调用时，该互斥量会被自动锁定
 *     当析构函数被调用时，该互斥量会被自动解锁
 *     无法复制或者移动，智能在局部作用域中使用
 *
 * std::unique_lock （常用）
 *   互斥量封装类，对互斥量进行更加灵活的管理，包括延迟加锁、条件变量、超时等
 *   提供以下成员函数：
 *     lock(): 尝试对互斥量加锁，如果已经被其他线程持有，则当前线程会被阻塞
 *     try_lock(): 尝试加锁，如果已被持有，则立即返回 false，否则返回 true
 *     try_lock_for(const std::chrono::duration<Rep, Perioid>& rel_time)
 *       尝试加锁，若已被持有，则当前线程会被阻塞，直到被成功加锁或者超过了指定时间
 *     try_lock_until(const std::chrono::time_point<Clock, Duration>& abs_time)
 *     unlock()：解锁
 *   支持交换互斥变量，移动语义
 *
 */
#include <iostream>
#include <thread>
#include <mutex>

int shared_data = 0;
//std::mutex mtx;
std::timed_mutex mtx;


void func() {
  for (int i = 0; i < 2; i++) {
//    mtx.lock();  // 加锁，但没有解锁
//    std::lock_guard<std::mutex> lq(mtx); // 自动加锁
//    std::unique_lock<std::mutex> lq(mtx);
    std::unique_lock<std::timed_mutex> lg(mtx, std::defer_lock); // 延迟加锁
    lg.try_lock_for(std::chrono::seconds(2));
    if (!lg.owns_lock()) continue;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 获取互斥锁的在释放之前，未获取的线程取消等待
    shared_data++;
  }  // lq 调用析构函数，进行自动解锁
}

int main() {
  std::thread t1(func);
  std::thread t2(func);
  t1.join();
  t2.join(); // 线程2 无法获得互斥量，主线程无限等待

  std::cout << shared_data << std::endl;
}


