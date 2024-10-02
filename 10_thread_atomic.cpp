//
// Created by morgen on 2024/10/2.
//
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

/*
 * 用于实现多线程环境下的原子操作，避免数据竞争问题, 比加锁解锁效率更高
 */

std::atomic<int> shared_data(0);


void func() {
    for (int i = 0; i < 1000; ++i) {
      shared_data++;
    }
}

int main() {
  std::thread t1(func);
  std::thread t2(func);

  t1.join();
  t2.join();
  std::cout << shared_data << std::endl;
  return 0;
}