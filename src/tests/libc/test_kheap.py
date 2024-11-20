import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST KHEAP.h

### KHEAP_INIT
gdb.execute('call kheap_init()')
kheap_enable = gdb.parse_and_eval('kheap_enable')
assert(kheap_enable == 0x1)

gdb_bitset_base = gdb.parse_and_eval('bitset_base')
bitset_base = int.from_bytes(gdb_bitset_base.bytes, byteorder = 'little', signed = False)
gdb_bitset_limit = gdb.parse_and_eval('bitset_limit')
bitset_limit = int.from_bytes(gdb_bitset_limit.bytes, byteorder = 'little', signed = False)
expected = b'\x00' * (bitset_limit - bitset_base)
assert(utils.TestMemoryRegion(expected, bitset_base))
print('\033[92m[+] KHEAP iniciado corretamente.\033[0m')

### FILL
gdb.execute('call fill(0, 12, 1)')
expected = b'\xff\x0f'
gdb_bitset_base = gdb.parse_and_eval('bitset_base')
bitset_base = int.from_bytes(gdb_bitset_base.bytes, byteorder = 'little', signed = False)
assert(utils.TestMemoryRegion(expected, bitset_base))
print('\033[92m[+] KHEAP fill(0, 12,1) executado corretamente.\033[0m')

gdb.execute('call fill(4, 4, 0)')
expected = b'\x0f\x0f'
gdb_bitset_base = gdb.parse_and_eval('bitset_base')
bitset_base = int.from_bytes(gdb_bitset_base.bytes, byteorder = 'little', signed = False)
assert(utils.TestMemoryRegion(expected, bitset_base))
print('\033[92m[+] KHEAP fill(4, 4, 0) executado corretamente.\033[0m')

gdb.execute('call fill(0, 12, 0)')
expected = b'\x00\x00'
gdb_bitset_base = gdb.parse_and_eval('bitset_base')
bitset_base = int.from_bytes(gdb_bitset_base.bytes, byteorder = 'little', signed = False)
assert(utils.TestMemoryRegion(expected, bitset_base))
print('\033[92m[+] KHEAP fill(0, 0, 0) executado corretamente.\033[0m')

### KMALLOC
kmalloc = gdb.parse_and_eval('_kmalloc(0, 0)')
assert(kmalloc == 0x0)
print('\033[92m[+] KHEAP kmalloc(0, 0) executado corretamente.\033[0m')

kmalloc = gdb.parse_and_eval('_kmalloc(1, 0)')
gdb_heap_base = gdb.parse_and_eval('heap_base')
heap_base = int.from_bytes(gdb_heap_base.bytes, byteorder = 'little', signed = False)
assert(kmalloc == heap_base)
print('\033[92m[+] KHEAP kmalloc(1, 0) executado corretamente.\033[0m')

kmalloc = gdb.parse_and_eval('_kmalloc(1, 1)')
gdb_heap_base = gdb.parse_and_eval('heap_base')
heap_base = int.from_bytes(gdb_heap_base.bytes, byteorder = 'little', signed = False)
try:
    assert(kmalloc == heap_base + 0x1000)
    print('\033[92m[+] KHEAP kmalloc(1, 1) executado corretamente.\033[0m')
except:
    print('\033[31m[-] KHEAP kmalloc(1, 1) executado com erro.\033[0m')

gdb_heap_base = gdb.parse_and_eval('heap_base')
heap_base = int.from_bytes(gdb_heap_base.bytes, byteorder = 'little', signed = False)
gdb.execute(f'call kfree({heap_base}, 1)')
try:
    expected = b'\x02' # 00000010
    gdb_bitset_base = gdb.parse_and_eval('bitset_base')
    bitset_base = int.from_bytes(gdb_bitset_base.bytes, byteorder = 'little', signed = False)
    assert(utils.TestMemoryRegion(expected, bitset_base))
    print(f'\033[92m[+] KHEAP kfree({heap_base}, 1) executado corretamente.\033[0m')
except:
    print(f'\033[31m[-] KHEAP kfree({heap_base}, 1) executado com erro.\033[0m')

utils.GdbStop()
