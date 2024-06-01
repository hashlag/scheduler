#include "libsched/scheduler/scheduler.h"
#include <gtest/gtest.h>
#include <format>

using namespace sched;

TEST(libsched, basics) {
  Scheduler scheduler;

  auto f0 = [](int a, int b){return a + b;};
  auto f1 = [](int a, float b){
	return static_cast<float>(a) + b;
  };
  auto f2 = [](const std::string& a, float b){
	return a + " " + std::format("{:.1f}", b);
  };

  int a = 10;

  auto t0 = scheduler.add(f0, a, 11);
  auto t1 = scheduler.add(f1, scheduler.get_future_result<int>(t0), 0.5);
  auto t2 = scheduler.add(f2, "abc", scheduler.get_result<float>(t1));

  ASSERT_EQ(scheduler.get_result<std::string>(t2), "abc 21.5");
}

TEST(libsched, result_caching) {
  Scheduler scheduler;

  size_t counter = 0;

  auto f0 = [&counter](int a, int b){
	++counter;
	return a + b;
  };

  auto t0 = scheduler.add(f0, 5, 10);

  ASSERT_EQ(scheduler.get_result<int>(t0), 15);
  ASSERT_NE(scheduler.get_result<int>(t0), 0);
  ASSERT_EQ(counter, 1);
}

TEST(libsched, multiple_arguments) {
  Scheduler scheduler;

  auto f0 = [](int a, short b, std::string c, std::string d){
	return std::to_string(a) + std::to_string(b) + c + d;
  };
  auto f1 = [](bool a, bool b, bool c, bool d, bool e, const std::string& f){
	return f + " " + std::to_string(a + b + c + d + e);
  };

  std::string c = "a";

  auto t0 = scheduler.add(f0, 100, 400, c, "b");
  auto t0_future_result = scheduler.get_future_result<std::string>(t0);

  auto t1 = scheduler.add(f1, true, false, false, true, true, t0_future_result);

  ASSERT_EQ(scheduler.get_result<std::string>(t1), "100400ab 3");
}

TEST(libsched, laziness) {
  Scheduler scheduler;

  size_t counter = 0;

  auto f0 = [&counter](int a, int b){
	++counter;
	return a + b;
  };
  auto f1 = [&counter](int a, int b){
	++counter;
	return a + b;
  };
  auto f2 = [&counter](int a, int b){
	++counter;
	return a + b;
  };

  int n = 2;

  auto t0 = scheduler.add(f0, 90, 10);
  auto t1 = scheduler.add(f1, n, scheduler.get_future_result<int>(t0));
  auto t2 = scheduler.add(f2, scheduler.get_future_result<int>(t1), 98);

  auto t2_future_result = scheduler.get_future_result<int>(t2);

  ASSERT_EQ(counter, 0);
  ASSERT_EQ(scheduler.get_result<int>(t1), 102);
  ASSERT_EQ(counter, 2);
  ASSERT_EQ(scheduler.get_result<int>(t2), 200);
  ASSERT_EQ(counter, 3);
}

TEST(libsched, execute_all) {
  Scheduler scheduler;

  size_t counter = 0;

  auto f0 = [&counter](int a, int b){
	++counter;
	return a + b;
  };
  auto f1 = [&counter](int a, int b){
	++counter;
	return a + b;
  };
  auto f2 = [&counter](int a, int b){
	++counter;
	return a + b;
  };

  int n = 2;

  auto t0 = scheduler.add(f0, 90, 10);
  auto t1 = scheduler.add(f1, n, scheduler.get_future_result<int>(t0));
  auto t2 = scheduler.add(f2, scheduler.get_future_result<int>(t1), 98);

  scheduler.execute_all();

  ASSERT_EQ(counter, 3);
  ASSERT_EQ(scheduler.get_result<int>(t2), 200);
  ASSERT_EQ(counter, 3);
}

TEST(libsched, task_order_independence) {
  Scheduler scheduler;

  size_t counter = 0;

  auto f0 = [&counter](int a, int b){
	++counter;
	return a + b;
  };
  auto f1 = [&counter](int& a, int b){
	++counter;
	return a + b;
  };
  auto f2 = [&counter](int a, int b){
	++counter;
	return a + b;
  };

  int n = -1000;

  auto t1 = scheduler.add(f1, std::ref(n), 2);

  auto t0 = scheduler.add(f0, 90, 10);
  n = scheduler.get_result<int>(t0);

  auto t2 = scheduler.add(f2, scheduler.get_future_result<int>(t1), 98);

  ASSERT_EQ(counter, 1);
  ASSERT_EQ(scheduler.get_result<int>(t2), 200);
  ASSERT_EQ(counter, 3);
}

int add_(int a, int b) {
  return a + b;
}

TEST(libsched, func_pointers) {
  Scheduler scheduler;

  int n = 2;

  auto t0 = scheduler.add(add_, 90, 10);
  auto t1 = scheduler.add(add_, n, scheduler.get_future_result<int>(t0));
  auto t2 = scheduler.add(add_, scheduler.get_future_result<int>(t1), 98);

  ASSERT_EQ(scheduler.get_result<int>(t2), 200);
}

//TEST(libsched, void_func) {
//  Scheduler scheduler;
//
//  size_t counter = 0;
//
//  auto f0 = [&counter](){
//	counter = 3107;
//  };
//
//  auto t0 = scheduler.add(f0);
//
//  scheduler.get_result<void>(t0);
//  ASSERT_EQ(counter, 3107);
//}
