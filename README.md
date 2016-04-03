# Quantum

[![Build Status](https://travis-ci.org/trost-lang/quantum.svg?branch=master)](https://travis-ci.org/trost-lang/quantum)

## Building and running on OSX (more coming soon...)
- ``` brew install qt5 autoconf automake libtool makeself flex bison```
- In the **libgc** directory
- ``` autoreconf -vif ```
- ``` automake --add-missing ```
- ``` ./configure --prefix=$(dirname $PWD)/gc_compiled --enable-threads=posix --enable-thread-local-alloc --enable-parallel-mark --enable-cplusplus ```
- ``` make -j $(sysctl -n hw.ncpu) ```
- ``` make install ```
- In the **compiler** directory
- ``` flex -8 -o trost.lex.cpp trost.l ```
- ``` bison -d -o trost.tab.cpp trost.y ```
- find where qmake is. Usually its in /usr/local/opt/qt5/bin/
- In the **quantum** directory
- ``` qmake -o Makefile trost.pro -recursive -spec macx-clang ```
- ``` make -j $(sysctl -n hw.ncpu) ```
- ``` ./trost -j <json.file> ``` (Sample files in jsons dir)
- ``` ./trost -j <program.tr> ```
- Run tests with ``` python test_quantum.py ``` (Python 2.7+ needed)

## Developing
- Write awesome code
- run ``` make -j $(sysctl -n hw.ncpu) ```
- repeat

## Installing built binary (for any UNIX system)
- Use the quantum_installer_*.bz2.sh file
- Install using: ``` sudo sh quantum_installer_*.bz2.sh ```
