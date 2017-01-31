/* -----------------------------------------------------------------------------
Program Name:   bistro.h
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Platform for Owner, Chef & Staff
----------------------------------------------------------------------------- */
#ifndef BISTRO_H
#define BISTRO_H

#include <chrono>
#include <iostream>

namespace Bistro_NS {

class Bistro {
  std::chrono::system_clock::time_point  start, end;
public:
  // Classes
  class Owner *owner;
  class Chef *chef;
  class Staff *staff;
  // Output Directory
  std::string outdir, propdir;
  // Loop over Dump files
  int numitrs;
  // Constructor & Destructor
  Bistro(int, char **);
  ~Bistro();
};

}

#endif
