#include <sys/stat.h>
#include "utility.h"
#include "anal_WriteID.h"
using namespace Bistro_NS;

Anal_WriteID::Anal_WriteID(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else if (strs[0] == "Target") { target = strs[1]; }
    else { error("Error(WID): Unknown string in Order: "+strs[1]+"."); }
  }
  // 出力ディレクトリの作成
  odir = bstr->outdir+"/writeid";
  auto s = [](bool b) { return !b ? "" : "NOT " ; } (mkdir(odir.c_str(),0755));
  std::cout << "Output directory for WriteID was " << s << "created.\n";
  // 条件を記録
  conditions = order;
}

void Anal_WriteID::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Particle *> ps;
  if (target == "atom") {
    for (Atom *a : extract(ingr->get_atoms())) { ps.push_back(a); } }
  else if (target == "bead") {
    for (Bead *b : extract(ingr->get_beads())) { ps.push_back(b); } }
  else if (target == "molecule") {
    for (Molecule *m : extract(ingr->get_mols())) { ps.push_back(m); } }
  else { error("Error(WID): Unknown Target: "+target+"."); }
  // 出力ファイルの作成
  std::string timestep_s = std::to_string(static_cast<long long>(sys->timestep));
  std::ofstream ofs(odir+"/ids-"+timestep_s+".dat");
  // 書き込み
  for (std::string line : conditions) { ofs << "# "+line << std::endl; }
  ofs << "# ---" << std::endl;
  for (Particle *p : ps) { ofs << p->iprop["id"] << std::endl; }
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "WriteID calculated: "+s+".\n";
}
