#ifndef ANAL_BEADDUMP_H
#define ANAL_BEADDUMP_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_BeadDump : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
  // 出力用
  std::string odir;
public:
  // Constructor & Destructor
  Anal_BeadDump(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
