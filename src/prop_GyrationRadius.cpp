#include "utility.h"
#include "ingredient.h"
#include "prop_GyrationRadius.h"
using namespace Bistro_NS;

void Prop_GyrationRadius::calculate_property(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  std::vector<Molecule *> ms = ingr->get_mols();
  // プロパティ計算
  for (Molecule *m : ms) {
    double Ixyz = m->dprop["Ixx"]+m->dprop["Iyy"]+m->dprop["Izz"];
    m->dprop["Rgxyz^2"] = 0.5*Ixyz/m->mass;
    m->dprop["Rgx^2"] = (0.5*Ixyz-m->dprop["Ixx"])/m->mass;
    m->dprop["Rgyz^2"] = m->dprop["Ixx"]/m->mass;
    m->dprop["Rgy^2"] = (0.5*Ixyz-m->dprop["Iyy"])/m->mass;
    m->dprop["Rgzx^2"] = m->dprop["Iyy"]/m->mass;
    m->dprop["Rgz^2"] = (0.5*Ixyz-m->dprop["Izz"])/m->mass;
    m->dprop["Rgxy^2"] = m->dprop["Izz"]/m->mass;
  }
}
