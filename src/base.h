#ifndef _CGIT_BASE_H_
#define _CGIT_BASE_H_

#include <set>
#include <string>
#include <vector>

struct Commit {
  std::string parent{};
  std::string tree{};
  std::string message{};
};

std::string commit(const std::string &);

std::vector<std::string> get_log(const std::string &);

std::string checkout(const std::string &);

std::string create_tag(const std::string &, const std::string &);

std::string create_branch(const std::string &, const std::string &);

#endif  // !_CGIT_BASE_H_
