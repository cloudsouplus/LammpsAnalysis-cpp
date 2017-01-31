#define _USE_MATH_DEFINES
#include <cmath>
#include <numeric>
#include <omp.h>
#include "utility.h"
#include "anal_RadialDistributionFunction.h"
using namespace Bistro_NS;

Anal_RadialDistributionFunction::Anal_RadialDistributionFunction(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Number_of_Bins") { numbins = std::stoi(strs[1]); }
    else if (strs[0] == "Bin_Width") { width = std::stod(strs[1]); }
    else if (strs[0] == "Target") { target = strs[1]; }
    else { error("Error(RDF): Unknown String in Order: "+strs[0]+"."); }
  }
  // トラジェクトリを入力ファイル数まで拡張
  RDF_traj.resize(bstr->numitrs);
}

void Anal_RadialDistributionFunction::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Particle *> ps;
  if (target == "atom") {
    for (Atom *a : extract(ingr->get_atoms())) { ps.push_back(a); } }
  else if (target == "bead") {
    for (Bead *b : extract(ingr->get_beads())) { ps.push_back(b); } }
  else if (target == "molecule") {
    for (Molecule *m : extract(ingr->get_mols())) { ps.push_back(m); } }
  else { error("Error(RDF): Unknown Target: "+target+"."); }
  // 現時刻のデータ（カウント用）
  std::vector<int> intRDF(numbins,0);
  // 粒子の組み合わせを走査
  for (int i = 0; i < ps.size(); ++i) {
    for (int j = i+1; j < ps.size(); ++j) {
      // イメージセルの中から最も近い粒子との相対位置ベクトルを取得
      Eigen::Vector3d rvec = ps[j]->wpos-ps[i]->wpos;
      for (std::string dim : {"x","y","z"}) {
        if (sys->periodic[dim] == "pp") {
          // 二乗が最小の要素を返すラムダ式
          auto min = [](const std::vector<double> &ds) {
            std::vector<double> d2s(ds.size());
            std::transform(ds.begin(),ds.end(),d2s.begin(),[](double d) { return d*d; } );
            return ds[std::distance(d2s.begin(),std::min_element(d2s.begin(),d2s.end()))]; };
          rvec[dim] = min({rvec[dim]-sys->boxlen[dim],rvec[dim],rvec[dim]+sys->boxlen[dim]});
        }
      }
      // 距離計算とカウント
      double r = rvec.norm();
      if (r < numbins*width) { intRDF[(int)(r/width)] += 2; }
    }
  }
  // 現時刻のデータ
  std::vector<double> RDF;
  double dens = ps.size()/(sys->boxlen["x"]*sys->boxlen["y"]*sys->boxlen["z"]);
  for (int i = 0; i < numbins; ++i) {
    double dV = (4.0*M_PI/3.0)*(3.0*i*i+3.0*i+1.0)*width*width*width;
    RDF.push_back(intRDF[i]/(ps.size()*dV*dens));
  }
  // 現時刻のデータをトラジェクトリに追加
  RDF_traj[ingr->get_index()] = RDF;
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "RadialDistributionFunction calculated: "+s+".\n";
}

void Anal_RadialDistributionFunction::analysis_trajectory() {
  // 時間平均
  std::vector<double> aveRDF(numbins,0.0);
  for (int i = 0; i < aveRDF.size(); ++i) {
    for (std::vector<double> RDF : RDF_traj) { aveRDF[i] += RDF[i]; }
    aveRDF[i] /= (double)bstr->numitrs;
  }
  // 出力ファイルの作成
  std::ofstream ofs(bstr->outdir+"/RadialDistributionFunction.dat");
  // 出力
  ofs << "# r [nm] | RDF" << std::endl;
  for (int i = 0; i < numbins; ++i) {
    ofs << std::defaultfloat << 0.1*(i+0.5)*width << " "
        << std::fixed << aveRDF[i] << std::endl;
  }
  std::cout << "RadialDistributionFunction served.\n";
}
