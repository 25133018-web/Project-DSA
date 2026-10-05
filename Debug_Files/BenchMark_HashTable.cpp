#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/HashTable.h"
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Hàm sinh nhãn ngẫu nhiên
Label_Container getRandomLabel() {
  int r = rand() % 3;
  if (r == 0)
    return Label_Container::DANGER;
  if (r == 1)
    return Label_Container::RF;
  return Label_Container::GP;
}

// Hàm chạy Benchmark so sánh HashTable O(1) vs Linear Scan O(N) cho quy mô N
void runBenchmarkMC1(int N, int num_queries = 2000) {
  cout << "==============================================================\n";
  cout << ">>> THI NGHIEM QUY MO N = " << N << " CONTAINER (DO " << num_queries
       << " TRUY VAN) <<<\n";
  cout << "==============================================================\n";

  // Danh sách các hãng tàu
  vector<string> shipping_lines = {"MAEU", "COSU", "EVER", "ONEU",
                                   "CMAU", "SGNG", "YMLU", "HMMU"};

  // 1. CHUẨN BỊ TẬP DỮ LIỆU N BẢN GHI
  vector<Container> dataset;
  dataset.reserve(N);

  for (int i = 0; i < N; i++) {
    string prefix = shipping_lines[i % shipping_lines.size()];
    string id = prefix + to_string(1000000 + i);
    double weight = 2.5 + (rand() % 300) / 10.0;
    string decl = "10000000" + to_string(1000 + (i % 500));
    dataset.push_back(
        {getRandomLabel(), id, Status_Container::in_yard, weight, decl});
  }

  // 2. NẠP DỮ LIỆU VÀO HASHTABLE VÀ MẢNG TUYẾN TÍNH
  HashTable ht;
  auto start_load_ht = chrono::high_resolution_clock::now();
  for (const auto &c : dataset) {
    ht.insert(c);
  }
  auto end_load_ht = chrono::high_resolution_clock::now();
  chrono::duration<double, milli> time_load_ht = end_load_ht - start_load_ht;

  cout << "  [BULK LOAD] Thoi gian nap " << N
       << " container vao HashTable: " << fixed << setprecision(2)
       << time_load_ht.count() << " ms\n\n";

  // 3. TẠO TẬP TRUY VẤN NGẪU NHIÊN (QUERY SET)
  // Chọn ngẫu nhiên num_queries mã ID từ dataset
  vector<string> queries;
  queries.reserve(num_queries);
  for (int i = 0; i < num_queries; i++) {
    int idx = rand() % N;
    queries.push_back(dataset[idx].container_id);
  }

  // -------------------------------------------------------------
  // THỰC NGHIỆM 1: QUÉT TUYẾN TÍNH - LINEAR SCAN O(N) (CÁCH TIẾP CẬN NGÂY THƠ)
  // -------------------------------------------------------------
  int linear_found = 0;
  auto start_linear = chrono::high_resolution_clock::now();
  for (const string &target_id : queries) {
    for (size_t j = 0; j < dataset.size(); j++) {
      if (dataset[j].container_id == target_id) {
        linear_found++;
        break;
      }
    }
  }
  auto end_linear = chrono::high_resolution_clock::now();
  auto time_linear_ns =
      chrono::duration_cast<chrono::nanoseconds>(end_linear - start_linear)
          .count();
  double time_linear_ms = time_linear_ns / 1000000.0;
  double avg_linear_us = (double)time_linear_ns / (num_queries * 1000.0);

  cout << "  [1] LINEAR SCAN (Quet tuyen tinh O(N) tren mang/list):\n";
  cout << "      * Tim thay " << linear_found << "/" << num_queries
       << " container\n";
  cout << "      * Tong thoi gian cho " << num_queries
       << " truy van : " << fixed << setprecision(3) << time_linear_ms
       << " ms\n";
  cout << "      * Thoi gian trung binh / 1 lan tra cuu    : " << fixed
       << setprecision(3) << avg_linear_us << " microseconds (us)\n";

  // -------------------------------------------------------------
  // THỰC NGHIỆM 2: BẢNG BĂM - HASHTABLE O(1) (GIẢI PHÁP CỐT LÕI CỦA NHÓM)
  // -------------------------------------------------------------
  volatile int ht_found = 0;
  auto start_ht = chrono::high_resolution_clock::now();
  for (const string &target_id : queries) {
    Container *found = ht.search(target_id);
    if (found != nullptr) {
      ht_found = ht_found + 1;
    }
  }
  auto end_ht = chrono::high_resolution_clock::now();
  auto time_ht_ns =
      chrono::duration_cast<chrono::nanoseconds>(end_ht - start_ht).count();
  double time_ht_ms = time_ht_ns / 1000000.0;
  double avg_ht_us = (double)time_ht_ns / (num_queries * 1000.0);

  cout << "\n  [2] HASHTABLE MC1 (Bang bam O(1) tu cai dat - djb2):\n";
  cout << "      * Tim thay " << ht_found << "/" << num_queries
       << " container\n";
  cout << "      * Tong thoi gian cho " << num_queries
       << " truy van : " << fixed << setprecision(3) << time_ht_ms << " ms\n";
  cout << "      * Thoi gian trung binh / 1 lan tra cuu    : " << fixed
       << setprecision(4) << avg_ht_us << " microseconds (us) ["
       << (time_ht_ns / num_queries) << " ns]\n";

  // -------------------------------------------------------------
  // SO SÁNH & KẾT LUẬN
  // -------------------------------------------------------------
  double speedup = (avg_ht_us > 0) ? (avg_linear_us / avg_ht_us) : 0;
  cout << "\n  ==> KET QUA SO SANH TAI QUY MO N = " << N << ":\n";
  cout << "      HashTable nhanh gap " << fixed << setprecision(1) << speedup
       << " LAN so voi Quet tuyen tinh!\n\n";
}

int main() {
  srand(42); // Seed co dinh de du lieu khach quan

  cout << "==============================================================\n";
  cout << "   BENCHMARK HIEU NANG TRA CUU MC1: HASHTABLE O(1) VS SCAN O(N)  \n";
  cout << "   Minh chung thuc nghiem theo Muc 5.3 & Deliverable D5      \n";
  cout << "==============================================================\n\n";

  // Mốc 1: 10.000 container (Quy mô chuẩn của cảng)
  runBenchmarkMC1(10000, 2000);

  // Mốc 2: 50.000 container (Gấp 5 lần để thấy rõ suy giảm của O(N) vs tính ổn
  // định của O(1))
  runBenchmarkMC1(50000, 2000);

  return 0;
}
