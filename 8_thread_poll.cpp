//
// Created by morgen on 2024/10/2.
//

/*
 * 线程池
 *   维护线程数组
 *   维护任务队列
 *     生产者向任务队列添加任务（用户）
 *     消费者从任务队列获取任务（线程）
 *
 *   它可以存储、调用或引用任何可调用（Callable）目标——函数、lambda 表达式
 *   、绑定表达式或其他函数对象，只要这些可调用目标的签名符合。
 */


#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <functional>
#include <queue>


class ThreadPool {
 public:
  ThreadPool(int numThreads): stop(false) {
    for (int i = 0; i < numThreads; ++i) {
      // 线程不支持拷贝构造和赋值运算符，因此需要使用 emplace_back
      // 使用移动语义构造 thread，传入 lambda 表达式, 这些 thread 是异步构造的
      // 不断地从任务队列中获取任务
      threads.emplace_back([this] {
        while (1) {
          std::unique_lock<std::mutex> lock(mtx);
          // 若任务队列不空或者线程池结束，则结束阻塞
          condition.wait(lock, [this] {
            return !tasks.empty() || stop;
          });

          // 若线程池结束，并且任务队列为空，才允许线程结束
          if (stop && tasks.empty()) {
            return;
          }

          // 移动语义, 从任务队列中取得任务，解锁，执行任务
          std::function<void()> task(std::move(tasks.front()));
          tasks.pop();
          lock.unlock();

          // 执行任务
          task();
        }
      });
    }
  }

  ~ThreadPool() {
    {  // 使用完共享参数就释放互斥量
      std::unique_lock<std::mutex> lock(mtx);
      stop = true;
    }

    // 通知挂起的所有线程，线程池要终止了
    condition.notify_all();
    for (auto& t : threads) {
      t.join();
    }
  }

  // 可变列表
  template<class F, class... Args>
  void enqueue(F&& f, Args&&... args) {
    // 将函数与参数绑定
    std::function<void()> task =
        std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    {
      std::unique_lock<std::mutex> lock(mtx);
      tasks.emplace(std::move(task));
    }
    condition.notify_one();
  }

 private:
  std::vector<std::thread> threads;

  // function 是可调用对象包装器，里面包装了一个返回值类型为 void 的函数
  std::queue<std::function<void()>> tasks;

  // 控制数据竞态
  std::mutex mtx;
  std::condition_variable condition;

  // 线程池什么时候终止
  bool stop;

};

int main() {
  ThreadPool pool(4);

  for (int i = 0; i < 10; ++i) {
    pool.enqueue([i] {
      std::cout << i << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds (1));
    });
  }
  return 0;
}