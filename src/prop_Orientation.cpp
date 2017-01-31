#include <cmath>
#include "utility.h"
#include "ingredient.h"
#include "prop_Orientation.h"
using namespace Bistro_NS;
using Eigen::MatrixXd;

void Prop_Orientation::calculate_property(Ingredient *ingr) {
  // Ingredientから必要なポインタを取得
  std::vector<Molecule *> ms = ingr->get_mols();
  // プロパティ計算
  for (Molecule *m : ms) {
    MatrixXd I(3,3);
    I << m->dprop["Ixx"],m->dprop["Ixy"],m->dprop["Izx"],
         m->dprop["Ixy"],m->dprop["Iyy"],m->dprop["Iyz"],
         m->dprop["Izx"],m->dprop["Iyz"],m->dprop["Izz"];
    Eigen::EigenSolver<MatrixXd> es(I);
    std::vector<double> evs;
    for (int i = 0; i < 3; ++i) { evs.push_back(es.eigenvalues()[i].real()); }
    // 固有値が最大となる主軸
    int imax = std::distance(evs.begin(),std::max_element(evs.begin(),evs.end()));
    Eigen::VectorXcd vmax = es.eigenvectors().col(imax);
    std::vector<double> cos2max;
    for (int i = 0; i < 3; ++i) { cos2max.push_back(pow(vmax[i].real(),2.0)); }
    m->dprop["Sx_max"] = 0.5*(3.0*cos2max[0]-1.0);
    m->dprop["Sy_max"] = 0.5*(3.0*cos2max[1]-1.0);
    m->dprop["Sz_max"] = 0.5*(3.0*cos2max[2]-1.0);
    // 固有値が最小となる主軸（一般的な配向度パラメータ）
    int imin = std::distance(evs.begin(),std::min_element(evs.begin(),evs.end()));
    Eigen::VectorXcd vmin = es.eigenvectors().col(imin);
    std::vector<double> cos2min;
    for (int i = 0; i < 3; ++i) { cos2min.push_back(pow(vmin[i].real(),2.0)); }
    m->dprop["Sx_min"] = 0.5*(3.0*cos2min[0]-1.0);
    m->dprop["Sy_min"] = 0.5*(3.0*cos2min[1]-1.0);
    m->dprop["Sz_min"] = 0.5*(3.0*cos2min[2]-1.0);
  }
}
