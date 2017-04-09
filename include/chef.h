/* -----------------------------------------------------------------------------
Program Name:   chef.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Analyze
----------------------------------------------------------------------------- */
#ifndef CHEF_H
#define CHEF_H

#include "pointers.h"

namespace Bistro_NS {

class Chef : protected Pointers {
  std::vector<std::string> property_requests;
  // Your Analysis Classes
  //  class Anal_YourAnalysis *yourA;
  std::vector<class Anal_AddTag *> addT;
  class Anal_PropertyRequest *propertyR;
  class Anal_BeadDump *beadD;
  class Anal_GyrationRadius *gyrationR;
  class Anal_Orientation *orientation;
  class Anal_NumberDensityDistribution *numberDD;
  class Anal_RadialDistributionFunction *radialDF;
  class Anal_MeanSquareDisplacement *meanSD;
  class Anal_Count *count;
  class Anal_WriteID *writeID;
  //
  template <class T> void add_property_request(T *ana) {
    for (std::string prop : ana->get_property_request()) {
      property_requests.push_back(prop);
    }
  }
public:
  // Constructor & Destructor
  Chef(Bistro *);
  ~Chef();
  // Functions
  void cook(class Ingredient *);      // 各時刻において解析・処理 & トラジェクトリに追加
  void serve();                       // トラジェクトリを解析・出力
  std::vector<std::string> get_property_requests() { return property_requests; }
};

}

#endif
