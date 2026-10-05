#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/PriorityQueue.h"
#include <string>

int total_tests = 0;
int passed_test = 0;

void CHECK(const string &ten_bai_test, bool dieu_kien_dung) {
  total_tests++;
  if (dieu_kien_dung) {
    passed_test++;
    cout << " [PASS] " << ten_bai_test << endl;
  } else {
    cout << " [FAIL] " << ten_bai_test << " -> (KET QUA SAI)" << endl;
  }
}

int main() {
  priority_queue pq;
  for (int i = 1; i <= 25; i++) {
    pq.push({Label_Container::GP, "C_" + to_string(i),
             Status_Container::in_yard, (double)i, "TK"});
  }
  CHECK("Nap 25 phan tu vuot qua capacity 16 khong bi crash", pq.size() == 25);
  CHECK("Phan tu nang 25 kg phai o tren dinh", pq.top().gross_weight == 25.0);
  cout << " Ket qua: Dat " << passed_test << "/" << total_tests << "test."
       << endl;
  return 0;
}