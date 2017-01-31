#include "utility.h"
#include "anal_NumberDensityDistribution.h"
using namespace Bistro_NS;

Anal_NumberDensityDistribution::Anal_NumberDensityDistribution(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Direction") { direc = strs[1]; }
    else if (strs[0] == "Number_of_Bins") { numbins = std::stoi(strs[1]); }
    else if (strs[0] == "Origin") { origin = std::stod(strs[1]); }
    else if (strs[0] == "Bin_Width") { width = std::stod(strs[1]); }
    else if (strs[0] == "Target") { target = strs[1]; }
    else { error("Error(NDD): Unknown String in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  NDD_traj.resize(bstr->numitrs);
}

void Anal_NumberDensityDistribution::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Particle *> ps;
  if (target == "atom") {
    for (Atom *a : extract(ingr->get_atoms())) { ps.push_back(a); } }
  else if (target == "bead") {
    for (Bead *b : extract(ingr->get_beads())) { ps.push_back(b); } }
  else if (target == "molecule") {
    for (Molecule *m : extract(ingr->get_mols())) { ps.push_back(m); } }
  else { error("Error(NDD): Unknown Target: "+target+"."); }
  // 現時刻のデータ（カウント用）
  std::vector<int> intNDD(numbins,0);
  // データ取得
  for (Particle *p : ps) {
    double pos = p->dprop[direc+"u"]-origin;
    if (pos < 0.0 || numbins*width < pos) { error("Error(NDD): Out of Bins."); }
    intNDD[(int)(pos/width)]++;
  }
  // 現時刻のデータ
  double invdV = sys->boxlen[direc]/(width*sys->boxlen["x"]*sys->boxlen["y"]*sys->boxlen["z"]);
  std::vector<double> NDD;
  for (int num : intNDD) { NDD.push_back(num*invdV); }
  // 現時刻のデータをトラジェクトリに追加
  NDD_traj[ingr->get_index()] = NDD;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "NumberDensityDistribution calculated: "+s+".\n";
}

void Anal_NumberDensityDistribution::analysis_trajectory() {
  // 時間平均
  std::vector<double> aveNDD(numbins,0.0);
  for (int i = 0; i < aveNDD.size(); ++i) {
    for (std::vector<double> NDD : NDD_traj) { aveNDD[i] += NDD[i]; }
    aveNDD[i] /= (double)bstr->numitrs;
  }
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/NumberDensityDistribution.dat");
  // 出力
  ofs << "# " << direc << " [nm] | Number dencity [/nm^3]" << std::endl;
  for (int i = 0; i < numbins; ++i) {
    ofs << std::defaultfloat << 0.1*(i+0.5)*width << " "
        << std::fixed << 1000.0*aveNDD[i] << std::endl;
  }
  std::cout << "NumberDensityDistribution served.\n";
}
