#ifndef ANAL_ADDTAG_H
#define ANAL_ADDTAG_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_AddTag : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
  // 読み込みパラメータ
  std::string target, tagname;
public:
  // Constructor & Destructor
  Anal_AddTag(Bistro *, const std::vector<std::string> &);
  // Functions
  void analysis_instant(class Ingredient *);
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
