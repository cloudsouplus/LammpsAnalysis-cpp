#ifndef PROP_INERTIAMOMENT_H
#define PROP_INERTIAMOMENT_H

#include "pointers.h"

namespace Bistro_NS {

class Prop_InertiaMoment : protected Pointers {
  // 必要なプロパティ名
  std::vector<std::string> property_request;
public:
  // Constructor & Destructor
  Prop_InertiaMoment(Bistro *bstr) : Pointers(bstr) {}
  // Functions
  void calculate_property(class Ingredient *);
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
