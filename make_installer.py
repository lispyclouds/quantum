#!/usr/bin/python

from sys import platform as _platform
from os.path import realpath, exists
from os import makedirs
from shutil import copyfile
from subprocess import call

import platform


if _platform not in ["linux", "linux2", "darwin"]:
    print "Platform not supported by Trost"
    exit(-1)

print "Copying the trost executable"
copyfile("trost", "installer/trost")

print "Copying the Qt libraries"
if _platform == "linux" or _platform == "linux2":
    copyfile(realpath("/usr/lib/x86_64-linux-gnu/libQt5Core.so.5"), "installer/libQt5Core.so.5")
    os = "linux"
elif _platform == "darwin":
    copyfile(realpath("/usr/local/opt/qt5/lib/QtCore.framework/Versions/5/QtCore"), "installer/QtCore")
    os = "mac"

arch = platform.machine()

call(["makeself", "--bzip2", "--notemp", "./installer", "./trost_installer_%s-%s.bz2.sh" % (os, arch), "SFX archive for Quantum", "./install.py"])
