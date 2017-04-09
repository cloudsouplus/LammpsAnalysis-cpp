#include "utility.h"
#include "anal_Count.h"
using namespace Bistro_NS;

Anal_Count::Anal_Count(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Target") { target = strs[1]; }
    else { error("Error(C): Unknown string in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  Count_traj.resize(bstr->numitrs);
}

void Anal_Count::analysis_instant(Ingredient *ingr) {
  System *sys = ingr->get_system();
  int Count;
  // カウント
  if (target == "atom") { Count = (extract(ingr->get_atoms())).size(); }
  else if (target == "bead") { Count = (extract(ingr->get_beads())).size(); }
  else if (target == "molecule") { Count = (extract(ingr->get_mols())).size(); }
  else { error("Error(AT): Unknown Target: "+target+"."); }
  // 現時刻のデータをトラジェクトリに追加
  Count_traj[ingr->get_index()] = Count;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "Count calculated: "+s+".\n";
}

void Anal_Count::analysis_trajectory() {
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/Count.dat");
  // 出力
  ofs << "# time index | # of Count" << std::endl;
  for (int i = 0; i < Count_traj.size(); ++i) {
    ofs << i << " " << Count_traj[i] << std::endl;
  }
  std::cout << "Count served.\n";
}
