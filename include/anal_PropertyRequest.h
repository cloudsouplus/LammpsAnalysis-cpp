#ifndef ANAL_PROPERTYREQUEST_H
#define ANAL_PROPERTYREQUEST_H

#include "analysisbase.h"

namespace Bistro_NS {

class Anal_PropertyRequest : protected AnalysisBase {
  // 解析に必要なプロパティ名
  std::vector<std::string> property_request;
public:
  // Constructor & Destructor
  Anal_PropertyRequest(Bistro *, const std::vector<std::string> &);
  // Functions
  std::vector<std::string> get_property_request() { return property_request; }
};

}

#endif
