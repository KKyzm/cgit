# cgit: toy git in modern C++

The code and the general structure of the program are primarily based on the following blog post:

- [Git Internals - Learn by Building Your Own Git](https://www.leshenko.net/p/ugit/)

Note that this C++ project is started to familiarize myself with the language. Hence, the code might not comply with the normal C++ styles, and there are undoubtedly places where the code can be optimized.

## build

To build this project, follow the commands below:

```bash
make vcpkg_install      # install dependencies
cmake --preset=default
cmake --build build
```

## Run

The program supports the following subcommands:
| Subcommands | Description                                          |
| ----------- | ---------------------------------------------------- |
| init        | Initialize a cgit repository                          |
| commit      | Store the directory as a snapshot |
| log         | Print log history                                    |
| tag         | Attach tag to a commit                               |
| branch      | Create a new branch                                  |
| checkout    | move HEAD pointer                                    |
| reset       | reset HEAD pointer                                   |

Use -h option to check usage.

```bash
cgit -h
```

## TODO

To make it more useful, some new features is needed:

- [ ] add Stage/Index, and distinguish Working Tree, Stage/Index and Repository
- [ ] subcommand status
- [ ] subcommand diff
- [ ] subcommand merge

Since this is only a project that I started for fun and to learn C++, it is unlikely that any of those features will be implemented any time soon. If you'd like to build on top of my current code, feel free to do so.
