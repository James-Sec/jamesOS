import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST mem.c

### KHEAP_INIT
gdb.execute('call kheap_init()')
source = gdb.parse_and_eval('kmalloc_u(5)')
destination = gdb.parse_and_eval('kmalloc_u(10)')

### MEMSET
gdb.execute(f'call memset({source}, 0xaa, 5)')
expected = b'\xaa' * 5
assert(utils.TestMemoryRegion(expected, source))
print(f'\033[92m[+] MEM memset({source}, 0xaa, 5) executado corretamente.\033[0m')

gdb.execute(f'call memset({destination}, 0xbb, 10)')
expected = b'\xbb' * 10
assert(utils.TestMemoryRegion(expected, destination))
print(f'\033[92m[+] MEM memset({source}, 0xbb, 10) executado corretamente.\033[0m')

### MEMCPY
gdb.execute(f'call memcpy({source}, {destination}, 5)')
expected = (b'\xaa' * 5) + (b'\xbb' * 5)
assert(utils.TestMemoryRegion(expected, destination))
print(f'\033[92m[+] MEM memcpy({source}, {destination}, 5) executado corretamente.\033[0m')

### MEMMOV
gdb.execute(f'call memmov({source}, {source}, 5)')
expected_source = (b'\xaa' * 5)
assert(utils.TestMemoryRegion(expected_source, source))
print(f'\033[92m[+] MEM memmov({source}, {source}, 5) executado corretamente.\033[0m')

gdb.execute(f'call memmov({source}, {destination}, 5)')
expected_source = (b'\xaa' * 5)
expected_destination = (b'\xaa' * 5) + (b'\xbb' * 5)
assert(utils.TestMemoryRegion(expected_source, source))
assert(utils.TestMemoryRegion(expected_destination, destination))
print(f'\033[92m[+] MEM memmov({source}, {destination}, 5) executado corretamente.\033[0m')

gdb.execute(f'call memmov({destination}, {source}, 5)')
expected_source = (b'\xaa' * 5)
expected_destination = (b'\xaa' * 5) + (b'\xbb' * 5)
assert(utils.TestMemoryRegion(expected_source, source))
assert(utils.TestMemoryRegion(expected_destination, destination))
print(f'\033[92m[+] MEM memmov({destination}, {source}, 5) executado corretamente.\033[0m')

### MEMCMP
memcmp = gdb.parse_and_eval(f'memcmp({source}, {destination}, 5)')
assert(memcmp == 0x0)
print(f'\033[92m[+] MEM memcmp({source}, {destination}, 5) executado corretamente.\033[0m')

gdb.execute(f'call memset({source}, 0xa9, 1)')
memcmp = gdb.parse_and_eval(f'memcmp({source}, {destination}, 5)')
assert(memcmp == -1)
print(f'\033[92m[+] MEM memcmp({source}, {destination}, 5) executado corretamente.\033[0m')

gdb.execute(f'call memset({source}, 0xab, 1)')
memcmp = gdb.parse_and_eval(f'memcmp({source}, {destination}, 5)')
assert(memcmp == 1)
print(f'\033[92m[+] MEM memcmp({source}, {destination}, 5) executado corretamente.\033[0m')

utils.GdbStop()
