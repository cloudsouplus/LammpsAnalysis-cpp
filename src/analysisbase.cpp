#include "analysisbase.h"
#include "utility.h"
using namespace Bistro_NS;

void AnalysisBase::add_range(const std::vector<std::string> &strs) {
  std::string::size_type index = strs[3].find(":");
  if (index == std::string::npos)  {
    range_name.push_back(strs[3]);
    range[strs[3]] = {std::stod(strs[1]),std::stod(strs[5])}; }
  else {
    std::vector<std::string> strs2 = split(strs[3],':');
    if (strs2[0] == "memberatom") {
      std::string ridname_tmp = strs2[1]+":"+strs2[2];
      range_ridname_aim.push_back(ridname_tmp);
      range_aim[ridname_tmp] = {std::stod(strs[1]),std::stod(strs[5])}; }
    else if (strs2[0] == "memberbead") {
      std::string ridname_tmp = strs2[1]+":"+strs2[2];
      range_ridname_bim.push_back(ridname_tmp);
      range_bim[ridname_tmp] = {std::stod(strs[1]),std::stod(strs[5])}; }
    else if (strs2[0] == "molecule") {
      range_name_mol.push_back(strs2[1]);
      range_mol[strs2[1]] = {std::stod(strs[1]),std::stod(strs[5])}; }
    else { error("Error(Range): Unknown string in Order: "+strs2[0]+"."); }
  }
}

void AnalysisBase::add_list(const std::vector<std::string> &strs) {
  std::string::size_type index = strs[1].find(":");
  if (index == std::string::npos)  {
    list_name.push_back(strs[1]);
    for (auto str = strs.begin()+2; str != strs.end(); ++str) {
      (list[*(strs.begin()+1)]).push_back(std::stoi(*str));
    } }
  else {
    std::vector<std::string> strs2 = split(strs[1],':');
    if (strs2[0] == "memberatom") {
      std::string ridname_tmp = strs2[1]+":"+strs2[2];
      list_ridname_aim.push_back(ridname_tmp);
      for (auto str = strs.begin()+2; str != strs.end(); ++str) {
        (list_aim[ridname_tmp]).push_back(std::stoi(*str));
      } }
    else if (strs2[0] == "memberbead") {
      std::string ridname_tmp = strs2[1]+":"+strs2[2];
      list_ridname_bim.push_back(ridname_tmp);
      for (auto str = strs.begin()+2; str != strs.end(); ++str) {
        (list_bim[ridname_tmp]).push_back(std::stoi(*str));
      } }
    else if (strs2[0] == "molecule") {
      list_name_mol.push_back(strs2[1]);
      for (auto str = strs.begin()+2; str != strs.end(); ++str) {
        (list_mol[strs2[1]]).push_back(std::stoi(*str));
      } }
    else { error("Error(List): Unknown string in Order: "+strs2[0]+"."); }
  }
}

std::vector<Molecule *> AnalysisBase::extract(std::vector<Molecule *> ps) {
  std::vector<Molecule *> qs;
  for (Molecule *p : ps) {
    //
    bool check1 = [=] {
      for (std::string name : range_name) {
        if (p->dprop[name] < (range[name])[0]) { return false; }
        if (p->dprop[name] > (range[name])[1]) { return false; }
      }
      return true;
    } ();
    if (!check1) { continue; }
    //
    bool check_aim1 = [=] {
      for (std::string ridname : range_ridname_aim) {
        std::vector<std::string> strs = split(ridname,':');
        int rid = std::stoi(strs[0]);
        std::string name = strs[1];
        if ((p->memberatoms)[rid-1]->dprop[name] < (range_aim[ridname])[0]) {
          return false;
        }
        if ((p->memberatoms)[rid-1]->dprop[name] > (range_aim[ridname])[1]) {
          return false;
        }
      }
      return true;
    } ();
    if (!check_aim1) { continue; }
    //
    bool check_bim1 = [=] {
      for (std::string ridname : range_ridname_bim) {
        std::vector<std::string> strs = split(ridname,':');
        int rid = std::stoi(strs[0]);
        std::string name = strs[1];
        if ((p->memberbeads)[rid-1]->dprop[name] < (range_bim[ridname])[0]) {
          return false;
        }
        if ((p->memberbeads)[rid-1]->dprop[name] > (range_bim[ridname])[1]) {
          return false;
        }
      }
      return true;
    } ();
    if (!check_bim1) { continue; }
    //
    bool check2 = [=] {
      for (std::string name : list_name) {
        if (!find_int(list[name],p->iprop[name])) { return false; }
      }
      return true;
    } ();
    if (!check2) { continue; }
    //
    bool check_aim2 = [=] {
      for (std::string ridname : list_ridname_aim) {
        std::vector<std::string> strs = split(ridname,':');
        int rid = std::stoi(strs[0]);
        std::string name = strs[1];
        if (!find_int(list_aim[ridname],(p->memberatoms)[rid-1]->iprop[name])) {
          return false;
        }
      }
      return true;
    } ();
    if (!check_aim2) { continue; }
    //
    bool check_bim2 = [=] {
      for (std::string ridname : list_ridname_bim) {
        std::vector<std::string> strs = split(ridname,':');
        int rid = std::stoi(strs[0]);
        std::string name = strs[1];
        if (!find_int(list_bim[ridname],(p->memberbeads)[rid-1]->iprop[name])) {
          return false;
        }
      }
      return true;
    } ();
    if (check_bim2) { qs.push_back(p); }
  }
  return qs;
}
