# Quantum

## building and running on OSX (more coming soon...)
- ``` brew install qt5 ```
- find where qmake is usually its in /usr/local/opt/qt5/bin/
- ``` qmake -o Makefile quantum.pro -recursive -spec macx-clang ```
- ``` make -j $(sysctl -n hw.ncpu) ```
- ``` ./quantum test/simple.json ```

## Developing
- Write awesome code
- run ``` make -j $(sysctl -n hw.ncpu) ```
- repeat
