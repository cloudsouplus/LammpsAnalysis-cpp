#ifndef ANAL_ORIENTATION_H
#define ANAL_ORIENTATION_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_Orientation : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request = {"Orientation"};
  // 読み込みパラメータ
  std::string direc;
  // トラジェクトリ：時刻 -> 分子 ->
  std::vector<std::vector<double>> S_traj;
public:
  // Constructor & Destructor
  Anal_Orientation(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  void analysis_trajectory();
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
