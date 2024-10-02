//
// Created by morgen on 2024/9/30.
//

/* 什么是线程安全的？多线程程序每一次运行结果和单线程的运行的结果始终一致，那么就是线程安全的
 * 在多个线程中共享数据时，需要注意线程安全问题
 * 数据竞争：多个线程同时访问同一个变量，且其中至少有一个线程对该变量
 * 进行写操作，那么就会出现数据竞争问题
 *
 * 使用同步机制来确保多个线程之间对共享数据的访问是安全的
 *
 * 常见同步机制：互斥量、条件变量、原子操作
 */

#include <iostream>
#include <thread>
#include <mutex> // 互斥锁

int a = 0; // 全局变量
std::mutex mtx;

void func() {
  for (int i = 0; i < 10000; i++) {
    mtx.lock();  // 加锁，如果不解锁，其他线程无法访问a，其他线程加锁失败会阻塞
    a += 1;      // 加锁成功又相当于该线程获得了互斥量的所有权，解锁相当于释放所有权
    mtx.unlock(); // 解锁
  }
}

int main() {
  std::thread t1(func);
  std::thread t2(func);

  t1.join();
  t2.join();

  std::cout << a << std::endl; // 我们预期结果是20000，但实际输出小于20000，这是因为发生了数据竞争
  return 0;
}