import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## BOOTLOADER
assert(utils.TestDiskLoadOnMemory('_start', '../os-image.bin', 512))
print(f'\033[92m[+] BOOTLOADER sistema operactional carregado corretamente.\033[0m')

utils.GdbStop()
