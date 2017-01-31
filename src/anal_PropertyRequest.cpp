#include <sys/stat.h>
#include "utility.h"
#include "anal_PropertyRequest.h"
using namespace Bistro_NS;

Anal_PropertyRequest::Anal_PropertyRequest(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    for (std::string str : split(line,' ')) { property_request.push_back(str); }
  }
}
