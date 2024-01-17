#include "config.h"

#include "data.h"
#include "fs.h"

void cgit_init() {
  fs::create_directories(CGIT_DIR);
  fs::create_directories(fs::path(CGIT_DIR) / "objects");
  fs::create_directories(fs::path(CGIT_DIR) / "refs/tags");
  fs::create_directories(fs::path(CGIT_DIR) / "refs/heads");

  write_file(fs::path(CGIT_DIR) / "refs/heads/master", "");
  write_file(fs::path(CGIT_DIR) / "HEAD", "ref refs/heads/master");
}

std::optional<fs::path> cgit_root() {
  auto dir = fs::current_path();
  while (dir != "/") {
    if (fs::is_directory(dir / CGIT_DIR)) {
      return dir;
    }
    dir = dir.parent_path();
  }
  return {};
}

bool already_init() { return cgit_root().has_value(); }

bool is_ignored(const fs::path path) { return path.filename() == CGIT_DIR; }
