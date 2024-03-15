# cgit: toy git in modern C++

The code and the general structure of the program are primarily based on the following blog post:

- [Git Internals - Learn by Building Your Own Git](https://www.leshenko.net/p/ugit/)

Please note that this project was created primarily to familiarize me with the C++ language. Therefore, the code may not conform to normal C++ style, and there are undoubtedly many places where the code can be optimized.

## build

To build this project, follow the commands below:

```bash
cmake --preset=default
cmake --build build
```

## Run

The program supports the following subcommands:
| Subcommands | Description                                          |
| ----------- | ---------------------------------------------------- |
| init        | Initialize a cgit repository                         |
| commit      | Store the directory as a snapshot                    |
| log         | Print log history                                    |
| tag         | Attach tag to a commit                               |
| branch      | Create a new branch                                  |
| checkout    | Move HEAD pointer                                    |
| reset       | Reset HEAD pointer                                   |

Use -h option to check usage.

```bash
cgit -h
```

## TODO

To make it more useful, some new features are needed:

- [ ] add Stage/Index, and distinguish Working Tree, Stage/Index and Repository
- [ ] subcommand status
- [ ] subcommand diff
- [ ] subcommand merge

Since this is only a project I started for fun and to learn C++, it is unlikely that any of these features will be implemented anytime soon. If you'd like to build on top of my current code, feel free to do so.
