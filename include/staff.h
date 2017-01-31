/* -----------------------------------------------------------------------------
Program Name:   staff.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Property Calculation
----------------------------------------------------------------------------- */
#ifndef STAFF_H
#define STAFF_H

#include "pointers.h"

namespace Bistro_NS {

class Staff : protected Pointers {
  std::vector<std::string> property_requests;
  // Your Property Classes
//  class Prop_YourProperty *yourP;
  class Prop_Integer *intgr;
  class Prop_Double *dble;
  class Prop_InertiaMoment *inertiaM;
  class Prop_GyrationRadius *gyrationR;
  class Prop_Orientation *orientation;
  class Prop_SystemDrift *systemD;
  //
  template <class T> void add_property_request(T *pro) {
    for (std::string prop : pro->get_property_request()) {
      property_requests.push_back(prop);
    }
  }
public:
  // Constructor & Destructor
  Staff(Bistro *);
  ~Staff();
  // Functions
  void precook(class Ingredient *);   // 各時刻においてプロパティ計算
};

}

#endif
