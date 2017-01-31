#include "utility.h"
#include "ingredient.h"
#include "prop_SystemDrift.h"
using namespace Bistro_NS;

void Prop_SystemDrift::calculate_property(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  System *sys = ingr->get_system();
  std::vector<Atom *> as = ingr->get_allatoms();
  // プロパティ計算
  double summass = 0.0;
  Eigen::Vector3d summpos(0.0,0.0,0.0);
  for (Atom *a : as) {
    summass += a->mass;
    summpos += a->mass*a->pos;
  }
  sys->dprop["CoMx"] = summpos["x"]/summass;
  sys->dprop["CoMy"] = summpos["y"]/summass;
  sys->dprop["CoMz"] = summpos["z"]/summass;
}
