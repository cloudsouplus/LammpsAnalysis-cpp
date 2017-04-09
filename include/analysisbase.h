#ifndef ANALYSISBASE_H
#define ANALYSISBASE_H

#include <vector>
#include "ingredient.h"
#include "pointers.h"

namespace Bistro_NS {

// 解析用基本クラス
class AnalysisBase : protected Pointers {
protected:
  //// 実数プロパティの範囲で選別するためのメンバ
  std::vector<std::string> range_name;
  std::map<std::string,std::vector<double>> range;
  // 分子内原子（Atom In Molecule）
  std::vector<std::string> range_ridname_aim;
  std::map<std::string,std::vector<double>> range_aim;
  // 分子内ビーズ（Bead In Molecule）
  std::vector<std::string> range_ridname_bim;
  std::map<std::string,std::vector<double>> range_bim;
  // 原子 or ビーズから分子
  std::vector<std::string> range_name_mol;
  std::map<std::string,std::vector<double>> range_mol;
  // 関数
  void add_range(const std::vector<std::string> &strs);
  //// 整数プロパティのリストで選別するためのメンバ
  std::vector<std::string> list_name;
  std::map<std::string,std::vector<int>> list;
  // 分子内原子（Atom In Molecule）
  std::vector<std::string> list_ridname_aim;
  std::map<std::string,std::vector<int>> list_aim;
  // 分子内ビーズ（Bead In Molecule）
  std::vector<std::string> list_ridname_bim;
  std::map<std::string,std::vector<int>> list_bim;
  // 原子 or ビーズから分子
  std::vector<std::string> list_name_mol;
  std::map<std::string,std::vector<int>> list_mol;
  // 関数
  void add_list(const std::vector<std::string> &strs);
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
      bool check_mol1 = [=] {
        for (std::string name : range_name_mol) {
          if (p->parentmol->dprop[name] < (range_mol[name])[0]) { return false; }
          if (p->parentmol->dprop[name] > (range_mol[name])[1]) { return false; }
        }
        return true;
      } ();
      if (!check_mol1) { continue; }
      bool check2 = [=] {
        for (std::string name : list_name) {
          if (!find_int(list[name],p->iprop[name])) { return false; }
        }
        return true;
      } ();
      if (!check2) { continue; }
      bool check_mol2 = [=] {
        for (std::string name : list_name_mol) {
          if (!find_int(list_mol[name],p->parentmol->iprop[name])) { return false; }
        }
        return true;
      } ();
      if (check_mol2) { qs.push_back(p); }
    }
    return qs;
  }
  // 分子用関数をオーバーロード
  std::vector<Molecule *> extract(std::vector<Molecule *> ps);
public:
  // Constructor & Destructor
  AnalysisBase(Bistro *ptr) : Pointers(ptr) {}
};

}

#endif
