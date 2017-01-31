#include "utility.h"
#include "ingredient.h"
#include "prop_InertiaMoment.h"
using namespace Bistro_NS;

void Prop_InertiaMoment::calculate_property(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  std::vector<Molecule *> ms = ingr->get_mols();
  // プロパティ計算
  for (Molecule *m : ms) {
    double sumxx = 0.0;
    double sumyy = 0.0;
    double sumzz = 0.0;
    double sumxy = 0.0;
    double sumyz = 0.0;
    double sumzx = 0.0;
    for (Atom *a : m->memberatoms) {
      double rx = a->pos["x"]-m->pos["x"];
      double ry = a->pos["y"]-m->pos["y"];
      double rz = a->pos["z"]-m->pos["z"];
      sumxx += (ry*ry+rz*rz)*a->mass;
      sumyy += (rz*rz+rx*rx)*a->mass;
      sumzz += (rx*rx+ry*ry)*a->mass;
      sumxy += -rx*ry*a->mass;
      sumyz += -ry*rz*a->mass;
      sumzx += -rz*rx*a->mass;
    }
    m->dprop["Ixx"] = sumxx;
    m->dprop["Iyy"] = sumyy;
    m->dprop["Izz"] = sumzz;
    m->dprop["Ixy"] = sumxy;
    m->dprop["Iyz"] = sumyz;
    m->dprop["Izx"] = sumzx;
  }
}
