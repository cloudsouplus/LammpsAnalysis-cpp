/* -----------------------------------------------------------------------------
Program Name:   bistro.cpp
Author:         Takayuki Kobayashi
Date:           Dec 31, 2016
Modified Date:  --- by ***
Purpose:        Platform for Owner, Chef & Staff
----------------------------------------------------------------------------- */
#include <cstring>
#include <omp.h>
#include <sys/stat.h>
#include "bistro.h"
#include "owner.h"
#include "chef.h"
#include "staff.h"
#include "ingredient.h"
using namespace Bistro_NS;
using namespace std::chrono;

Bistro::Bistro(int narg, char **arg) {
  // 実行時間計測開始
  start = system_clock::now();
  // 出力用ディレクトリの設定・作成
  for (int i = 0; i < narg; ++i) {
    if (!std::strcmp(arg[i],"-out")) {
      outdir = arg[i+1];
      auto str = [](bool b) { return !b ? "" : "NOT "; } (mkdir(arg[i+1],0755));
      std::cout << "Output directory \""+outdir+"\" was " << str << "created.\n";
    }
  }
  // プロパティディレクトリの取得
  for (int i = 0; i < narg; ++i) {
    if (!std::strcmp(arg[i],"-prop")) { propdir = arg[i+1]; }
  }
  // Owner
  for(int i = 0; i < narg; ++i) {
    if (!std::strcmp(arg[i],"-in")) { owner = new Owner(arg[i+1],this); }
  }
  numitrs = owner->get_numdumps();
  // Chef
  chef = new Chef(this);
  // Staff
  staff = new Staff(this);
  // Dumpを並列処理: データ読み込み，プロパティ計算，時点解析
  #pragma omp parallel for schedule(dynamic)
  for (int itr = 0; itr < numitrs; ++itr) {
      class Ingredient *ingr = new Ingredient(this,itr);
      staff->precook(ingr);
      chef->cook(ingr);
      delete ingr;
  }
  // トラジェクトリの解析・出力
  chef->serve();
}

Bistro::~Bistro() {
  delete staff;
  delete chef;
  delete owner;
  // 実行時間計測終了
  end = system_clock::now();
  // 実行時間出力
  double elapsed = duration_cast<milliseconds>(end-start).count();
  std::cout << "Runtime: " << elapsed/(double)1000 << " [s]\n";
}
