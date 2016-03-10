#!/usr/bin/python

from sys import platform as _platform
from shutil import copyfile
from subprocess import call

import os
import getpass

linux_path = "/usr/lib/x86_64-linux-gnu"
mac_path = "/usr/local/opt/qt5/lib/QtCore.framework/Versions/5"

if _platform == "linux" or _platform == "linux2":
    if not os.path.exists(linux_path):
        os.makedirs(linux_path)

    copyfile("libQt5Core.so.5", "%s/libQt5Core.so.5" % linux_path)
elif _platform == "darwin":
    if not os.path.exists(mac_path):
        os.makedirs(mac_path)

    copyfile("QtCore", "%s/QtCore" % mac_path)

copyfile("quantum", "/usr/local/bin/quantum")
call(["chmod", "755", "/usr/local/bin/quantum"])
call(["chmod", "-R", "755", "/usr/local/opt/qt5"])
