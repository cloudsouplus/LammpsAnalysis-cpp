/* -----------------------------------------------------------------------------
Program Name:   pointers.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Pointers to main classes
----------------------------------------------------------------------------- */
#ifndef BSTR_POINTERS_H
#define BSTR_POINTERS_H

#include "bistro.h"

namespace Bistro_NS {

class Pointers {
public:
  Pointers(Bistro *ptr) : bstr(ptr), owner(ptr->owner), chef(ptr->chef), staff(ptr->staff) {}
  ~Pointers() {}
protected:
  Bistro *bstr;
  Owner *&owner;
  Chef *&chef;
  Staff *&staff;
};

}

#endif
