/* -----------------------------------------------------------------------------
Program Name:   owner.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Call order
----------------------------------------------------------------------------- */
#ifndef OWNER_H
#define OWNER_H

#include <map>
#include <vector>
#include "pointers.h"

namespace Bistro_NS {

class Owner : protected Pointers {
  void read_order(char *);
  void call_order();
  void order_classic();
  void order_atom();
  void order_molecule();
  void order_bead();
  void order_dumpfile();
  void order_coarsegraining();
  int read_mappingfile(const std::string &, int);
  // Order File
  std::vector<std::string> names_order;
  std::map<std::string,std::vector<std::string>> orders;
  // Classic
  bool tof_C;
  int nummols, numatoms, numbeads;
  std::vector<std::vector<int>> molconfig;
  // Atom
  std::vector<double> atommasses;
  std::vector<std::string> names_atomproperty;
  std::vector<std::map<std::string,double>> atomproperties;
  // Molecule
  std::vector<std::string> names_molproperty;
  std::vector<std::map<std::string,double>> molproperties;
  // Bead
  std::vector<std::string> names_beadproperty;
  std::vector<std::map<std::string,double>> beadproperties;
  // DumpFile
  int numdumps, initdump, deltadump;
  std::string prefix_dump, suffix_dump;
  std::vector<std::string> dumpintegers;
  // CoarseGraining
  bool tof_CG;
  std::map<int,int> moltype_numbeads;
  std::map<int,std::vector<int>> cgconfig_type;
  std::map<int,std::vector<std::vector<int>>> cgconfig_members;
  std::map<int,std::vector<std::vector<double>>> cgconfig_weights;
public:
  // Friend Classes
  friend class Ingredient;
  // Constructor & Destructor
  Owner(char *, Bistro *);
  ~Owner();
  // Functions
  int get_numdumps() { return numdumps; }
  std::vector<std::string> get_names_order() { return names_order; }
  std::vector<std::string> get_order(const std::string &s) { return orders[s]; }
};

}

#endif
