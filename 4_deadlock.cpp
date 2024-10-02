//
// Created by morgen on 2024/9/30.
//

/*
 * 死锁：两个线程互相等待对方释放互斥量的所有权，造成循环等待
 */


#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>


std::mutex m1, m2;

void func_1() {
  m2.lock();
  sleep(1); // 获取 m2 后，等待一会，让另一个线程获得另一个互斥量
  m1.lock();  // 获取 m2 后才释放 m1
  m1.unlock();
  m2.unlock();
}

void func_2() {
  m1.lock();
  m2.lock(); // 获取 m1 后才释放 m2
  m1.unlock();
  m2.unlock();
} // func_1、func_2 两者造成死锁

// 如何避免死锁？让获取 m1 的线程 也获取 m2

int main() {
  std::thread t1(func_1);
  std::thread t2(func_2);
  t1.join();
  t2.join();
  std::cout << "Over" << std::endl;
}
