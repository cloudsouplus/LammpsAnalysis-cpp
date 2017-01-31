#ifndef ANAL_RADIALDISTRIBUTIONFUNCTION_H
#define ANAL_RADIALDISTRIBUTIONFUNCTION_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_RadialDistributionFunction : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
  // 読み込みパラメータ
  std::string target;
  int numbins;
  double width;
  // トラジェクトリ：時刻 -> Bin ->
  std::vector<std::vector<double>> RDF_traj;
public:
  // Constructor & Destructor
  Anal_RadialDistributionFunction(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  void analysis_trajectory();
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
