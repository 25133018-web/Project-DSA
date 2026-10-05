#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/PriorityQueue.h"

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
  CHECK("Hang doi vua tao co phai rong", pq.empty() == true);
  CHECK("Kich thuoc ban dau phai bang 0", pq.size() == 0);

  Container c1 = {Label_Container::GP, "MAEU1234567", Status_Container::in_yard,
                  20.5, "TK01"};
  pq.push(c1);
  CHECK("Sau khi push thi khong con rong", pq.empty() == false);
  CHECK("Phan tu tren dinh top phai la MAEU1234567",
        pq.top().container_id == "MAEU1234567");
  cout << " Ket qua: Dat " << passed_test << "/" << total_tests << "test."
       << endl;
  return 0;
}