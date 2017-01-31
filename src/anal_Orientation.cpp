#include <numeric>
#include "utility.h"
#include "anal_Orientation.h"
using namespace Bistro_NS;

Anal_Orientation::Anal_Orientation(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Direction") { direc = strs[1]; }
    else { error("Error(O): Unknown String in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  S_traj.resize(bstr->numitrs);
}

void Anal_Orientation::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Molecule *> ms = extract(ingr->get_mols());
  // 現時刻のデータ
  std::vector<double> S;
  // データ取得
  for (Molecule *m : ms) {
    S.push_back(m->dprop["S"+direc]);
  }
  // 現時刻のデータをトラジェクトリに追加
  S_traj[ingr->get_index()] = S;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "Orientation calculated: "+s+".\n";
}

void Anal_Orientation::analysis_trajectory() {
  // 全平均
  auto ave = [](double d, const std::vector<double> &v) {
    return d+std::accumulate(v.begin(),v.end(),0.0)/v.size(); };
  double aveS = std::accumulate(S_traj.begin(),S_traj.end(),0.0,ave)/S_traj.size();
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/Orientation-"+direc+".dat");
  // 出力
  ofs << "# Average : " << aveS << std::endl;
  ofs << "# time index | S of each molecule" << std::endl;
  for (int i = 0; i < S_traj.size(); ++i) {
    ofs << i;
    for (double S : S_traj[i]) { ofs << " " << S; }
    ofs << std::endl;
  }
  std::cout << "Orientation served.\n";
}
