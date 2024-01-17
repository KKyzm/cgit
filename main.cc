#include <docopt/docopt.h>
#include <docopt/docopt_value.h>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "src/base.h"
#include "src/config.h"
#include "src/data.h"

static const std::string USAGE =
    R"(cgit.

    Usage:
      cgit init
      cgit commit -m <message>
      cgit checkout <ref>
      cgit log [<ref>]
      cgit tag <name> [<ref>]
      cgit branch <name> [<ref>]
      cgit (-h | --help)
      cgit (-v | --version)

    Options:
      -h --help       Show this screen.
      -v --version    Show version.
)";

int main(int argc, const char **argv) {
  auto arg = std::vector<std::string>{argv + 1, argv + argc};
  auto args = docopt::docopt(USAGE, arg, true, "cgit 0");

  // init cmd
  if (args["init"].asBool()) {
    if (already_init()) {
      throw std::runtime_error("repo has already initialized.");
    }
    cgit_init();
  } else if (!already_init()) {
    throw std::runtime_error("repo not initialized.");
  }

  // commit cmd
  if (args["commit"].asBool()) {
    fmt::println("{}", commit(args["<message>"].asString()));
  }

  // log cmd
  if (args["log"].asBool()) {
    auto ref = args["<ref>"] ? args["<ref>"].asString() : "HEAD";
    fmt::println("{}", get_log(ref));
  }

  // checkout cmd
  if (args["checkout"].asBool()) {
    fmt::println("checkout to {}.", checkout(args["<ref>"].asString()));
  }

  // tag cmd
  if (args["tag"].asBool()) {
    auto ref = args["<ref>"] ? args["<ref>"].asString() : "HEAD";
    auto oid = create_tag(args["<name>"].asString(), ref);
    fmt::println("create tag {0} on commit {1}.", args["<name>"].asString(), oid);
  }

  // branch cmd
  if (args["branch"].asBool()) {
    auto ref = args["<ref>"] ? args["<ref>"].asString() : "HEAD";
    auto oid = create_branch(args["<name>"].asString(), ref);
    fmt::println("create branch {0} on commit {1}.", args["<name>"].asString(), oid);
  }

  return 0;
}

template <>
struct fmt::formatter<docopt::value> : ostream_formatter {};
