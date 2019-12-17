import subprocess
import os
import platform
import sys

_dir = ""
_make = ""
if platform.system() == "Linux":
    _dir = "../build"
    _make = "make"
else:
    _dir = "./cmake-build-debug"
    _make = "mingw32-make"

pipes = subprocess.Popen([_make, "docs"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=_dir)
std_out, std_err = pipes.communicate()

print(str(std_out, 'utf-8'))
print("Current directory :", os.getcwd())
if len(std_err) != 0 and " WARNING: " in str(std_err):
    print(str(std_err, 'utf-8'))
    sys.exit(-1)
sys.exit(0)