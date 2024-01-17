#include "base.h"

#include <fmt/core.h>

#include <filesystem>
#include <stdexcept>

#include "config.h"
#include "data.h"
#include "fs.h"

std::string commit(const std::string &message) {
  auto content = std::string("commit\n");
  content += fmt::format("tree {}\n", hash_tree());
  if (!get_HEAD(true).empty()) {
    content += fmt::format("parent {}\n", get_HEAD(true));
  }
  content += "\n";
  content += message;
  content += "\n";

  auto oid = hash_object(content);
  write_file(cgit_root().value() / CGIT_DIR / "objects" / oid, content);

  set_HEAD(oid, true);

  return oid;
}

Commit get_commit(const std::string &oid) {
  auto res = Commit{};
  auto commit = get_object(oid, "commit");

  auto ss = std::stringstream{std::move(commit)};
  auto item = std::string{};
  while (getline(ss, item, '\n')) {
    if (item.starts_with("tree ")) {
      res.tree = split(item, " ")[1];
    } else if (item.starts_with("parent ")) {
      res.parent = split(item, " ")[1];
    } else if (item.length() > 0) {
      res.message += item;
    }
  }
  return res;
}

std::vector<std::string> get_log(const std::string &ref) {
  auto commits = std::vector<std::string>{};
  auto commit_id = ref.length() > 0 ? deref(ref).value : get_HEAD(true);

  while (commit_id.length() > 0) {
    commits.push_back(commit_id);
    commit_id = get_commit(commit_id).parent;
  }
  return commits;
}

std::string get_branch_name(const std::string &ref) {
  const auto PROBABLITIES = std::set<fs::path>{
      cgit_root().value() / CGIT_DIR / ref,                // refs/heads/master
      cgit_root().value() / CGIT_DIR / "refs" / ref,       // heads/master
      cgit_root().value() / CGIT_DIR / "refs/heads" / ref  // master
  };
  for (auto &path : PROBABLITIES) {
    if (fs::is_regular_file(path)) {
      return fs::relative(path, cgit_root().value() / CGIT_DIR);  // refs/heafs/master
    }
  }
  return {};
}

std::string checkout(const std::string &ref) {
  auto commit_id = deref(ref).value;
  auto commit = get_commit(commit_id);
  if (!get_branch_name(ref).empty()) {
    set_HEAD(std::string("ref ") + get_branch_name(ref), false);
  } else {
    set_HEAD(commit_id, false);
  }
  read_tree(commit.tree);
  return commit_id;
}

std::string reset(const std::string &ref) {
  auto commit_id = deref(ref).value;
  auto commit = get_commit(commit_id);
  set_HEAD(commit_id, true);
  read_tree(commit.tree);
  return commit_id;
}

std::string create_tag(const std::string &tag, const std::string &ref) {
  auto oid = deref(ref).value;
  write_file(cgit_root().value() / CGIT_DIR / "refs" / "tags" / tag, oid);
  return oid;
}

std::string create_branch(const std::string &branch, const std::string &ref) {
  auto oid = deref(ref).value;
  if (oid.empty()) {
    throw std::runtime_error("No commit to points to.");
  }
  write_file(cgit_root().value() / CGIT_DIR / "refs" / "heads" / branch, oid);
  return oid;
}
