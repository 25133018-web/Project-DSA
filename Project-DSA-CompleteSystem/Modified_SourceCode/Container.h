#pragma once

#include <iostream>
#include <list>
#include <string>

using namespace std;

const int MAX_YARD_CAPACITY = 15000;

enum class Status_Container {
  pre_gate = 1, // cho vao cong
  in_yard = 2,  // da duoc add vao trong bai
  released = 3  // da giai phong khoi bai
};

enum class Label_Container {
  DANGER = 1, // hang nguy hiem
  RF = 2,     // hang dong lanh
  GP = 3      // hang thuong
};

inline string labelToString(Label_Container label) {
  switch (label) {
  case Label_Container::DANGER:
    return "Hang nguy hiem";
  case Label_Container::RF:
    return "Hang dong lanh";
  case Label_Container::GP:
    return "Hang thuong";
  default:
    return "Unknown";
  }
}

inline string statusToString(Status_Container status) {
  switch (status) {
  case Status_Container::pre_gate:
    return "Pre Gate";
  case Status_Container::in_yard:
    return "In Yard";
  case Status_Container::released:
    return "Realeased";
  default:
    return "Unknown";
  }
}

struct Container {
  Label_Container container_label; // nhãn container
  string container_id;             // id container
  Status_Container status;         // trạng thái container
  double gross_weight;             // khối lượng container
  string customs_declaration_no;   // mã tờ khai

  // Nạp chồng toán tử so sánh các container
  bool operator<(const Container &other) const {
    // top1. Ưu tiên theo cân nặng lớn hơn
    if (gross_weight != other.gross_weight)
      return gross_weight < other.gross_weight;
    // top2. Ưu tiên theo nhãn nghiêm trọng hơn
    if (container_label != other.container_label)
      return static_cast<int>(container_label) >
             static_cast<int>(other.container_label);
    // top3. Ưu tiên theo id nhỏ hơn
    return container_id > other.container_id;
  }
};

struct Vessel {
  list<Container> ContainerList;
};