#include <numeric>
#include "utility.h"
#include "anal_GyrationRadius.h"
using namespace Bistro_NS;

Anal_GyrationRadius::Anal_GyrationRadius(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Dimension") { dim = strs[1]; }
    else { error("Error(GR): Unknown string in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  Rg2_traj.resize(bstr->numitrs);
}

void Anal_GyrationRadius::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Molecule *> ms = extract(ingr->get_mols());
  // 現時刻のデータ
  std::vector<double> Rg2;
  // データ取得
  for (Molecule *m : ms) {
    Rg2.push_back(m->dprop["Rg"+dim+"^2"]);
  }
  // 現時刻のデータをトラジェクトリに追加
  Rg2_traj[ingr->get_index()] = Rg2;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "GyrationRadius calculated: "+s+".\n";
}

void Anal_GyrationRadius::analysis_trajectory() {
  // 全平均
  auto ave = [](double d, const std::vector<double> &v) {
    return d+std::accumulate(v.begin(),v.end(),0.0)/v.size(); };
  double sum = std::accumulate(Rg2_traj.begin(),Rg2_traj.end(),0.0,ave);
  double aveRg = sqrt(sum/Rg2_traj.size());
  // 二乗根
  std::vector<std::vector<double>> Rg_traj;
  for (std::vector<double> ds : Rg2_traj) {
    for (double &d : ds) { d = sqrt(d); }
    Rg_traj.push_back(ds);
  }
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/GyrationRadius-"+dim+".dat");
  // 出力
  ofs << "# Average : " << 0.1*aveRg << " [nm]" << std::endl;
  ofs << "# time index | Rg of each molecule [nm]" << std::endl;
  for (int i = 0; i < Rg_traj.size(); ++i) {
    ofs << i;
    for (double Rg : Rg_traj[i]) { ofs << " " << 0.1*Rg; }
    ofs << std::endl;
  }
  std::cout << "GyrationRadius served.\n";
}
