#ifndef ANAL_WRITEID_H
#define ANAL_WRITEID_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_WriteID : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
  // 解析対象
  std::string target;
  // 解析条件
  std::vector<std::string> conditions;
  // 出力用
  std::string odir;
public:
  // Constructor & Destructor
  Anal_WriteID(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
