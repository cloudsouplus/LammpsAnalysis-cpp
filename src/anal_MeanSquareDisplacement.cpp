#include <cmath>
#include <omp.h>
#include "utility.h"
#include "anal_MeanSquareDisplacement.h"
using namespace Bistro_NS;

Anal_MeanSquareDisplacement::Anal_MeanSquareDisplacement(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Dimension") { dimension = strs[1]; }
    else if (strs[0] == "Target") { target = strs[1]; }
    else if (strs[0] == "Number_of_Samples") { numsamples = std::stoi(strs[1]); }
    else { error("Error(MSD): Unknown String in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  points_traj.resize(bstr->numitrs);
  ids_traj.resize(bstr->numitrs);
}

void Anal_MeanSquareDisplacement::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Particle *> ps;
  if (target == "atom") {
    for (Atom *a : extract(ingr->get_atoms())) { ps.push_back(a); } }
  else if (target == "bead") {
    for (Bead *b : extract(ingr->get_beads())) { ps.push_back(b); } }
  else if (target == "molecule") {
    for (Molecule *m : extract(ingr->get_mols())) { ps.push_back(m); } }
  else { error("Error(MSD): Unknown Target: "+target+"."); }
  // 現時刻のデータ
  std::vector<Eigen::Vector3d> points;
  std::vector<int> ids;
  // データ取得
  Eigen::Vector3d sysCoM(sys->dprop["CoMx"],sys->dprop["CoMy"],sys->dprop["CoMz"]);
  for (Particle *p : ps) {
    points.push_back(p->pos-sysCoM);
    ids.push_back(p->iprop["id"]);
  }
  // 現時刻のデータをトラジェクトリに追加
  points_traj[ingr->get_index()] = points;
  ids_traj[ingr->get_index()] = ids;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "MeanSquareDisplacement calculated: "+s+".\n";
}

void Anal_MeanSquareDisplacement::analysis_trajectory() {
  // IDチェック
  std::vector<int> ids0 = ids_traj[0];
  for (std::vector<int> ids : ids_traj) {
    if (ids.size() != ids0.size()) { error("Error(MSD): ID Size."); }
    for (int i = 0; i < ids0.size(); ++i) {
      if (ids[i] != ids0[i]) { error("Error(MSD): ID."); }
    }
  }
  // 粒子ごとの平均
  std::vector<std::vector<double>> MSDs(ids0.size());
  #pragma omp parallel for schedule(dynamic)
  for (int ip = 0; ip < ids0.size(); ++ip) {
    std::vector<double> tmpMSD(bstr->numitrs-numsamples+1,0.0);
    // 初期時刻のループ
    for (int t0 = 0; t0 < numsamples; ++t0) {
      Eigen::Vector3d rvec0 = points_traj[t0][ip];
      for (int i = 0; i < bstr->numitrs-numsamples+1; ++i) {
        Eigen::Vector3d dvec = points_traj[i+t0][ip]-rvec0;
        // 指定した次元の自乗変位を計算
        for (std::string dim : split(dimension,'+')) {
          tmpMSD[i] += dvec[dim]*dvec[dim];
        }
      }
    }
    for (double &el : tmpMSD) { el /= (double)numsamples; }
    MSDs[ip] = tmpMSD;
  }
  // 全粒子平均
  std::vector<double> aveMSD(bstr->numitrs-numsamples+1,0.0);
  for (std::vector<double> MSD : MSDs) {
    for (int i = 0; i < aveMSD.size(); ++i) { aveMSD[i] += MSD[i]; }
  }
  for (double &el : aveMSD) { el /= (double)MSDs.size(); }
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/MeanSquareDisplacement.dat");
  // 出力
  ofs << "# time index | aveMSD [nm^2] | MSDs [nm^2] : id =";
  for (int id : ids0) { ofs << " " << id; }
  ofs << std::endl;
  for (int i = 0; i < aveMSD.size(); ++i) {
    ofs << i << " " << 0.01*aveMSD[i];
    for (std::vector<double> MSD : MSDs) { ofs << " " << 0.01*MSD[i]; }
    ofs << std::endl;
  }
  std::cout << "MeanSquareDisplacement served.\n";
}
