import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST minmax.h

### MAX
value = gdb.parse_and_eval('max(1, 2)')
assert(value == 2)
print(f'\033[92m[+] MINMAX max(1, 2) executado corretamente.\033[0m')

value = gdb.parse_and_eval('max(2, 1)')
assert(value == 2)
print(f'\033[92m[+] MINMAX max(2, 1) executado corretamente.\033[0m')

### MIN
value = gdb.parse_and_eval('min(1, 2)')
assert(value == 1)
print(f'\033[92m[+] MINMAX min(1, 2) executado corretamente.\033[0m')

value = gdb.parse_and_eval('min(2, 1)')
assert(value == 1)
print(f'\033[92m[+] MINMAX min(2, 1) executado corretamente.\033[0m')


utils.GdbStop()
