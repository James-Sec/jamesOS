import asyncio
import gdb
import os
import sys
import time
import threading
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

### VGA
vga_address = 0xb8000 # VGA DMA fixed address

### KHEAP_INIT
gdb.execute('call kheap_init()')
sti = gdb.parse_and_eval('kmalloc_u(1)')

### ISR_INIT
gdb.execute('call isr_install()')

### ENABLE INTERRUPTS
gdb.execute(f'set *{sti} = 0xfb')
gdb.execute(f'set $pc = {sti}')
gdb.execute('si')

### KEYBOARD_INIT
gdb.execute('call keyboard_init()')

### CLEAR_SCREEN
gdb.execute('call clear_screen()')

### INFINITE LOOP
gdb.execute('set $pc = 0x20005')
gdb.execute('break *0x210aa')

def test_key(key, code, expected_press, expected_release):
    cont = threading.Thread(target = gdb.execute, args = ('continue',))
    cont.start()
    asyncio.run(utils.QmpCommand('send-key', {
            'keys': [
                {'data': f'{key}', 'type': 'qcode'}
            ]
        }))
    cont.join()

    expected_press = b''.join([b.encode()+b'\x0f' for b in expected_press])
    assert(utils.TestMemoryRegion(expected_press, vga_address))
    print(f'\033[92m[+] KEYBOARD keyboard_callback executado corretamente (press {key}).\033[0m')

    gdb.execute('continue')
    expected_release = b''.join([b.encode()+b'\x0f' for b in expected_release])
    assert(utils.TestMemoryRegion(expected_release, vga_address + (80 * 2)))
    print(f'\033[92m[+] KEYBOARD keyboard_callback executado corretamente (release {key}).\033[0m')

test_key('a', 0x1e, 'scan_code: 30 (a)', 'scan_code: 158 (a released)')
gdb.execute('call clear_screen()')
test_key('caps_lock', 0x3a, 'scan_code: 58 (unknown)', 'scan_code: 186 (unknown released)')

utils.GdbStop()
