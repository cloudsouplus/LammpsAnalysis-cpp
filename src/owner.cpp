/* -----------------------------------------------------------------------------
Program Name:   owner.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Call order
----------------------------------------------------------------------------- */
#include "utility.h"
#include "owner.h"
using namespace Bistro_NS;

Owner::Owner(char *filename, Bistro *bstr) : Pointers(bstr) {
  // Orderファイル読み込み
  std::cout << "Input file name is \"" << filename << "\".\n";
  read_order(filename);
  // Order設定
  call_order();
  // 出力ディレクトリにコピー
  copyfile(filename,bstr->outdir+"/"+split(filename,'/').back());
}

void Owner::read_order(char *filename) {
  std::ifstream ifs(filename);
  std::string line;
  while (std::getline(ifs,line)) {
    if (line[0] == '$') {
      names_order.push_back(split(line,' ')[1]);
      orders[names_order.back()]; }
    else if (line[0] == '#' || line[0] == '\0') { continue; }
    else { orders[names_order.back()].push_back(line); }
  }
}

void Owner::call_order() {
  order_atom();
  order_dumpfile();
  if (find_str(names_order,"CoarseGraining")) {
    tof_CG = true;
    order_coarsegraining(); }
  else { tof_CG = false; }
  if (find_str(names_order,"Classic")) {
    tof_C = true;
    order_classic(); }
  else { tof_C = false; tof_CG = false; }
  if (tof_C && find_str(names_order,"Molecule")) { order_molecule(); }
  if (tof_CG && find_str(names_order,"Bead")) { order_bead(); }
}

void Owner::order_classic() {
  std::vector<std::string> lines = orders["Classic"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"Classic"));
  std::vector<int> atmax;
  nummols = 0;
  numbeads = 0;
  for (std::string line : lines) {
    std::vector<int> is;
    for (std::string str : split(line,' ')) { is.push_back(std::stoi(str)); }
    if (is[0]) {
      nummols += is[1];
      for (int i = 0; i < is[1]; ++i) {
        if ((is[3]-is[2]+1)%is[1]) {
          error("Error: Non-Integer Number of Atoms in a Molecule."); }
        else {
          int num = (is[3]-is[2]+1)/is[1];
          molconfig.push_back({is[0],num,is[2]+i*num});
          if (tof_CG) {
            (molconfig.back()).push_back(moltype_numbeads[is[0]]);
            (molconfig.back()).push_back(numbeads+1);
            numbeads += moltype_numbeads[is[0]];
          }
        }
      }
    }
    atmax.push_back(is[3]);
  }
  numatoms = *std::max_element(atmax.begin(),atmax.end());
}

void Owner::order_atom() {
  std::vector<std::string> lines = orders["Atom"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"Atom"));
  for (std::string line : lines) {
    std::vector<std::string> strs = split(line,' ');
    std::string str0 = strs[0];
    strs.erase(strs.begin());
    if (str0 != "type") {
      atommasses.push_back(std::stod(strs[0]));
    }
    strs.erase(strs.begin());
    if (strs.empty()) { continue; }
    if (str0 == "type") { names_atomproperty = strs; }
    else {
      if (names_atomproperty.empty()) { error("Error: Atom Property Name."); }
      if (std::stoi(str0)-1 != atomproperties.size()) {
        error("Error: Atom Type Index.");
      }
      atomproperties.resize(atomproperties.size()+1);
      for (int i = 0; i < strs.size(); ++i) {
        (atomproperties.back())[names_atomproperty[i]] = std::stod(strs[i]);
      }
    }
  }
}

void Owner::order_molecule() {
  std::vector<std::string> lines = orders["Molecule"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"Molecule"));
  for (std::string line : lines) {
    std::vector<std::string> strs = split(line,' ');
    std::string str0 = strs[0];
    strs.erase(strs.begin());
    if (strs.empty()) { return; }
    if (str0 == "type") { names_molproperty = strs; }
    else {
      if (names_molproperty.empty()) { error("Error: Molecule Property Name."); }
      if (std::stoi(str0)-1 != molproperties.size()) {
        error("Error: Molecule Type Index.");
      }
      molproperties.resize(molproperties.size()+1);
      for (int i = 0; i < strs.size(); ++i) {
        (molproperties.back())[names_molproperty[i]] = std::stod(strs[i]);
      }
    }
  }
}

void Owner::order_bead() {
  std::vector<std::string> lines = orders["Bead"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"Bead"));
  for (std::string line : lines) {
    std::vector<std::string> strs = split(line,' ');
    std::string str0 = strs[0];
    strs.erase(strs.begin());
    if (strs.empty()) { return; }
    if (str0 == "type") { names_beadproperty = strs; }
    else {
      if (names_beadproperty.empty()) { error("Error: Bead Property Name."); }
      if (std::stoi(str0)-1 != beadproperties.size()) {
        error("Error: Bead Type Index.");
      }
      beadproperties.resize(beadproperties.size()+1);
      for (int i = 0; i < strs.size(); ++i) {
        (beadproperties.back())[names_beadproperty[i]] = std::stod(strs[i]);
      }
    }
  }
}

void Owner::order_dumpfile() {
  std::vector<std::string> lines = orders["DumpFile"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"DumpFile"));
  for (std::string line : lines) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "Number_of_Files") { numdumps = std::stoi(strs[1]); }
    else if (strs[0] == "Initial") { initdump = std::stoi(strs[1]); }
    else if (strs[0] == "Interval") { deltadump = std::stoi(strs[1]); }
    else if (strs[0] == "Prefix_of_FileName") { prefix_dump = strs[1]; }
    else if (strs[0] == "Suffix_of_FileName") { suffix_dump = strs[1]; }
    else if (strs[0] == "Integers") {
      strs.erase(strs.begin());
      dumpintegers = strs; }
    else { error("Error: Unknown String in DumpFile Section: "+strs[1]); }
  }
}

void Owner::order_coarsegraining() {
  std::vector<std::string> lines = orders["CoarseGraining"];
  names_order.erase(std::find(names_order.begin(),names_order.end(),"CoarseGraining"));
  for (std::string line : lines) {
    std::vector<std::string> strs = split(line,' ');
    moltype_numbeads[std::stoi(strs[0])] = read_mappingfile(strs[1],std::stoi(strs[0]));
  }
}

int Owner::read_mappingfile(const std::string &filename, int n) {
  std::ifstream ifs(filename);
  std::string line;
  std::streampos fpos;
  while (std::getline(ifs,line)) {
    std::vector<std::string> strs = split(line,' ');
    if (strs[0] == "ID" && strs[2] == "Type") {
      while (std::getline(ifs,line)) {
        std::vector<std::string> strs = split(line,' ');
        if (strs[0] == "ID") { ifs.seekg(fpos); break; }
        else if (std::stoi(strs[0]) != (cgconfig_type[n]).size()+1) {
          error("Error: Mapping Type Index.");
        }
        (cgconfig_type[n]).push_back(std::stoi(strs[1]));
        fpos = ifs.tellg();
      } }
    else if (strs[0] == "ID" && strs[2] == "Members") {
      while (std::getline(ifs,line)) {
        std::vector<std::string> strs = split(line,' ');
        if (strs[0] == "ID") { ifs.seekg(fpos); break; }
        else if (std::stoi(strs[0]) != (cgconfig_members[n]).size()+1) {
          error("Error: Mapping Members Index.");
        }
        strs.erase(strs.begin());
        (cgconfig_members[n]).resize((cgconfig_members[n]).size()+1);
        for (std::string str : strs) {
          ((cgconfig_members[n]).back()).push_back(std::stoi(str));
        }
        fpos = ifs.tellg();
      } }
    else if (strs[0] == "ID" && strs[2] == "Weights") {
      while (std::getline(ifs,line)) {
        std::vector<std::string> strs = split(line,' ');
        if (strs[0] == "ID") { ifs.seekg(fpos); break; }
        else if (std::stoi(strs[0]) != (cgconfig_weights[n]).size()+1) {
          error("Error: Mapping Weights Index.");
        }
        strs.erase(strs.begin());
        (cgconfig_weights[n]).resize((cgconfig_weights[n]).size()+1);
        for (std::string str : strs) {
          ((cgconfig_weights[n]).back()).push_back(std::stod(str));
        }
        fpos = ifs.tellg();
      } }
    else { error("Error: Mapping File."); }
  }
  if ((cgconfig_weights[n]).empty()) {
    for (std::vector<int> is : cgconfig_members[n]) {
      std::vector<double> tmp(is.size(),1.0);
      (cgconfig_weights[n]).push_back(tmp);
    }
  }
  int nbs = (cgconfig_type[n]).size();
  if (nbs != (cgconfig_members[n]).size() || nbs != (cgconfig_weights[n]).size()) {
    error("Error: Number of Beads in Mapping File.");
  }
  return nbs;
}

Owner::~Owner() { std::cout << "<Owner> Goodbye!\n"; }
