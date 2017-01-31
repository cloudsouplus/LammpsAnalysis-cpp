#ifndef PROP_ORIENTATION_H
#define PROP_ORIENTATION_H

#include "pointers.h"

namespace Bistro_NS {

class Prop_Orientation : protected Pointers {
  // 必要なプロパティ名
  std::vector<std::string> property_request = {"InertiaMoment"};
public:
  // Constructor & Destructor
  Prop_Orientation(Bistro *bstr) : Pointers(bstr) {}
  // Functions
  void calculate_property(class Ingredient *);
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
