#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/PriorityQueue.h"
#include <chrono>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Hàm sinh nhãn ngẫu nhiên
Label_Container randomLabel() {
  int r = rand() % 3;
  if (r == 0)
    return Label_Container::DANGER;
  if (r == 1)
    return Label_Container::RF;
  return Label_Container::GP;
}

// Hàm chạy Benchmark cho quy mô N container
void runBenchmark(int N) {
  cout << "--------------------------------------------------------\n";
  cout << ">>> DO HIEU NANG VOI QUY MO N = " << N << " CONTAINERS <<<\n";
  cout << "--------------------------------------------------------\n";

  priority_queue pq;

  // 1. DO THOI GIAN PUSH
  auto start_push = chrono::high_resolution_clock::now();
  for (int i = 0; i < N; i++) {
    string id = "CONT_" + to_string(i);
    double weight =
        5.0 + (rand() % 350) / 10.0; // Random khối lượng từ 5.0 đến 40.0 tấn
    pq.push({randomLabel(), id, Status_Container::in_yard, weight, "TK"});
  }
  auto end_push = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> time_push = end_push - start_push;

  cout << "  [1] Thoi gian PUSH " << N << " container: " << fixed
       << setprecision(3) << time_push.count() << " ms\n";
  cout << "      -> Trung binh moi lan push: "
       << (time_push.count() * 1000.0 / N) << " microseconds (us)\n";

  // 2. DO THOI GIAN POP (Xuat kho theo dung thu tu Heap)
  auto start_pop = chrono::high_resolution_clock::now();
  while (!pq.empty()) {
    pq.pop();
  }
  auto end_pop = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> time_pop = end_pop - start_pop;

  cout << "  [2] Thoi gian POP sach " << N << " container: " << fixed
       << setprecision(3) << time_pop.count() << " ms\n";
  cout << "      -> Trung binh moi lan pop: " << (time_pop.count() * 1000.0 / N)
       << " microseconds (us)\n";

  double tong = time_push.count() + time_pop.count();
  cout << "  => TONG THOI GIAN: " << tong << " ms\n\n";
}

int main() {
  srand(42); // Seed co dinh de ket qua khach quan

  cout << "========================================================\n";
  cout << "   BENCHMARK HIEU NANG PRIORITY QUEUE (MAX-HEAP) - D5   \n";
  cout << "========================================================\n\n";

  // Mốc 1: 10.000 container
  runBenchmark(10000);

  // Mốc 2: 50.000 container (gấp 5 lần)
  runBenchmark(50000);

  return 0;
}
