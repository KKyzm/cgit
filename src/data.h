#ifndef _CGIT_DATA_H_
#define _CGIT_DATA_H_

#include <filesystem>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "config.h"

Ref deref(const std::string &);

std::string get_object(const std::string, const std::string);

std::string hash_object(std::string &);

std::string hash_file(const std::string &);

std::string hash_tree(const std::string & = cgit_root().value());

void read_tree(const std::string &);

std::vector<std::string> split(std::string, const std::string &);

std::string to_hex(const ustring);

void set_HEAD(const std::string &, const bool);

std::string get_HEAD(const bool);

#endif  // !_CGIT_DATA_H_
