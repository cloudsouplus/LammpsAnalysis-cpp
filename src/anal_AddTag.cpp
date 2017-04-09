#include <sys/stat.h>
#include "utility.h"
#include "anal_AddTag.h"
using namespace Bistro_NS;

Anal_AddTag::Anal_AddTag(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Target") { target = strs[1]; }
    else if (strs[0] == "Name") { tagname = strs[1]; }
    else if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else { error("Error(AT): Unknown string in Order: "+strs[1]+"."); }
  }
}

void Anal_AddTag::analysis_instant(Ingredient *ingr) {
  System *sys = ingr->get_system();
  // タグづけ
  if (target == "atom") {
    for (Atom *a : ingr->get_atoms()) { a->iprop[tagname] = 0; }
    for (Atom *a : extract(ingr->get_atoms())) { a->iprop[tagname] = 1; } }
  else if (target == "bead") {
    for (Bead *b : ingr->get_beads()) { b->iprop[tagname] = 0; }
    for (Bead *b : extract(ingr->get_beads())) { b->iprop[tagname] = 1; } }
  else if (target == "molecule") {
    for (Molecule *m : ingr->get_mols()) { m->iprop[tagname] = 0; }
    for (Molecule *m : extract(ingr->get_mols())) { m->iprop[tagname] = 1; } }
  else { error("Error(AT): Unknown Target: "+target+"."); }
  //
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "AddTag calculated: "+s+".\n";
}
