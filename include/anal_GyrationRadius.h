#ifndef ANAL_GYRATIONRADIUS_H
#define ANAL_GYRATIONRADIUS_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_GyrationRadius : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request = {"GyrationRadius"};
  // 読み込みパラメータ
  std::string dim;
  // トラジェクトリ：時刻 -> 分子 ->
  std::vector<std::vector<double>> Rg2_traj;
public:
  // Constructor & Destructor
  Anal_GyrationRadius(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  void analysis_trajectory();
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
