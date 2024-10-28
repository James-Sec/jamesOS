import gdb
import os
import sys
import time
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST paging.c

### CLEAR_SCREEN
gdb.execute('call clear_screen()')

### KHEAP_INIT
gdb.execute('call kheap_init()')
call_paging_init = gdb.parse_and_eval('kmalloc_u(8)')
call_bytes = b'\xfb' # sti
call_bytes += b'\xb8\x38\x0b\x02\x00' # mov eax, 0x00020b38
call_bytes += b'\xff\xd0' # call eax
gdb.inferiors()[0].write_memory(call_paging_init, call_bytes)

### ISR_INSTALL
gdb.execute('call isr_install()')
gdb.execute(f'set $pc = {call_paging_init}')
gdb.execute('si')

### PAGING_INIT
gdb.execute(f'break *{call_paging_init + len(call_bytes)}')
gdb.execute('continue')
page_enabled = gdb.parse_and_eval('page_enabled')
tables_physical = gdb.parse_and_eval('&kernel_directory->tables_physical')
cr3 = gdb.parse_and_eval('$cr3')
assert(page_enabled == 0x1)
assert(cr3 == tables_physical)
print(f'\033[92m[+] PAGING paging_init() executado corretamente.\033[0m')

### FIRST_FRAME
first_frame = gdb.parse_and_eval('first_frame()')
### paging_init reserva 3840 frames de 4096
assert(first_frame == 3841)
print(f'\033[92m[+] PAGING first_frame() executado corretamente.\033[0m')

### SET_FRAME
gdb.execute('call set_frame(3841)')
bitmap = gdb.parse_and_eval('bitmap')
assert(bitmap[480] == 0b00000011)
print(f'\033[92m[+] PAGING set_frame() executado corretamente.\033[0m')

### CLEAR_FRAME
gdb.execute('call clear_frame(3841)')
bitmap = gdb.parse_and_eval('bitmap')
assert(bitmap[480] == 0b00000001)
print(f'\033[92m[+] PAGING clear_frame() executado corretamente.\033[0m')

### GET_PAGE
current_directory = gdb.parse_and_eval('current_directory')
page = gdb.parse_and_eval(f'get_page(0x0, 0x0, {hex(current_directory)})')
#expected_page = gdb.parse_and_eval('current_directory.tables[0].pages[0]')
expected_page = current_directory['tables'][0]['pages'][0]
assert(expected_page.address == page)
print(f'\033[92m[+] PAGING get_page(0x0, 0x0, {hex(current_directory)}) executado corretamente.\033[0m')

current_directory = gdb.parse_and_eval('current_directory')
page = gdb.parse_and_eval(f'get_page(0x1000000, 0x1, {hex(current_directory)})')
expected_page = current_directory['tables'][0x4]['pages'][0]
### now there is 5 physical pages
assert(current_directory['tables_physical'][4]['present'] == 1)
assert(expected_page.address == page)
print(f'\033[92m[+] PAGING get_page(0x1000000, 0x1, {hex(current_directory)}) executado corretamente.\033[0m')

current_directory = gdb.parse_and_eval('current_directory')
# 0x400000 => 1024 page_table_t * 4096 page_size
page = gdb.parse_and_eval(f'get_page(0x1400000, 0x0, {hex(current_directory)})')
assert(0x0 == page)
print(f'\033[92m[+] PAGING get_page(0x1400000, 0x0, {hex(current_directory)}) executado corretamente.\033[0m')

### ALLOC_PAGE
current_directory = gdb.parse_and_eval('current_directory')
page = gdb.parse_and_eval(f'get_page(0x1000000, 0x0, {hex(current_directory)})')
gdb.execute(f'call alloc_page({hex(page)}, 0x0, 0x0)')
assert(page['present'] == 0x1)
assert(page['rw'] == 0x0)
assert(page['user'] == 0x1)
assert(page['frame'] == 3841)
print(f'\033[92m[+] PAGING alloc_page({hex(page)}, 0x0, 0x0) executado corretamente.\033[0m')

#### FILL FRAMES
while True:
    frame = gdb.parse_and_eval('first_frame()')
    if frame == 0xffffffff:
        break
    gdb.execute(f'call set_frame({frame})')

current_directory = gdb.parse_and_eval('current_directory')
page = gdb.parse_and_eval(f'get_page(0x1001000, 0x0, {hex(current_directory)})')
gdb.execute('break *0x00020a74') # break before HLT in alloc_page
try:
    gdb.execute(f'call alloc_page({hex(page)}, 0x0, 0x0)')
except:
    pass
print(f'\033[92m[+] PAGING alloc_page({hex(page)}, 0x0, 0x0) executado corretamente.\033[0m')

### FREE_PAGE
current_directory = gdb.parse_and_eval('current_directory')
page = gdb.parse_and_eval(f'get_page(0x1000000, 0x0, {hex(current_directory)})')
gdb.execute(f'call free_page({hex(page)})')
assert(page['present'] == 0x0)
assert(page['frame'] == 0x0)
print(f'\033[92m[+] PAGING get_page(0x1001000, 0x0, {hex(current_directory)}) executado corretamente.\033[0m')

### VIRTUAL2PHYS
gdb.execute('set page_enabled = 0')
current_directory = gdb.parse_and_eval('current_directory')
phys = gdb.parse_and_eval(f'virtual2phys({hex(current_directory.address)}, 0x1234)')
assert(0x1234 == phys)
gdb.execute('set page_enabled = 1')
print(f'\033[92m[+] PAGING virtual2phys({hex(current_directory.address)}, 0x1234) executado corretamente.\033[0m')

current_directory = gdb.parse_and_eval('current_directory')
phys = gdb.parse_and_eval(f'virtual2phys({hex(current_directory)}, 0xffffffff)')
assert(0x0 == phys)
print(f'\033[92m[+] PAGING virtual2phys({hex(current_directory)}, 0xffffffff) executado corretamente.\033[0m')

current_directory = gdb.parse_and_eval('current_directory')
phys = gdb.parse_and_eval(f'virtual2phys({hex(current_directory)}, 0x1000000)')
assert(0x0 == phys)
print(f'\033[92m[+] PAGING virtual2phys({hex(current_directory)}, 0x1000000) executado corretamente.\033[0m')

current_directory = gdb.parse_and_eval('current_directory')
phys = gdb.parse_and_eval(f'virtual2phys({hex(current_directory)}, 0x1234)')
assert(0x1234 == phys)
print(f'\033[92m[+] PAGING virtual2phys({hex(current_directory)}, 0x1234) executado corretamente.\033[0m')

### PAGE FAULT TEST
### int *x = 0xf01000
### *x = 8;
fault_bytes = b'\xfb\xc7\x45\xf4\x00\x10\xf0\x00\x8b\x45\xf4\xc7\x00\x08\x00\x00\x00'
run_fault = gdb.parse_and_eval(f'kmalloc_u({len(fault_bytes)})')
gdb.inferiors()[0].write_memory(run_fault, fault_bytes)
gdb.execute(f'set $pc = {run_fault}')
gdb.execute('si')
gdb.execute('b *0x00020f08') #end page_fault_handler
print(f'\033[92m[+] PAGING page_fault_handler() chamado corretamente.\033[0m')

utils.GdbStop()
