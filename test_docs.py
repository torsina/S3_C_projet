import subprocess
import platform
import sys

_dir = ""
if platform.system() == "Linux":
    _dir = "./build"
else:
    _dir = "./cmake-build-debug"

pipes = subprocess.Popen("make docs", stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=_dir)
std_out, std_err = pipes.communicate()

print(std_out)
print("")
if len(std_err) != 0:
    print(std_err)
    sys.exit(-1)
sys.exit(0)