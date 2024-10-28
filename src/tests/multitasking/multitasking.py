import gdb
import os
import sys
import time
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()
jamesos = gdb.inferiors()[0]

## TEST multitask.c

### KHEAP_INIT
gdb.execute('call kheap_init()')
pname = gdb.parse_and_eval('kmalloc_u(10)')
sti = gdb.parse_and_eval('kmalloc_u(1)')
jamesos.write_memory(sti, b'\xfb')
software_interrupt = gdb.parse_and_eval('kmalloc_u(2)')
jamesos.write_memory(software_interrupt, b'\xcd\x20')

### ISR_INSTALL
gdb.execute('call isr_install()')

### PIT INIT
gdb.execute('call pit_init(0)')

### CREATE_TASK
'''
jamesos.write_memory(pname, b'minha task')
idle_state = gdb.parse_and_eval('IDLE')
idle_task = gdb.parse_and_eval(f'create_task(0x1234, {pname}, {idle_state}, 1, 0x4321)')
kernel_directory = gdb.parse_and_eval('kernel_directory')
expected_ebp = 0x11f00d + 0x1000 - 0x4
expected_esp = expected_ebp - 28
assert(idle_task['ebp'] == expected_ebp)
assert(idle_task['esp'] == expected_esp)
assert(idle_task['page_dir'] == kernel_directory)
assert(idle_task['next_task'] == 0)
assert(idle_task['pid'] == 0)
assert(idle_task['state'] == idle_state)
assert(utils.TestMemoryRegion(b'minha task', idle_task['pname'].address))

jamesos.write_memory(pname, b'custom task')
custom_state = gdb.parse_and_eval('READY_TO_RUN')
custom_task = gdb.parse_and_eval(f'create_task(0x1234, {pname}, {custom_state}, 1, 0x4321)')
kernel_directory = gdb.parse_and_eval('kernel_directory')
expected_ebp = 0x12000d + 0x1000 - 0x4
expected_esp = expected_ebp - 28
assert(custom_task['ebp'] == expected_ebp)
assert(custom_task['esp'] == expected_esp)
assert(custom_task['page_dir'] == kernel_directory)
assert(custom_task['next_task'] == 0)
assert(custom_task['pid'] == 2)
assert(custom_task['state'] == custom_state)
assert(utils.TestMemoryRegion(b'custom task', custom_task['pname'].address))
'''

### MULTITASK INIT
gdb.execute('call multitask_init()')
multitasking_on = gdb.parse_and_eval('multitasking_on')
current_task = gdb.parse_and_eval('current_task')
idle_task = gdb.parse_and_eval('idle_task')
head = gdb.parse_and_eval('head')
assert(multitasking_on == 0x1)
assert(utils.TestMemoryRegion(b'JAMES', current_task['pname'].address))
assert(utils.TestMemoryRegion(b'IDLE', idle_task['pname'].address))
assert(utils.TestMemoryRegion(b'CHUAZNEGUER', head['pname'].address))
print(f'\033[92m[+] MULTITASKING multitask_init() executado corretamente.\033[0m')

gdb.execute(f'break *0x20005')

gdb.execute(f'set $pc = {sti}')
gdb.execute('si')

gdb.execute(f'set $pc = *0x20005')
gdb.execute('continue')

'''
for i in range(504):
    ###
    gdb.execute(f'set $pc = {software_interrupt}')
    gdb.execute(f'break *{software_interrupt + 2}')
    while True:
        gdb.execute('si')
        pc = gdb.parse_and_eval('$pc')
        if pc == (software_interrupt + 2):
            break

    current_task = gdb.parse_and_eval('current_task')
    print(current_task['pname'])
    print(current_task['next_task'])

gdb.execute(f'set $pc = {software_interrupt}')
gdb.execute(f'break *{software_interrupt + 2}')
gdb.execute('continue')
current_task = gdb.parse_and_eval('current_task')
print(current_task['pname'])
'''

while True:
    continue

utils.GdbStop()
