#include "fs.h"
#include "config.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

Pathes os_walk(const fs::path path) {
  auto pathes = Pathes{};
  for (const auto &entry : fs::directory_iterator(path)) {
    if (is_ignored(entry.path())) {
      continue;
    }
    if (entry.is_directory()) {
      pathes.dirs.insert(entry.path());
    } else if (entry.is_regular_file()) {
      pathes.files.insert(entry.path());
    }
  }
  return pathes;
}

void write_file(const fs::path &path, const std::string &content) {
  fs::create_directories(path.parent_path());
  auto ofs = std::ofstream(path, std::ios::binary);
  ofs.write(content.c_str(), content.length());
}

std::string read_file(const fs::path &path) {
  if (!fs::is_regular_file(path)) {
    throw std::runtime_error("Try to read unexist file.");
  }
  auto ifs = std::ifstream(path, std::ios::binary | std::ios::ate);
  auto pos = ifs.tellg();
  auto content = std::string(pos, '0');
  ifs.seekg(0, std::ios::beg);
  ifs.read(content.data(), pos);
  return content;
}

void clear_root_dir() {
  auto pathes = os_walk(cgit_root().value());
  for (auto &file : pathes.files) {
    fs::remove(file);
  }
  for (auto &dir : pathes.dirs) {
    fs::remove_all(dir);
  }
}
