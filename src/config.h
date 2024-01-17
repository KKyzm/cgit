#ifndef _CGIT_CONFIG_H_
#define _CGIT_CONFIG_H_

#include <filesystem>
#include <optional>
#include <set>
#include <string>

namespace fs = std::filesystem;

struct Commit {
  std::string parent{};
  std::string tree{};
  std::string message{};
};

struct Ref {
  fs::path path{};
  std::string value{};
  bool is_ref{};
};

void cgit_init();

std::optional<fs::path> cgit_root();

const auto CGIT_DIR = std::string(".cgit");

using ustring = std::basic_string<unsigned char>;

bool is_ignored(const fs::path);

bool already_init();

#endif  // !_CGIT_CONFIG_H_
