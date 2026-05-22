#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// ==============================
// 【坏写法】一把大锁
// ==============================
mutex global_big_lock;
int bad_a = 0;
int bad_b = 0;

void bad_increment() {
    for (int i = 0; i < 1000000; ++i) {
        lock_guard<mutex> lock(global_big_lock);
        bad_a++;
        bad_b++;
    }
}

// ==============================
// 【好写法】细粒度锁（修复版，可编译）
// ==============================
mutex lock_a, lock_b;
int good_a = 0;
int good_b = 0;

void good_increment() {
    for (int i = 0; i < 1000000; ++i) {
        { std::lock_guard<std::mutex> lk_a(lock_a); good_a++; }
        { std::lock_guard<std::mutex> lk_b(lock_b); good_b++; }
    }
}

// ==============================
// 测试
// ==============================
void test(bool use_bad) {
    vector<thread> threads;
    for (int i = 0; i < 8; ++i) {
        if (use_bad)
            threads.emplace_back(bad_increment);
        else
            threads.emplace_back(good_increment);
    }
    for (auto& t : threads) t.join();
}

int main() {
    auto start1 = high_resolution_clock::now();
    test(true);
    auto end1 = high_resolution_clock::now();
    cout << "[粗锁] 耗时: " << duration_cast<milliseconds>(end1 - start1).count() << "ms\n";

    auto start2 = high_resolution_clock::now();
    test(false);
    auto end2 = high_resolution_clock::now();
    cout << "[细锁] 耗时: " << duration_cast<milliseconds>(end2 - start2).count() << "ms\n";

    return 0;
}