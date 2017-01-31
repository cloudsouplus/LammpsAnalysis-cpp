#ifndef PROP_DOUBLE_H
#define PROP_DOUBLE_H

#include "pointers.h"

namespace Bistro_NS {

class Prop_Double : protected Pointers {
  std::vector<std::string> names;             // プロパティ名．実数プロパティからこの名前で参照できる．
  std::vector<std::string> targets;           // molecule or atom or bead
  std::vector<std::vector<double>> values;    // プロパティ値
  void read_file(const std::string &);
  // タグ付け関数
  template <class T> void add(std::vector<T *> ps, int itag) {
    if ((values[itag]).size() != ps.size()) {
      error("Error: Number of D-prop - "+names[itag]);
    }
    for (int i = 0; i < ps.size(); ++i) {
      ps[i]->dprop[names[itag]] = values[itag][i];
    }
  }
public:
  // Constructor & Destructor
  Prop_Double(Bistro *);
  // Functions
  void calculate_property(class Ingredient *);
};

}

#endif
