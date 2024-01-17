#include "base.h"

#include <fmt/core.h>

#include <filesystem>

#include "config.h"
#include "data.h"
#include "fs.h"

std::string commit(const std::string &message) {
  auto content = std::string("commit\n");
  content += fmt::format("tree {}\n", hash_tree());
  if (!get_HEAD().empty()) {
    content += fmt::format("parent {}\n", get_HEAD());
  }
  content += "\n";
  content += message;
  content += "\n";

  auto oid = hash_object(content);
  write_file(cgit_root().value() / CGIT_DIR / "objects" / oid, content);

  set_HEAD(oid);

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
  auto commit_id = ref.length() > 0 ? get_oid(ref) : get_HEAD();

  while (commit_id.length() > 0) {
    commits.push_back(commit_id);
    commit_id = get_commit(commit_id).parent;
  }
  return commits;
}

bool is_branch(const std::string &ref) {
  if (ref.starts_with("refs/heads")) {
    return fs::is_regular_file(cgit_root().value() / CGIT_DIR / ref);
  } else if (ref.starts_with("heads")) {
    return fs::is_regular_file(cgit_root().value() / CGIT_DIR / "refs" / ref);
  } else {
    return fs::is_regular_file(cgit_root().value() / CGIT_DIR / "refs/heads" / ref);
  }
}

std::string checkout(const std::string &ref) {
  auto commit_id = get_oid(ref);
  auto commit = get_commit(commit_id);
  if (is_branch(ref)) {
    set_HEAD(std::string("ref ") + ref);
  } else {
    set_HEAD(commit_id);
  }
  read_tree(commit.tree);
  return commit_id;
}

std::string create_tag(const std::string &tag, const std::string &ref) {
  auto oid = get_oid(ref);
  write_file(cgit_root().value() / CGIT_DIR / "refs" / "tags" / tag, oid);
  return oid;
}

std::string create_branch(const std::string &branch, const std::string &ref) {
  auto oid = get_oid(ref);
  write_file(cgit_root().value() / CGIT_DIR / "refs" / "heads" / branch, oid);
  return oid;
}
