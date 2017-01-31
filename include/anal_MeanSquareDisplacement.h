#ifndef ANAL_MEANSQUAREDISPLACEMENT_H
#define ANAL_MEANSQUAREDISPLACEMENT_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_MeanSquareDisplacement : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request = {"SystemDrift"};
  // 読み込みパラメータ
  std::string dimension, target;
  int numsamples;
  // トラジェクトリ：時刻 -> 粒子 ->
  std::vector<std::vector<Eigen::Vector3d>> points_traj;
  std::vector<std::vector<int>> ids_traj;
public:
  // Constructor & Destructor
  Anal_MeanSquareDisplacement(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  void analysis_trajectory();
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
