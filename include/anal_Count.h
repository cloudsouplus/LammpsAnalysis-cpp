#ifndef ANAL_COUNT_H
#define ANAL_COUNT_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_Count : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
  // 解析対象
  std::string target;
  // トラジェクトリ：時刻 -> 分子 ->
  std::vector<int> Count_traj;
public:
  // Constructor & Destructor
  Anal_Count(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  void analysis_trajectory();
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
