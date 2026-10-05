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
  Container c_thuong = {Label_Container::GP, "GP_CONT",
                        Status_Container::in_yard, 20.0, "TK1"};
  Container c_nguy_hiem = {Label_Container::DANGER, "DANGER_CONT",
                           Status_Container::in_yard, 20.0, "TK2"};
  pq.push(c_thuong);
  pq.push(c_nguy_hiem);
  CHECK("Cung can nang thi DANGER phai duoc uu tien hon GP",
        pq.top().container_id == "DANGER_CONT");
  cout << " Ket qua: Dat " << passed_test << "/" << total_tests << "test."
       << endl;
  return 0;
}