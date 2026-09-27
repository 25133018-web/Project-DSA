#include "Structures.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

string labelToString(Label_Container label) {
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

string statusToString(Status_Container status) {
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