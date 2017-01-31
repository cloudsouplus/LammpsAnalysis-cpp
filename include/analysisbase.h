#ifndef ANALYSISBASE_H
#define ANALYSISBASE_H

#include <vector>
#include "pointers.h"
#include "ingredient.h"

namespace Bistro_NS {

// 解析用基本クラス
class AnalysisBase : protected Pointers {
protected:
  // 実数プロパティの範囲で選別するためのメンバ
  std::vector<std::string> range_name;
  std::map<std::string,std::vector<double>> range;
  void add_range(const std::vector<std::string> &strs) {
    range_name.push_back(strs[3]);
    range[strs[3]] = {std::stod(strs[1]),std::stod(strs[5])};
  }
  // 整数プロパティのリストで選別するためのメンバ
  std::vector<std::string> list_name;
  std::map<std::string,std::vector<int>> list;
  void add_list(const std::vector<std::string> &strs) {
    list_name.push_back(strs[1]);
    for (auto str = strs.begin()+2; str != strs.end(); ++str) {
      (list[*(strs.begin()+1)]).push_back(std::stoi(*str));
    }
  }
  // 選別関数：条件を満たすデータクラスポインタのベクトルを返す
  template <class T> std::vector<T *> extract(std::vector<T *> ps) {
    std::vector<T *> qs;
    for (T *p : ps) {
      bool check1 = [=] {
        for (std::string name : range_name) {
          if (p->dprop[name] < (range[name])[0]) { return false; }
          if (p->dprop[name] > (range[name])[1]) { return false; }
        }
        return true;
      } ();
      if (!check1) { continue; }
      bool check2 = [=] {
        for (std::string name : list_name) {
          if (!find_int(list[name],p->iprop[name])) { return false; }
        }
        return true;
      } ();
      if (check2) { qs.push_back(p); }
    }
    return qs;
  }
public:
  // Constructor & Destructor
  AnalysisBase(Bistro *ptr) : Pointers(ptr) {}
};

}

#endif
