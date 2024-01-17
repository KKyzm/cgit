#include "data.h"

#include <assert.h>
#include <cryptopp/sha.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "config.h"
#include "fs.h"

std::string get_object(const std::string oid, const std::string type) {
  auto obj = read_file(cgit_root().value() / CGIT_DIR / "objects" / oid);
  auto obj_type = obj.substr(0, obj.find('\n'));
  if (obj_type != type) {
    throw std::runtime_error(fmt::format("Expecte type: {0}, but get {1}", type, obj_type));
  }
  obj.erase(0, obj.find('\n') + 1);

  return obj;
}

// SHA1 algorithm
std::string hash_object(std::string &data) {
  CryptoPP::byte digest[CryptoPP::SHA1::DIGESTSIZE + 1] = {0};
  CryptoPP::SHA1().CalculateDigest(digest, reinterpret_cast<CryptoPP::byte *>(data.data()), data.length());
  auto dstr = ustring(digest, CryptoPP::SHA1::DIGESTSIZE);
  auto oid = to_hex(dstr);
  return oid;
}

std::string hash_file(const std::string &file_path) {
  auto content = read_file(file_path);
  content = "blob\n" + content;  // object type tag
  auto oid = hash_object(content);
  write_file(cgit_root().value() / CGIT_DIR / "objects" / oid, content);
  return oid;
}

std::string hash_tree(const std::string &root_path) {
  auto ss = std::stringstream{};
  ss << "tree\n";  // object type tag

  auto pathes = os_walk(root_path);
  for (auto &file : pathes.files) {
    ss << "blob " << hash_file(file) << " " << fs::relative(file, cgit_root().value()).string() << "\n";
  }
  for (auto &dir : pathes.dirs) {
    ss << "tree " << hash_tree(dir) << " " << fs::relative(dir, cgit_root().value()).string() << "\n";
  }
  auto content = ss.str();
  auto oid = hash_object(content);

  write_file(cgit_root().value() / CGIT_DIR / "objects" / oid, content);

  return oid;
}

std::set<std::pair<std::string, fs::path>> get_tree(const std::string &ref) {
  auto res = std::set<std::pair<std::string, fs::path>>{};
  auto tree = get_object(get_oid(ref), "tree");

  auto ss = std::stringstream{std::move(tree)};
  auto item = std::string{};
  while (getline(ss, item, '\n')) {
    auto tokens = split(item, " ");
    if (tokens[0] == "tree") {
      res.merge(get_tree(tokens[1]));
    } else if (tokens[0] == "blob") {
      res.insert({tokens[1], cgit_root().value() / tokens[2]});
    }
  }

  return res;
}

void read_tree(const std::string &tree) {
  clear_root_dir();
  auto items = get_tree(tree);
  for (auto item : items) {
    auto content = get_object(item.first, "blob");
    write_file(item.second, content);
  }
}

std::string get_oid(const std::string &ref) {
  auto ref_dirs = std::set<fs::path>{
      cgit_root().value() / CGIT_DIR,
      cgit_root().value() / CGIT_DIR / "refs",
      cgit_root().value() / CGIT_DIR / "refs" / "tags",
      cgit_root().value() / CGIT_DIR / "refs" / "heads",
  };
  for (auto &dir : ref_dirs) {
    if (fs::is_regular_file(dir / ref)) {
      auto content = read_file(dir / ref);
      if (read_file(dir / ref).starts_with("ref")) {
        return get_oid(content.substr(content.find(' ') + 1));
      } else {
        return content;
      }
    }
  }

  // ref is oid prefix
  auto obj_pathes = os_walk(cgit_root().value() / CGIT_DIR / "objects");
  assert(obj_pathes.dirs.size() == 0);
  for (auto &file : obj_pathes.files) {
    if (file.filename().string().starts_with(ref)) {
      return file.filename();
    }
  }

  throw std::runtime_error(fmt::format("No oid corresponds to {}", ref));
}

std::vector<std::string> split(std::string str, const std::string &del) {
  auto res = std::vector<std::string>{};
  auto pos = std::string::size_type{0};
  auto token = std::string{};
  while ((pos = str.find(del)) != std::string::npos) {
    token = str.substr(0, pos);
    res.push_back(token);
    str.erase(0, pos + del.length());
  }
  res.push_back(str);
  return res;
}

std::string to_hex(const ustring str) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (auto c : str) {
    ss << std::setw(2) << static_cast<unsigned int>(c);
  }
  return ss.str();
}

void set_HEAD(const std::string &oid) { write_file(cgit_root().value() / CGIT_DIR / "HEAD", oid); }

std::string get_HEAD() {
  fs::path HEAD_path = cgit_root().value() / CGIT_DIR / "HEAD";
  if (fs::is_regular_file(HEAD_path)) {
    return read_file(HEAD_path);
  } else
    return {};
}
