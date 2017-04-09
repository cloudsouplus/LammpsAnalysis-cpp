/* -----------------------------------------------------------------------------
Program Name:   ingredient.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Read dump file & Set basic property
----------------------------------------------------------------------------- */
#include <cmath>
#include <iomanip>
#include "utility.h"
#include "owner.h"
#include "ingredient.h"
using namespace Bistro_NS;

Ingredient::Ingredient(Bistro *bstr, int itr) : Pointers(bstr) {
  index = itr;
  // 計算系データを生成・設定
  sys = new System((owner->initdump)+index*(owner->deltadump));
  if (owner->tof_C) { sys->numatoms = owner->numatoms; }
  if (owner->tof_C) { sys->nummols = owner->nummols; }
  if (owner->tof_CG) { sys->numbeads = owner->numbeads; }
  // 原子データ生成・読み込み
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  atom = new Atom[read_dumpheader(owner->prefix_dump+s+owner->suffix_dump)];
  std::vector<int> molids = read_dumpfile(owner->prefix_dump+s+owner->suffix_dump);
  // 分子・ビーズデータ生成・設定
  if (owner->tof_C) { mol = new Molecule[sys->nummols]; }
  else if (!molids.empty()) { mol = new Molecule[molids.size()]; }
  if (owner->tof_CG) { bead = new Bead[sys->numbeads]; }
  if (owner->tof_C || !molids.empty()) { config_molecule(molids); }
  if (owner->tof_CG) { config_bead(); }
  // 基本プロパティの計算
  calculate_atom();
  if (owner->tof_C || !molids.empty()) { calculate_molecule(); }
  if (owner->tof_CG) { calculate_bead(); }
}

int Ingredient::read_dumpheader(const std::string &filename) {
  std::ifstream ifs(filename);
  std::string line;
  for (int item = 0; item < 3; ++item) {
    std::getline(ifs,line);
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "ITEM:") {
      if (strs[1] == "TIMESTEP") {
        std::getline(ifs,line);
        if (sys->timestep != std::stoi(line)) { error("Error: Timestep."); } }
      else if (strs[1] == "NUMBER") {
        std::getline(ifs,line);
        if (owner->tof_C) {
          if (sys->numatoms != std::stoi(line)) { error("Error: Number."); } }
        else { sys->numatoms = std::stoi(line); } }
      else if (strs[1] == "BOX") {
        sys->periodic["x"] = strs[3];
        sys->periodic["y"] = strs[4];
        sys->periodic["z"] = strs[5];
        for (int i = 0; i < 3; i++) {
          std::getline(ifs,line);
          sys->boxmin[i] = std::stod(split(line,' ')[0]);
          sys->boxmax[i] = std::stod(split(line,' ')[1]);
          sys->boxlen[i] = abs(sys->boxmax[i]-sys->boxmin[i]);
        }
      }
    }
  }
  return sys->numatoms;
}

std::vector<int> Ingredient::read_dumpfile(const std::string &filename) {
  std::vector<std::string> pkey;  // プロパティキー
  std::vector<bool> int_tof;
  std::ifstream ifs(filename);
  std::string line;
  while (std::getline(ifs,line)) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "ITEM:" && strs[1] == "ATOMS") {
      for (auto str = strs.begin()+2; str != strs.end(); ++str) {
        pkey.push_back(*str);
        if (find_str(owner->dumpintegers,*str)) { int_tof.push_back(true); }
        else { int_tof.push_back(false); }
      }
      break;
    }
  } // End of Header
  bool mol_tof = find_str(pkey,"mol");
  std::vector<int> molids; // 分子ID配列（重複無し）
  int iid = std::distance(pkey.begin(),std::find(pkey.begin(),pkey.end(),"id"));
  while (std::getline(ifs,line)) {
    std::vector<std::string> strs = split(line,' ');
    int index = std::stoi(strs[iid])-1;
    for (int i = 0; i < strs.size(); ++i) {
      if (int_tof[i]) { atom[index].iprop[pkey[i]] = std::stoi(strs[i]); }
      else { atom[index].dprop[pkey[i]] = std::stod(strs[i]); }
    }
    if (mol_tof) {
      if (!find_int(molids,atom[index].iprop["mol"])) {
        molids.push_back(atom[index].iprop["mol"]);
      }
    }
  }
  if (!molids.empty()) { std::sort(molids.begin(),molids.end()); }
  return molids;
}

void Ingredient::config_molecule(const std::vector<int> &molids) {
  if (molids.empty()) {
    for (int im = 0; im < sys->nummols; ++im) {
      std::vector<int> config = owner->molconfig[im];
      mol[im].iprop["id"] = im+1;
      mol[im].iprop["type"] = config[0];
      for (int ia = config[2]-1; ia < config[1]+config[2]-1; ++ia){
        mol[im].memberatoms.push_back(&atom[ia]);
        atom[ia].iprop["mol"] = im+1;
        atom[ia].parentmol = &mol[im];
      }
      if (owner->tof_CG) {
        for (int ib = config[4]-1; ib < config[3]+config[4]-1; ++ib){
          mol[im].memberbeads.push_back(&bead[ib]);
          bead[ib].iprop["mol"] = im+1;
          bead[ib].parentmol = &mol[im];
        }
      }
    } }
  else {
    sys->nummols = molids.size();
    for (int im = 0; im < sys->nummols; ++im) {
      mol[im].iprop["id"] = molids[im];
      for (int ia = 0; ia < sys->numatoms; ++ia){
        if (atom[ia].iprop["mol"] == molids[im]) {
          mol[im].memberatoms.push_back(&atom[ia]);
          atom[ia].parentmol = &mol[im];
        }
      }
    }
  }
}

void Ingredient::config_bead() {
  int ib = 0;
  for (int im = 0; im < sys->nummols; ++im) {
    for (int rib = 0; rib < mol[im].memberbeads.size(); ++rib) {
      bead[ib].iprop["id"] = ib+1;
      bead[ib].iprop["type"] = (owner->cgconfig_type[mol[im].iprop["type"]])[rib];
      bead[ib].weights = (owner->cgconfig_weights[mol[im].iprop["type"]])[rib];
      for (int ria : (owner->cgconfig_members[mol[im].iprop["type"]])[rib]) {
        int ia = ria+mol[im].memberatoms[0]->iprop["id"]-2;
        bead[ib].members.push_back(&atom[ia]);
        if (atom[ia].iprop.count("bead")) {
          atom[ia].iprop["bead1"] = atom[ia].iprop["bead"];
          atom[ia].iprop["bead2"] = ib+1;
          atom[ia].iprop.erase("bead"); }
        else if (atom[ia].iprop.count("bead1")) {
          error("Error: Atom devided into too many beads."); }
        else { atom[ia].iprop["bead"] = ib+1; }
      }
      ib++;
    }
  }
  if (sys->numbeads != ib) { error("Error: Number of Beads."); }
}

void Ingredient::calculate_atom() {
  for (int i = 0; i < sys->numatoms; ++i) {
    if (!(atom[i].iprop["mol"]) && owner->tof_C) { continue; } // タイプゼロの分子は飛ばす
    atom[i].pos << atom[i].dprop["xu"],atom[i].dprop["yu"],atom[i].dprop["zu"];
    atom[i].vel << atom[i].dprop["vx"],atom[i].dprop["vy"],atom[i].dprop["vz"];
    atom[i].mass = owner->atommasses[atom[i].iprop["type"]-1];
    //
    for (std::string dim : {"x","y","z"}) {
      if (sys->periodic[dim] == "pp") {
        double shift = floor((atom[i].pos[dim]-sys->boxmin[dim])/sys->boxlen[dim]);
        atom[i].wpos[dim] = atom[i].pos[dim]-shift*sys->boxlen[dim]; }
      else { atom[i].wpos[dim] = atom[i].pos[dim]; }
      atom[i].dprop["w"+dim] = atom[i].wpos[dim];
    }
    atom[i].dprop["mass"] = atom[i].mass;
    //
    for (std::string name : owner->names_atomproperty) {
      atom[i].dprop[name] = (owner->atomproperties[atom[i].iprop["type"]-1])[name];
    }
  }
}

void Ingredient::calculate_molecule() {
  for (int i = 0; i < sys->nummols; ++i) {
    double summass = 0.0;
    mol[i].pos << 0.0,0.0,0.0;
    mol[i].vel << 0.0,0.0,0.0;
    for (Atom *a : mol[i].memberatoms) {
      summass += a->mass;
      mol[i].pos += a->pos*a->mass;
      mol[i].vel += a->vel*a->mass;
    }
    mol[i].mass = summass;
    mol[i].pos /= summass;
    mol[i].vel /= summass;
    //
    for (std::string dim : {"x","y","z"}) {
      if (sys->periodic[dim] == "pp") {
        double shift = floor((mol[i].pos[dim]-sys->boxmin[dim])/sys->boxlen[dim]);
        mol[i].wpos[dim] = mol[i].pos[dim]-shift*sys->boxlen[dim]; }
      else { mol[i].wpos[dim] = mol[i].pos[dim]; }
      mol[i].dprop[dim+"u"] = mol[i].pos[dim];
      mol[i].dprop["v"+dim] = mol[i].vel[dim];
      mol[i].dprop["w"+dim] = mol[i].wpos[dim];
    }
    mol[i].dprop["mass"] = mol[i].mass;
    //
    for (std::string name : owner->names_molproperty) {
      mol[i].dprop[name] = (owner->molproperties[mol[i].iprop["type"]-1])[name];
    }
  }
}

void Ingredient::calculate_bead() {
  for (int i = 0; i < sys->numbeads; ++i) {
    double summass = 0.0;
    bead[i].pos << 0.0,0.0,0.0;
    bead[i].vel << 0.0,0.0,0.0;
    for (int ii = 0; ii < bead[i].members.size(); ++ii) {
      Atom *a = bead[i].members[ii];
      summass += a->mass*bead[i].weights[ii];
      bead[i].pos += a->pos*(a->mass*bead[i].weights[ii]);
      bead[i].vel += a->vel*(a->mass*bead[i].weights[ii]);
    }
    bead[i].mass = summass;
    bead[i].pos /= summass;
    bead[i].vel /= summass;
    //
    for (std::string dim : {"x","y","z"}) {
      if (sys->periodic[dim] == "pp") {
        double shift = floor((bead[i].pos[dim]-sys->boxmin[dim])/sys->boxlen[dim]);
        bead[i].wpos[dim] = bead[i].pos[dim]-shift*sys->boxlen[dim]; }
      else { bead[i].wpos[dim] = bead[i].pos[dim]; }
      bead[i].dprop[dim+"u"] = bead[i].pos[dim];
      bead[i].dprop["v"+dim] = bead[i].vel[dim];
      bead[i].dprop["w"+dim] = bead[i].wpos[dim];
    }
    bead[i].dprop["mass"] = bead[i].mass;
    //
    for (std::string name : owner->names_beadproperty) {
      bead[i].dprop[name] = (owner->beadproperties[bead[i].iprop["type"]-1])[name];
    }
  }
}

std::vector<Atom *> Ingredient::get_atoms() {
  std::vector<Atom *> atoms;
  for (int i = 0; i < sys->numatoms; ++i) {
    if (atom[i].iprop["mol"] || !(owner->tof_C)) { atoms.push_back(&atom[i]); }
  }
  return atoms;
}

std::vector<Bead *> Ingredient::get_beads() {
  std::vector<Bead *> beads;
  for (int i = 0; i < sys->numbeads; ++i) { beads.push_back(&bead[i]); }
  return beads;
}

std::vector<Molecule *> Ingredient::get_mols() {
  std::vector<Molecule *> mols;
  for (int i = 0; i < sys->nummols; ++i) { mols.push_back(&mol[i]); }
  return mols;
}

std::vector<Atom *> Ingredient::get_allatoms() {
  std::vector<Atom *> atoms;
  for (int i = 0; i < sys->numatoms; ++i) { atoms.push_back(&atom[i]); }
  return atoms;
}

Ingredient::~Ingredient() {
  std::string s = std::to_string(static_cast<long long>(sys->timestep));
  std::cout << "Ingredient Destruction: "+s+".\n";
}
