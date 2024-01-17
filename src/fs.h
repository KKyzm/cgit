#ifndef _CGIT_FS_H_
#define _CGIT_FS_H_

#include <filesystem>
#include <set>
#include <string>

namespace fs = std::filesystem;

struct Pathes {
  std::set<fs::path> dirs{};
  std::set<fs::path> files{};
  void merge(Pathes &&other) {
    this->dirs.insert(other.dirs.begin(), other.dirs.end());
    this->files.insert(other.files.begin(), other.files.end());
  }
};

Pathes os_walk(const fs::path);

void write_file(const fs::path &, const std::string &);

std::string read_file(const fs::path &);

void clear_root_dir();

#endif  // !_CGIT_FS_H_
