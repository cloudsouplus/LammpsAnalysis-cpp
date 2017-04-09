/* -----------------------------------------------------------------------------
Program Name:   chef.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Analyze
----------------------------------------------------------------------------- */
#include "utility.h"
#include "owner.h"
#include "chef.h"
#include "anal_AddTag.h"
#include "anal_PropertyRequest.h"
#include "anal_BeadDump.h"
#include "anal_GyrationRadius.h"
#include "anal_Orientation.h"
#include "anal_NumberDensityDistribution.h"
#include "anal_RadialDistributionFunction.h"
#include "anal_MeanSquareDisplacement.h"
#include "anal_Count.h"
// include header of your analysis
using namespace Bistro_NS;

Chef::Chef(Bistro *bstr) : Pointers(bstr) {
//  if (find_str(owner->get_name_orders(),"YourAnalysis")) {
//    youranalysis = new Anal_YourAnalysis(bstr,owner->get_order("YourAnalysis"));
//    add_property_request(youranalysis);
//  }
  if (find_str(owner->get_names_order(),"AddTag")) {
    std::vector<std::string> tmp;
    for (std::string line : owner->get_order("AddTag")) {
      if (line == "===") {
        addT.push_back(new Anal_AddTag(bstr,tmp));
        tmp.clear(); }
      else { tmp.push_back(line); }
    }
    addT.push_back(new Anal_AddTag(bstr,tmp));
  }
  if (find_str(owner->get_names_order(),"PropertyRequest")) {
    propertyR = new Anal_PropertyRequest(bstr,owner->get_order("PropertyRequest"));
    add_property_request(propertyR);
  }
  if (find_str(owner->get_names_order(),"BeadDump")) {
    beadD = new Anal_BeadDump(bstr,owner->get_order("BeadDump"));
    add_property_request(beadD);
  }
  if (find_str(owner->get_names_order(),"GyrationRadius")) {
    gyrationR = new Anal_GyrationRadius(bstr,owner->get_order("GyrationRadius"));
    add_property_request(gyrationR);
  }
  if (find_str(owner->get_names_order(),"Orientation")) {
    orientation = new Anal_Orientation(bstr,owner->get_order("Orientation"));
    add_property_request(orientation);
  }
  if (find_str(owner->get_names_order(),"NumberDensityDistribution")) {
    numberDD = new Anal_NumberDensityDistribution(bstr,owner->get_order("NumberDensityDistribution"));
    add_property_request(numberDD);
  }
  if (find_str(owner->get_names_order(),"RadialDistributionFunction")) {
    radialDF = new Anal_RadialDistributionFunction(bstr,owner->get_order("RadialDistributionFunction"));
    add_property_request(radialDF);
  }
  if (find_str(owner->get_names_order(),"MeanSquareDisplacement")) {
    meanSD = new Anal_MeanSquareDisplacement(bstr,owner->get_order("MeanSquareDisplacement"));
    add_property_request(meanSD);
  }
  if (find_str(owner->get_names_order(),"Count")) {
    count = new Anal_Count(bstr,owner->get_order("Count"));
    add_property_request(count);
  }
}

void Chef::cook(Ingredient *ingr) {
//  if (find_str(owner->get_names_order(),"YourAnalysis")) {
//    youranalysis->analysis_instant(ingr);
//  }
  if (find_str(owner->get_names_order(),"AddTag")) {
    for (Anal_AddTag *at : addT) { at->analysis_instant(ingr); }
  }
  if (find_str(owner->get_names_order(),"BeadDump")) {
    beadD->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"GyrationRadius")) {
    gyrationR->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"Orientation")) {
    orientation->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"NumberDensityDistribution")) {
    numberDD->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"RadialDistributionFunction")) {
    radialDF->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"MeanSquareDisplacement")) {
    meanSD->analysis_instant(ingr);
  }
  if (find_str(owner->get_names_order(),"Count")) {
    count->analysis_instant(ingr);
  }
}

void Chef::serve() {
//  if (find_str(owner->get_names_order(),"YourAnalysis")) {
//    youranalysis->analysis_trajectory();
//  }
  if (find_str(owner->get_names_order(),"GyrationRadius")) {
    gyrationR->analysis_trajectory();
  }
  if (find_str(owner->get_names_order(),"Orientation")) {
    orientation->analysis_trajectory();
  }
  if (find_str(owner->get_names_order(),"NumberDensityDistribution")) {
    numberDD->analysis_trajectory();
  }
  if (find_str(owner->get_names_order(),"RadialDistributionFunction")) {
    radialDF->analysis_trajectory();
  }
  if (find_str(owner->get_names_order(),"MeanSquareDisplacement")) {
    meanSD->analysis_trajectory();
  }
  if (find_str(owner->get_names_order(),"Count")) {
    count->analysis_trajectory();
  }
}

Chef::~Chef() { std::cout << "<Chef> Goodbye!\n"; }
