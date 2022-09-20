### Work in progrss toy c compiler written in c
I've done a similar stuff on other languages, but I wanted to try if c
was capable of the same except the lack of generics and that I completely
forgot how to use the debugger no mayor issues encountered.

### How to build
```shell
meson setup build
meson compile -C build
```
#### Why meson?
Because is the only c/c++ package manager that doesn't suck too much, you
can create a working project with external dependencies in less than 30 
seconds (unlike cmake) and doesn't bloat (like bazel), clib is also a good
option, but isn't really widely used and meson integrates with cmake and 
autotools most of the times without issues

### Code style guidelines
Lots of comments and snake case, like the stdlib
