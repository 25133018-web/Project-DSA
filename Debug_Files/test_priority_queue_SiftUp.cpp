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
  Container nhe = {Label_Container::GP, "C_NHE", Status_Container::in_yard, 6,
                   "TK1"};
  Container nang = {Label_Container::GP, "C_NANG", Status_Container::in_yard,
                    10, "TK2"};
  Container trung = {Label_Container::GP, "C_TRUNG", Status_Container::in_yard,
                     6.5, "TK3"};

  pq.push(nhe);
  pq.push(nang);
  pq.push(trung);

  CHECK("Lay con dau tien cau len tau phai la con nang",
        pq.top().container_id == "C_NANG");
  pq.pop();
  CHECK("Lay con thu hai cau len tau phai la con trung binh",
        pq.top().container_id == "C_TRUNG");
  pq.pop();
  CHECK("Lay con thu ba cau len tau phai la con nhe",
        pq.top().container_id == "C_NHE");
  pq.pop();
  CHECK("Khong con con nao nen hang cau rong", pq.empty() == true);
  cout << " Ket qua: Dat " << passed_test << "/" << total_tests << "test."
       << endl;
  return 0;
}