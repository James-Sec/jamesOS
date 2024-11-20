import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST bitmap.h

### KHEAP_INIT
gdb.execute('call kheap_init()')
bitmap = gdb.parse_and_eval('kmalloc_u(5)')

### BITMAP_SET
gdb.execute(f'call bitmap_set({bitmap}, 4)')
expected = b'\x10\x00\x00\x00\x00'
assert(utils.TestMemoryRegion(expected, bitmap))
print(f'\033[92m[+] BITMAP bitmap_set({bitmap}, 4) executado corretamente.\033[0m')

### BITMAP_READ
read = gdb.parse_and_eval(f'bitmap_read({bitmap}, 4)')
assert(read == 0x1)
print(f'\033[92m[+] BITMAP bitmap_read({bitmap}, 4) executado corretamente.\033[0m')

### BITMAP_CLEAR
gdb.execute(f'call bitmap_clear({bitmap}, 4)')
expected = b'\x00\x00\x00\x00\x00'
assert(utils.TestMemoryRegion(expected, bitmap))
print(f'\033[92m[+] BITMAP bitmap_clear({bitmap}, 4) executado corretamente.\033[0m')

### BITMAP_FILL
gdb.execute(f'call bitmap_fill({bitmap}, 40, 4, 12, 1)')
expected = b'\xf0\xff\x00\x00\x00'
assert(utils.TestMemoryRegion(expected, bitmap))
print(f'\033[92m[+] BITMAP bitmap_fill({bitmap}, 40, 4, 12, 1) executado corretamente.\033[0m')

gdb.execute(f'call bitmap_fill({bitmap}, 40, 12, 4, 0)')
expected = b'\xf0\x0f\x00\x00\x00'
assert(utils.TestMemoryRegion(expected, bitmap))
print(f'\033[92m[+] BITMAP bitmap_fill({bitmap}, 40, 12, 4, 0) executado corretamente.\033[0m')

### BITMAP_FIND_SEQUENCE
start_position = gdb.parse_and_eval(f'bitmap_find_sequence({bitmap}, 40, 8, 4, 1)')
assert(start_position == 0x8)
print(f'\033[92m[+] BITMAP bitmap_find_sequence({bitmap}, 40, 8, 4, 1) executado corretamente.\033[0m')

start_position = gdb.parse_and_eval(f'bitmap_find_sequence({bitmap}, 40, 8, 5, 1)')
try:
    assert(start_position == -1)
    print(f'\033[92m[+] BITMAP bitmap_find_sequence({bitmap}, 40, 8, 5, 1) executado corretamente.\033[0m')
except:
    print(f'\033[31m[-] BITMAP bitmap_find_sequence({bitmap}, 40, 8, 5, 1) executado com erro.\033[0m')

utils.GdbStop()
