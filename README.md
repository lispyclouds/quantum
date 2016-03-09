# Quantum

[![Build Status](https://travis-ci.org/trost-lang/quantum.svg?branch=master)](https://travis-ci.org/trost-lang/quantum)

## Building and running on OSX (more coming soon...)
- ``` brew install qt5 ```
- ``` brew install autoconf ```
- ``` brew install automake ```
- ``` brew install libtool ```
- ``` brew install makeself ```
- In the **libgc** directory
- ``` autoreconf -vif ```
- ``` automake --add-missing ```
- ``` ./configure --prefix=$(dirname $PWD)/gc_compiled --enable-threads=posix --enable-thread-local-alloc --enable-parallel-mark --enable-cplusplus ```
- ``` make -j $(sysctl -n hw.ncpu) ```
- ``` make install ```
- find where qmake is. Usually its in /usr/local/opt/qt5/bin/
- In the **quantum** directory
- ``` qmake -o Makefile quantum.pro -recursive -spec macx-clang ```
- ``` make -j $(sysctl -n hw.ncpu) ```
- ``` ./quantum <json.file> ``` (Sample files in jsons dir)
- Run tests with ``` python test_quantum.py ``` (Python 2.7+ needed)

## Developing
- Write awesome code
- run ``` make -j $(sysctl -n hw.ncpu) ```
- repeat
