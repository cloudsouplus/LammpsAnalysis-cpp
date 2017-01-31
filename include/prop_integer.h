#ifndef PROP_INTEGER_H
#define PROP_INTEGER_H

#include "pointers.h"

namespace Bistro_NS {

class Prop_Integer : protected Pointers {
  std::vector<std::string> names;          // プロパティ名．整数プロパティからこの名前で参照できる．
  std::vector<std::string> targets;        // molecule or atom or bead
  std::vector<std::vector<int>> numbers;   // プロパティ数
  void read_file(const std::string &);
  // タグ付け関数
  template <class T> void add(std::vector<T *> ps, int itag) {
    if ((numbers[itag]).size() != ps.size()) {
      error("Error: Number of I-prop - "+names[itag]);
    }
    for (int i = 0; i < ps.size(); ++i) {
      ps[i]->iprop[names[itag]] = numbers[itag][i];
    }
  }
public:
  // Constructor & Destructor
  Prop_Integer(Bistro *);
  // Functions
  void calculate_property(class Ingredient *);
};

}

#endif
