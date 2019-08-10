# Trivial-Dashboard
> Showcase program for Nontrivial-MIPS, NSCSCC19

## Building
You may want to configure your cross compile toolchain first as followed.

The CMake configuration expects an output from buildroot. You may alter the path to the output by setting the environment variable `TDB_BUILDROOT_OUTPUT`. By default it's /opt/buildroot/output.

buildroot should be configured to contain QtCore and C++ support for host toolchain at least.

```bash
cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../buildroot.toolchain.cmake ..
make
```
