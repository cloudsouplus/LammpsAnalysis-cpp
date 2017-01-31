#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace Bistro_NS {

// 文字列を分割
static std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (getline(ss,item,delim)) {
    if (!item.empty()) { elems.push_back(item); }
  }
  return elems;
}
// 文字列検索
static bool find_str(const std::vector<std::string> &strs,
                     const std::string &target) {
  return find(strs.begin(),strs.end(),target) != strs.end() ? true : false;
}
// 整数検索
static bool find_int(std::vector<int> is, int target) {
  return find(is.begin(),is.end(),target) != is.end() ? true : false;
}
// エラーメッセージ出力
static void error(std::string message) {
  std::cout << message << std::endl;
  exit(1);
}
// ファイルのコピー
static void copyfile(const std::string &src, const std::string &dst) {
    std::ifstream ifs(src);
    if (!ifs) { std::cout << "file open error: " << src << std::endl; }
    std::ofstream ofs(dst);
    if (!ofs) { std::cout << "file open error: " << dst << std::endl; }
    ofs << ifs.rdbuf() << std::flush;
    if (!ifs) { std::cout << "I/O error: " << src << std::endl; }
    if (!ofs) { std::cout << "I/O error: " << dst << std::endl; }
}

}

#endif
