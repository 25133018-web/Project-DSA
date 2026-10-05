#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  // Đường dẫn mặc định xuất ra file database của dự án
  string output_file =
      "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/"
      "containers_database.csv";

  if (argc > 1) {
    output_file = argv[1];
  }

  int total_containers = 10000;
  int in_yard_count = 8000;
  int pre_gate_count = 2000;

  // Danh sách các hãng tàu vận tải biển quốc tế phổ biến
  vector<string> shipping_lines = {"MAEU", "COSU", "EVER", "ONEU",
                                   "CMAU", "SGNG", "YMLU", "HMMU"};

  // Tạo sẵn 1500 mã tờ khai hải quan (dạng 12 số: 100000000001 -> 100000001500)
  int num_declarations = 1500;
  vector<string> declarations;
  declarations.reserve(num_declarations);
  for (int i = 1; i <= num_declarations; i++) {
    string d = "100000000000";
    string numStr = to_string(i);
    d.replace(d.length() - numStr.length(), numStr.length(), numStr);
    declarations.push_back(d);
  }

  ofstream fout(output_file);
  if (!fout.is_open()) {
    cerr << "[LOI] Khong the mo file: " << output_file << " de ghi!\n";
    return 1;
  }

  // Ghi tiêu đề CSV
  fout << "label,id,status,weight,declaration\n";

  srand(2026); // Seed cố định để dữ liệu có tính tái lập (Reproducible)

  int count_danger = 0, count_rf = 0, count_gp = 0;

  for (int i = 0; i < total_containers; i++) {
    // 1. Phân loại trạng thái: 8.000 đầu là in_yard, 2.000 sau là pre_gate
    string status = (i < in_yard_count) ? "in_yard" : "pre_gate";

    // 2. Sinh ID: Hãng tàu + 7 chữ số duy nhất
    string line = shipping_lines[i % shipping_lines.size()];
    int numeric_id = 1000000 + i + 1; // 1000001 -> 1010000
    string id = line + to_string(numeric_id);

    // 3. Phân bố nhãn: 10% DANGER, 25% RF, 65% GP
    int rLabel = rand() % 100;
    string label;
    if (rLabel < 10) {
      label = "DANGER";
      count_danger++;
    } else if (rLabel < 35) {
      label = "RF";
      count_rf++;
    } else {
      label = "GP";
      count_gp++;
    }

    // 4. Khối lượng: ngẫu nhiên từ 2.5 đến 32.5 tấn
    double weight = 2.5 + (rand() % 301) / 10.0; // Bước nhảy 0.1 tấn

    // 5. Gán mã tờ khai (mỗi tờ khai gom cụm 6-7 container)
    string decl = declarations[i % num_declarations];

    fout << label << "," << id << "," << status << "," << fixed
         << setprecision(1) << weight << "," << decl << "\n";
  }

  fout.close();

  cout << "=========================================================\n";
  cout << "  SINH THANH CONG DU LIEU 10.000 CONTAINER CHUAN LOGISTICS\n";
  cout << "=========================================================\n";
  cout << "  * File xuat: " << output_file << "\n";
  cout << "  * Tong so container : " << total_containers << "\n";
  cout << "    - Trong bai (in_yard) : " << in_yard_count << "\n";
  cout << "    - Ngoai cong (pre_gate): " << pre_gate_count << "\n";
  cout << "  * Ty le loai hang:\n";
  cout << "    - DANGER (Nguy hiem)  : " << count_danger << " ("
       << fixed << setprecision(1) << (count_danger * 100.0 / total_containers)
       << "%)\n";
  cout << "    - RF (Hang dong lanh) : " << count_rf << " ("
       << fixed << setprecision(1) << (count_rf * 100.0 / total_containers)
       << "%)\n";
  cout << "    - GP (Hang thuong)    : " << count_gp << " ("
       << fixed << setprecision(1) << (count_gp * 100.0 / total_containers)
       << "%)\n";
  cout << "  * So luong ma to khai   : " << num_declarations
       << " (trung binh ~6.7 container/to khai)\n";
  cout << "=========================================================\n";

  return 0;
}
