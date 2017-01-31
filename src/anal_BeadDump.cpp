#include <sys/stat.h>
#include "utility.h"
#include "anal_BeadDump.h"
using namespace Bistro_NS;

Anal_BeadDump::Anal_BeadDump(Bistro *bstr,
  const std::vector<std::string> &order) : AnalysisBase(bstr) {
  // 設定の読み込み
  for (std::string line : order) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Range") { add_range(strs); }
    else if (strs[0] == "List") { add_list(strs); }
    else { error("Error(BD): Unknown string in Order: "+strs[1]+"."); }
  }
  // 出力ディレクトリの作成
  odir = bstr->outdir+"/beaddump";
  auto s = [](bool b) { return !b ? "" : "NOT " ; } (mkdir(odir.c_str(),0755));
  std::cout << "Output directory for BeadDump was " << s << "created.\n";
}

void Anal_BeadDump::analysis_instant(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Bead *> bs = extract(ingr->get_beads());
  // 出力ファイルの作成
  std::string timestep_s = std::to_string(static_cast<long long>(sys->timestep));
  std::ofstream ofs(odir+"/bead."+timestep_s+".dump");
  // 書き込み
  ofs << "ITEM: TIMESTEP" << std::endl;
  ofs << sys->timestep << std::endl;
  ofs << "ITEM: NUMBER OF ATOMS" << std::endl;
  ofs << bs.size() << std::endl;
  ofs << "ITEM: BOX BOUNDS";
  for (std::string s : {"x","y","z"}) { ofs << " " << sys->periodic[s]; }
  ofs << std::endl;
  ofs << (sys->boxmin)["x"] << " " << (sys->boxmax)["x"] << std::endl;
  ofs << (sys->boxmin)["y"] << " " << (sys->boxmax)["y"] << std::endl;
  ofs << (sys->boxmin)["z"] << " " << (sys->boxmax)["z"] << std::endl;
  ofs << "ITEM: ATOMS id mol type xu yu zu vx vy vz" << std::endl;
  for (Bead *b : bs) {
    ofs << b->iprop["id"] << " " << b->iprop["mol"] << " " << b->iprop["type"];
    for (std::string s : {"xu","yu","zu","vx","vy","vz"}) { ofs << " " << b->dprop[s]; }
    ofs << std::endl;
  }
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "BeadDump calculated: "+s+".\n";
}
