/* -----------------------------------------------------------------------------
Program Name:   staff.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Property Calculation
----------------------------------------------------------------------------- */
#include "utility.h"
#include "chef.h"
#include "staff.h"
#include "prop_integer.h"
#include "prop_double.h"
#include "prop_InertiaMoment.h"
#include "prop_GyrationRadius.h"
#include "prop_Orientation.h"
#include "prop_SystemDrift.h"
// include header of your property
using namespace Bistro_NS;

Staff::Staff(Bistro *bstr) : Pointers(bstr) {
  // 解析に必要なプロパティを取得
  property_requests = chef->get_property_requests();
  // ディレクトリが指定されていればファイルからプロパティを読み込む
  if (!(bstr->propdir).empty()) {
    intgr = new Prop_Integer(bstr);
    dble = new Prop_Double(bstr);
  }
  // プロパティ計算用のインスタンス生成：他のプロパティに依存するプロパティは依存先のプロパティよりも「先」に書くこと
//  if (find_str(chef->property_requests,"YourProperty")) {
//     yourproperty = new YourProperty(bstr);
//  }
  if (find_str(property_requests,"Orientation")) {
     orientation = new Prop_Orientation(bstr);
     add_property_request(orientation);
  }
  if (find_str(property_requests,"GyrationRadius")) {
     gyrationR = new Prop_GyrationRadius(bstr);
     add_property_request(gyrationR);
  }
  if (find_str(property_requests,"InertiaMoment")) {
     inertiaM = new Prop_InertiaMoment(bstr);
     add_property_request(inertiaM);
  }
  if (find_str(property_requests,"SystemDrift")) {
     systemD = new Prop_SystemDrift(bstr);
     add_property_request(systemD);
  }
  std::cout << "List of Property to be Calculated:";
  for (std::string str : property_requests) { std::cout << " " << str; }
  std::cout << std::endl;
}

void Staff::precook(Ingredient *ingr) {
  if (!(bstr->propdir).empty()) {
    intgr->calculate_property(ingr);
    dble->calculate_property(ingr);
  }
  // プロパティ計算：他のプロパティに依存するプロパティは依存先のプロパティよりも「後」に書くこと
//  if (find_str(chef->property_requests,"YourProperty")) {
//    yourproperty->calculate_prop(ingr);
//  }
  if (find_str(property_requests,"SystemDrift")) {
    systemD->calculate_property(ingr);
  }
  if (find_str(property_requests,"InertiaMoment")) {
    inertiaM->calculate_property(ingr);
  }
  if (find_str(property_requests,"GyrationRadius")) {
    gyrationR->calculate_property(ingr);
  }
  if (find_str(property_requests,"Orientation")) {
    orientation->calculate_property(ingr);
  }
}

Staff::~Staff() { std::cout << "<Staff> Goodbye!\n"; }
