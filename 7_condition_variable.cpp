//
// Created by morgen on 2024/9/30.
//

/*
 *  生产者和消费者模型
 *    生产者：往任务队列中加任务
 *    消费者：取任务
 *
 *  条件变量：如果条件不满足（即队列 q 为空），则线程会阻塞并释放 lock 锁，
 *  直到其他线程调用 cv 的 notify_one 或 notify_all 方法通知它，此时线程
 *  会重新检查条件，如果条件满足，则 wait 方法返回并重新获取 lock 锁，线程继续执行。
 */
#include <climits>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>

std::queue<int> g_queue;
std::condition_variable g_cv;

std::mutex mtx;

void Producer() {
  for (int i = 0; i < 10; ++i) {
    {
      std::unique_lock<std::mutex> lock(mtx);
      g_queue.push(i);
      // 通知消费者
      g_cv.notify_one(); // 通知一个线程取
      std::cout << "Producer : " << i << std::endl;
    } // 这里使用 {} 是为了在每次迭代结束时，释放 lock 对 互斥量的所有权
    std::this_thread::sleep_for(std::chrono::microseconds(100)); // 基于消费者时间处理任务
  }
}

void Consumer() {
  while (1) {
    std::unique_lock<std::mutex> lock(mtx);
    g_cv.wait(lock, []() { return !g_queue.empty(); }); // 当条件变量等待的时候，会先释放持有的锁，然后挂起线程，在唤醒时再获取锁
    int value = g_queue.front();
    std::cout << "Consumer : " << value << std::endl;
    g_queue.pop();
  }
} // 对取任务和放任务加锁

int main() {
  std::thread t1(Producer);
  std::thread t2(Consumer);
  t1.join();
  t2.join();
  return 0;
}