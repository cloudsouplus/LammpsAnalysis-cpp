#include <cstdlib>
#include <dirent.h>
#include "utility.h"
#include "ingredient.h"
#include "prop_double.h"
using namespace Bistro_NS;

Prop_Double::Prop_Double(Bistro *bstr) : Pointers(bstr) {
  // 参考：http://xr0038.hatenadiary.jp/entry/2015/02/26/154608
  const char *path = bstr->propdir.c_str(); // 検索ディレクトリ
  DIR *dp = opendir(path);                  // ディレクトリへのポインタ
  dirent *entry;                            // readdir() で返されるエントリーポイント
  // 検索開始
  if (dp == NULL) { exit(1); }
  do {
    entry = readdir(dp);
    if (entry != NULL) {
      std::string filename = entry->d_name;
      std::vector<std::string> strs = split(filename,'_');
      if (strs[0] == "double") {
        names.push_back(strs[1]);
        std::cout << "D-prop " << names.size() << " : " << strs[1] << std::endl;
        read_file(bstr->propdir+"/"+filename);
      }
    }
  } while (entry != NULL);
}

void Prop_Double::calculate_property(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得・プロパティ計算
  for (int i = 0; i < names.size(); ++i) {
    if (targets[i] == "atom") { add(ingr->get_atoms(),i); }
    else if (targets[i] == "molecule") { add(ingr->get_mols(),i); }
    else if (targets[i] == "bead") { add(ingr->get_beads(),i); }
    else { error("Error: Unknown Target of D-prop: "+names[i]+"."); }
  }
}

void Prop_Double::read_file(const std::string &filename) {
  std::ifstream ifs(filename);
  std::string line;
  std::getline(ifs,line);
  targets.push_back(line);
  values.resize(values.size()+1);
  while (std::getline(ifs,line)) { (values.back()).push_back(std::stod(line)); }
}
