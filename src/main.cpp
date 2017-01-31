/* -----------------------------------------------------------------------------
Program Name:   main.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Open Bistro: Simple Molecular-dynamics Analysis Program
----------------------------------------------------------------------------- */
#include "bistro.h"
using namespace Bistro_NS;

int main(int argc, char **argv) {
  Bistro *bstr = new Bistro(argc,argv);
  delete bstr;
}
