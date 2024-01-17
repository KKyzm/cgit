#ifndef _CGIT_CONFIG_H_
#define _CGIT_CONFIG_H_

#include <filesystem>
#include <optional>
#include <string>

namespace fs = std::filesystem;

const auto CGIT_DIR = std::string(".cgit");

using ustring = std::basic_string<unsigned char>;

void cgit_init();

std::optional<fs::path> cgit_root();

bool is_ignored(const fs::path);

bool already_init();

#endif // !_CGIT_CONFIG_H_
