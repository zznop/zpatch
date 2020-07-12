# zpatch

![demo zpatch](zpatch.gif)

# Description

zpatch is a helper utility for applying patches to SEGA Genesis ROM files. It takes a path to a ROM file and base
offset as inputs and launches a text editor so that patches can be written in assembly. Upon saving the assembly
code and exiting the text editor, zpatch compiles the patch file, extract the text section, and creates a new ROM
containing the patch at the specified offset.

# Building

To build this project, you must build latest capstone library from source (for M68K support).

```
git clone https://github.com/aquynh/capstone.git
cd capstone
make
sudo make install
```

zpatch also needs the GNU cross-compiler toolchain for building patch files:

```
sudo apt install gcc-m68k-linux-gnu
```

At this point you can install SCons and clone/build zpatch:

```
sudo apt install scons
git clone https://github.com/zznop/zpatch.git
cd zpatch
scons
```
