import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST string.h

### KHEAP_INIT
gdb.execute('call kheap_init()')
string = gdb.parse_and_eval('kmalloc_u(10)')

### ITOA
### CANNOT TEST SIGNED CONVERSION, variables are unsigned
gdb.execute(f'call itoa(12345678, {string})')
expected = b'12345678'
assert(utils.TestMemoryRegion(expected, string))
print(f'\033[92m[+] STRING itoa(12345678, {string}) executado corretamente.\033[0m')

### REVERSE
gdb.execute(f'call reverse({string})')
expected = b'87654321'
assert(utils.TestMemoryRegion(expected, string))
print(f'\033[92m[+] STRING reverse({string}) executado corretamente.\033[0m')

### STRLEN
size = gdb.parse_and_eval(f'strlen({string})')
assert(size == 8)
print(f'\033[92m[+] STRING reverse({string}) executado corretamente.\033[0m')


utils.GdbStop()
