import gdb
import os
import sys
import time
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

### KHEAP_INIT
gdb.execute('call kheap_init()')

### VGA
vga_address = 0xb8000 # VGA DMA fixed address

### GET/SET CURSOR
gdb.execute('call set_cursor_offset(7)')
cursor = gdb.parse_and_eval('get_cursor_offset()')
assert(cursor == 7)
print('\033[92m[+] VGA set_cursor_offset(7) executado corretamente.\033[0m')

### CLEAR_SCREEN
gdb.execute('call clear_screen()')
cursor = gdb.parse_and_eval('get_cursor_offset()')
expected = b'\x20\x0f' * (25 * 80) # char SPACE | attribute WHITE_ON_BLACK
assert(utils.TestMemoryRegion(expected, vga_address))
assert(cursor == 0x0)
print('\033[92m[+] VGA clear_screen() executado corretamente.\033[0m')

### PRINT_CHAR
offset = gdb.parse_and_eval('print_char(0x61, 0x0f, 0x0)')
expected = b'\x61\x0f'
assert(utils.TestMemoryRegion(expected, vga_address))
assert(offset == 0x2)
print('\033[92m[+] VGA print_char(0x61, 0x0f, 0x0) executado corretamente.\033[0m')

offset = gdb.parse_and_eval('print_char(0x0a, 0x0f, 0x0)')
assert(offset == 160)
print('\033[92m[+] VGA print_char(0x0a, 0x0f, 0x0) executado corretamente.\033[0m')

### SCROLL_SCREEN
expected = b''
for i in range(0, 25 * 80 * 2, 2):
    line = i // (80 * 2)
    _ = gdb.parse_and_eval(f'print_char({0x61 + line}, 0x0f, {i})')
    if line >= 1:
        expected = expected + bytes([0x61 + line]) + b'\x0f'
expected = expected + b'\x20\x07' * 80
gdb.execute('call scroll_screen()')
assert(utils.TestMemoryRegion(expected, vga_address))
print('\033[92m[+] VGA scroll_screen() executado corretamente.\033[0m')

### KPRINT_AT
message = 'message_00'
offset = 0
gdb.execute(f'call kprint_at("{message}", {offset})')
attribute = gdb.parse_and_eval('attribute')
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
assert(utils.TestMemoryRegion(expected, vga_address + offset))
print(f'\033[92m[+] VGA kprint_at("{message}", {offset}) executado corretamente.\033[0m')

message = 'message_01'
offset = 25 * 80 * 2
gdb.execute(f'call kprint_at("{message}", {offset})')
attribute = gdb.parse_and_eval('attribute')
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
expected_offset = 24 * 80 * 2
try:
    assert(utils.TestMemoryRegion(expected, vga_address + expected_offset))
    print(f'\033[92m[+] VGA kprint_at("{message}", {offset}) executado corretamente.\033[0m')
except:
    ### SCROLL SCREEN BUG
    print(f'\033[31m[-] VGA kprint_at("{message}", {offset}) executado com erro.\033[0m')

message = 'message_02'
offset = 25 * 80 * 2 - 10
gdb.execute(f'call kprint_at("{message}", {offset})')
attribute = gdb.parse_and_eval('attribute')
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
expected_offset = (24 * 80 * 2) - (len(message))
assert(utils.TestMemoryRegion(expected, vga_address + expected_offset))
print(f'\033[92m[+] VGA kprint_at("{message}", {offset}) executado corretamente.\033[0m')

gdb.execute('call set_cursor_offset(0)')
message = 'message_03'
gdb.execute(f'call kprint_at("{message}", -1)')
attribute = gdb.parse_and_eval('attribute')
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
expected_offset = 0x0
assert(utils.TestMemoryRegion(expected, vga_address + expected_offset))
print(f'\033[92m[+] VGA kprint_at("{message}", -1) executado corretamente.\033[0m')

# KPRINT
gdb.execute('call set_cursor_offset(0)')
message = 'message_04'
gdb.execute(f'call kprint("{message}")')
attribute = gdb.parse_and_eval('attribute')
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
expected_offset = 0x0
assert(utils.TestMemoryRegion(expected, vga_address + expected_offset))
print(f'\033[92m[+] VGA kprint("{message}") executado corretamente.\033[0m')

# KPRINT_DEBUG
gdb.execute('call set_cursor_offset(0)')
message = 'message_05'
gdb.execute(f'call kprint_debug("{message}", 0x4)')
expected_attribute = gdb.parse_and_eval('attribute')
attribute = 0x4
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
expected_offset = 0x0
assert(utils.TestMemoryRegion(expected, vga_address + expected_offset))
assert(expected_attribute == 0x0e)
print(f'\033[92m[+] VGA kprint("{message}", 0x4) executado corretamente.\033[0m')

# KPRINTF
gdb.execute('call set_cursor_offset(0)')
gdb.execute('call kprintf("%%modifiers%%: %x, %d, %s", 3, 12345678, 12345678, "teste")')
attribute = gdb.parse_and_eval('attribute')
message = '%modifiers%: 0x00bc614e, 12345678, teste'
expected = bytes(''.join([char + chr(attribute) for char in message]), 'utf-8')
assert(utils.TestMemoryRegion(expected, vga_address))
print(f'\033[92m[+] VGA kprintf("%%numbers%%: %x, %d, %s", 3, 12345678, 12345678, "teste") executado corretamente.\033[0m')

utils.GdbStop()
