//
// Created by morgen on 2024/9/30.
//

#include <iostream>
#include <thread>
#include <memory>

class MyClass {
 public:
  int a = 0;
  void func() {
    std::cout << "Thread" << std::this_thread::get_id()
              << "  started" << std::endl;
    std::cout << this->a << std::endl;
    std::cout << "Thread " << std::this_thread::get_id()
              << "  finished" << std::endl;
  }
};

void thread_foo() {

}

std::thread thread2;
void foo(int& x) {  // 线程函数默认用值传递将 a 的副本传递给 foo，因此不会影响到传入的 a
  std::cout << "in foo" << std::endl;
  x += 1;
  std::cout << x << std::endl;
}

void foo1(int* x) {  // 线程函数默认用值传递将 a 的副本传递给 foo，因此不会影响到传入的 a
  std::cout << "in foo" << std::endl;
  *x += 1;
  std::cout << x << std::endl;
  std::cout << *x << std::endl;
}

void test() {
  int a = 1;
  thread2 = std::thread(foo, std::ref(a));
} // 主线程结束 test 时，a被释放，引用无效, foo 会输出一个随机数

void test1() {
  int a = 1;
  thread2 = std::thread(foo1, &a);
} // 主线程结束 test 时，a被释放，引用无效, 但是子线程继续运行 foo 会输出一个随机数

void test2() {
  std::cout << "***** in test2 *****" << std::endl;
  int* a = new int(1);
  std::cout << "a before foo = " << *a << std::endl;
  thread2 = std::thread(foo1, a);
  delete a;
  std::cout << "after delete, *a = " << *a << std::endl;

} // 这里指针指向的内容先被释放，因此原先的内容已不存在, foo 中 反引用取得的应该是 0,因为输出的是1

void test3() {
  std::cout << "***** in test3 *****" << std::endl;
  int* a = new int(1);
  std::cout << "a before foo = " << *a << std::endl;
  thread2 = std::thread(foo1, a);
  delete a;
  std::cout << "after delete, *a = " << *a << std::endl;
} // 经测试，test3 分配了与 test2 中相同位置的内存, 在 delete 后，这块内存可重新被分配，但是其中的值不变

void test4() {
  MyClass obj;
  std::cout << "***** in test4 *****" << std::endl;

  thread2 = std::thread(&MyClass::func, &obj); // 传入类的非静态成员函数需要加上 &
} // 类成员函数正常调用，类的成员变量输出异常

void test5() {
  std::shared_ptr<MyClass> ptr = std::make_shared<MyClass>();
  std::cout << "***** in test5 *****" << std::endl;

  thread2 = std::thread(&MyClass::func, ptr);
} // 在堆区创建，使用智能指针管理

int main() {
//  // 1.传递临时变量问题
//  int a = 1;
////  std::thread thread1(foo, a); // 字面量在传递后会被销毁，因此不能够使用引用接收
//  std::thread thread1(foo, std::ref(a)); // 使用 ref 处理 foo 使用引用接收的情况
//  if (thread1.joinable()) thread1.join();
//  std::cout << a << std::endl;

  // 2.传递引用指向局部变量的问题
  test();
  thread2.join();

  // 3. 传递指针会指向已经释放的内存问题
  test1();
  thread2.join();

  // 4. 尝试堆区情况
  test2();
  thread2.join();

  // 5. 指针置空, debug 产生错误
  test3();
  thread2.join();

  // 6. 类对象被提前释放
  test4();
  thread2.join();

  // 7. 使用智能指针
  test5();
  thread2.join();
}