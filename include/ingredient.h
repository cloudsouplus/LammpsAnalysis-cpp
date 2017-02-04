/* -----------------------------------------------------------------------------
Program Name:   ingredient.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Read dump file & Set basic property
----------------------------------------------------------------------------- */
#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <map>
#include <Eigen/Dense>
#include "pointers.h"

namespace Bistro_NS {

class Ingredient : protected Pointers {
  int index;
  int read_dumpheader(const std::string &);
  std::vector<int> read_dumpfile(const std::string &);
  void config_molecule(const std::vector<int> &);
  void config_bead();
  void calculate_atom();
  void calculate_molecule();
  void calculate_bead();
  // Data Classes
  class System *sys;
  class Atom *atom;
  class Bead *bead;
  class Molecule *mol;
public:
  // Constructor & Destructor
  Ingredient(Bistro *, int);
  ~Ingredient();
  // Functions
  int get_index() { return index; }
  System * get_system() { return sys; }   // データクラスへのポインタを取得
  std::vector<Atom *> get_atoms();        //
  std::vector<Bead *> get_beads();        //
  std::vector<Molecule *> get_mols();     //
  std::vector<Atom *> get_allatoms();     //
};
/* ---- 解析およびプロパティ計算は以下のデータクラスにアクセスして行う ----------------- */
// データの基本クラス
class Data {
public:
  std::map<std::string,double> dprop;           // property name -> value (double)
  std::map<std::string,int> iprop;              // property name -> value (integer)
};
// 計算系データ
class System : public Data {
public:
  int timestep, numatoms, numbeads, nummols;    // timestep, number of atoms/beads/molecules in a whole system
  std::map<std::string,std::string> periodic;   // periodicity in X, Y, Z direction
  Eigen::Vector3d boxmin, boxmax, boxlen;       // minimum & maximam vertices of simulation box and length of box side
  System(int i) { timestep = i; }
};
// 粒子データの基本クラス
class Particle : public Data {
public:
  Eigen::Vector3d pos, vel, wpos;               // position, velocity, wrapped position
  double mass;
};
// 原子データ
class Atom : public Particle {};
// 分子データ
class Molecule : public Particle {
public:
  std::vector<Atom *> memberatoms;              // atoms in molecule
  std::vector<Bead *> memberbeads;              // beads in molecule
};
// ビーズデータ
class Bead : public Particle {
public:
  std::vector<Atom *> members;                  // atoms in bead
  std::vector<double> weights;                  // weiths of atoms in bead
};

}

#endif
