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

### CLEAR_SCREEN
gdb.execute('call clear_screen()')

### KHEAP_INIT
gdb.execute('call kheap_init()')
sti = gdb.parse_and_eval('kmalloc_u(1)')
software_interrupt = gdb.parse_and_eval('kmalloc_u(2)')
pname = gdb.parse_and_eval('kmalloc_u(10)')

last_kmalloc = pname + 10 + 0x3000 # + size + 3 new tasks on init

jamesos.write_memory(sti, b'\xfb')
jamesos.write_memory(software_interrupt, b'\xcd\x20')

### ISR_INSTALL
gdb.execute('call isr_install()')


### MULTITASK INIT
gdb.execute('call multitask_init()')
multitasking_on = gdb.parse_and_eval('multitasking_on')
kernel_task = gdb.parse_and_eval('current_task')
idle_task = gdb.parse_and_eval('idle_task')
head = gdb.parse_and_eval('head')

assert(multitasking_on == 0x1)
assert(utils.TestMemoryRegion(b'JAMES', kernel_task['pname'].address))
assert(utils.TestMemoryRegion(b'IDLE', idle_task['pname'].address))
assert(utils.TestMemoryRegion(b'CHUAZNEGUER', head['pname'].address))
print(f'\033[92m[+] MULTITASKING multitask_init() executado corretamente.\033[0m')

### CREATE_TASK
jamesos.write_memory(pname, b'custom task')
custom_state = gdb.parse_and_eval('BLOCKED')
general_task_function = gdb.parse_and_eval('general_task_function')
custom_task = gdb.parse_and_eval(f'create_task({hex(general_task_function.address)}, {pname}, {custom_state}, 1, 0x1234)')
kernel_directory = gdb.parse_and_eval('kernel_directory')
expected_ebp = last_kmalloc + 0x1000 - 0x4
expected_esp = expected_ebp - 28
ready_to_run_counter = gdb.parse_and_eval('ready_to_run_counter')
assert(custom_task['ebp'] == expected_ebp)
assert(custom_task['esp'] == expected_esp)
assert(custom_task['page_dir'] == kernel_directory)
assert(custom_task['next_task'] == head)
assert(custom_task['pid'] == 3)
assert(custom_task['state'] == custom_state)
assert(utils.TestMemoryRegion(b'custom task', custom_task['pname'].address))
assert(ready_to_run_counter == 0x2)
print(f'\033[92m[+] MULTITASKING create_task(0x1234, {pname}, {custom_state}, 1, 0x4312) executado corretamente.\033[0m')


### LOCK_IRQ
gdb.execute('call lock_irq()')
lock_irq_counter = gdb.parse_and_eval('lock_irq_counter')
assert(lock_irq_counter == 0x1)
print(f'\033[92m[+] MULTITASKING lock_irq() executado corretamente.\033[0m')

### UNLOCK_IRQ
gdb.execute('call unlock_irq()')
lock_irq_counter = gdb.parse_and_eval('lock_irq_counter')
assert(lock_irq_counter == 0x0)
print(f'\033[92m[+] MULTITASKING unlock_irq() executado corretamente.\033[0m')

gdb.execute('set lock_irq_counter = 2')
gdb.execute('call unlock_irq()')
lock_irq_counter = gdb.parse_and_eval('lock_irq_counter')
assert(lock_irq_counter == 0x1)
print(f'\033[92m[+] MULTITASKING unlock_irq() executado corretamente.\033[0m')
gdb.execute('set lock_irq_counter = 0')

### PRINT_TASK
def print_task_assert(expected_task, attribute, vga_address, start_line):
    current_task = gdb.parse_and_eval('current_task')
    pname = jamesos.read_memory(current_task["pname"], 32).tobytes().split(b'\x00')[0]
    expected_pname = f'pname: {pname.decode()}'
    expected_pname = bytes(''.join([char + chr(attribute) for char in expected_pname]), 'utf-8')
    expected_pid = f'pid: {current_task["pid"]}'
    expected_pid = bytes(''.join([char + chr(attribute) for char in expected_pid]), 'utf-8')
    expected_esp = f'esp: {int(current_task["esp"]):#010x}'
    expected_esp = bytes(''.join([char + chr(attribute) for char in expected_esp]), 'utf-8')
    expected_next_task = f'next_task: {int(current_task["next_task"]):#010x}'
    expected_next_task = bytes(''.join([char + chr(attribute) for char in expected_next_task]), 'utf-8')
    expected_page_dir = f'page_directory: {int(current_task["page_dir"]):#010x}'
    expected_page_dir = bytes(''.join([char + chr(attribute) for char in expected_page_dir]), 'utf-8')
    expected_state = f'current state: {int(current_task["state"])}'
    expected_state = bytes(''.join([char + chr(attribute) for char in expected_state]), 'utf-8')
    gdb.execute(f'call print_task({hex(expected_task)})')

    assert(utils.TestMemoryRegion(expected_pname, vga_address + (160 * (start_line))))
    assert(utils.TestMemoryRegion(expected_pid, vga_address + (160 * (start_line + 1))))
    assert(utils.TestMemoryRegion(expected_esp, vga_address + (160 * (start_line + 2))))
    assert(utils.TestMemoryRegion(expected_next_task, vga_address + (160 * (start_line + 3))))
    assert(utils.TestMemoryRegion(expected_page_dir, vga_address + (160 * (start_line + 4))))
    assert(utils.TestMemoryRegion(expected_state, vga_address + (160 * (start_line + 5))))

    print(f'\033[92m[+] MULTITASKING print_task({hex(expected_task)}) executado corretamente.\033[0m')


current_task = gdb.parse_and_eval('current_task')
attribute = gdb.parse_and_eval('attribute')
vga_address = 0xb8000
print_task_assert(current_task, attribute, vga_address, 1)

attribute = gdb.parse_and_eval('attribute')
vga_address = 0xb8000
print_task_assert(0, attribute, vga_address, 8)


### SEARCH_TASK
head_task = gdb.parse_and_eval('head')
search = gdb.parse_and_eval(f'search_task({hex(head_task["pid"])})')
assert(head_task == search)
print(f'\033[92m[+] MULTITASKING search_task({hex(head_task["pid"])}) executado corretamente.\033[0m')

search = gdb.parse_and_eval(f'search_task(1234)')
assert(search == 0)
print(f'\033[92m[+] MULTITASKING search_task(1234) executado corretamente.\033[0m')


### DISPATCHER
current_task_expected = gdb.parse_and_eval('current_task')
gdb.execute(f'call (void)dispatcher({hex(current_task)})')
current_task = gdb.parse_and_eval('current_task')
assert(current_task_expected == current_task)
print(f'\033[92m[+] MULTITASKING dispatcher({hex(current_task)}) executado corretamente.\033[0m')

### SCHEDULER
gdb.execute('break *0x00022fa7')
try:
    gdb.execute(f'call scheduler()')
except gdb.error as e:
    pass
next_task = gdb.parse_and_eval('next')
assert(next_task['state'] == gdb.parse_and_eval('RUNNING'))
assert(utils.TestMemoryRegion(b'CHUAZNEGUER', next_task['pname'].address))
print(f'\033[92m[+] MULTITASKING scheduler() executado corretamente.\033[0m')

try:
    gdb.execute(f'call scheduler()')
except gdb.error as e:
    pass
gdb.execute('delete 2')

### TASK_TERMINATION
gdb.execute(f'set current_task = {hex(custom_task)}')
terminated = gdb.parse_and_eval('TERMINATED')
ready_to_run_counter_expected = gdb.parse_and_eval('ready_to_run_counter')
argc = 4
argp = gdb.parse_and_eval(f'kmalloc_u({argc})')

gdb.execute('break scheduler')
try:
    gdb.execute(f'call task_termination({argc}, {argp})')
except gdb.error as e:
    pass

current_task = gdb.parse_and_eval('current_task')
ready_to_run_counter = gdb.parse_and_eval('ready_to_run_counter')
assert(int(current_task['state']) == terminated)
assert(ready_to_run_counter == ready_to_run_counter_expected)
print(f'\033[92m[+] MULTITASKING task_termination({argc}, {argp}) executado corretamente.\033[0m')
gdb.execute(f'set current_task = {hex(kernel_task)}')
gdb.execute(f'delete 3')


### BLOCK_TASK
ready_to_run_counter_expected = gdb.parse_and_eval('ready_to_run_counter') - 1
state = gdb.parse_and_eval('BLOCKED')
gdb.execute('break scheduler')

try:
    gdb.execute(f'call block_task({int(state)}, 1)')
except gdb.error:
    pass

ready_to_run_counter = gdb.parse_and_eval('ready_to_run_counter')
current_task = gdb.parse_and_eval('current_task')
assert(ready_to_run_counter == ready_to_run_counter_expected)
assert(current_task['state'] == state)
print(f'\033[92m[+] MULTITASKING block_task({int(state)}, 1) executado corretamente.\033[0m')

gdb.execute('delete 4')

### UNBLOCK_TASK
pid = 1
gdb.execute('break *0x00022ea0')
running_state = gdb.parse_and_eval('RUNNING')

try:
    gdb.execute(f'call unblock_task({pid})')
except gdb.error:
    pass

tmp_task = gdb.parse_and_eval('tmp')
current_task = gdb.parse_and_eval('current_task')
assert(tmp_task == kernel_task)
assert(current_task == kernel_task)
assert(tmp_task['state'] == running_state)
print(f'\033[92m[+] MULTITASKING unblock_task({pid}) executado corretamente.\033[0m')

gdb.execute('delete 5')

### SLEEP
gdb.execute('break *0x00022f25')
gdb.execute('break scheduler')
tick = gdb.parse_and_eval('tick')
sleeping_state = gdb.parse_and_eval('SLEEPING')

seconds = 5
try:
    gdb.execute(f'call sleep({seconds})')
except gdb.error:
    current_task = gdb.parse_and_eval('current_task')
    pass

try:
    gdb.execute(f'call block_task({int(sleeping_state)}, 1)')
except gdb.error:
    pass

assert(current_task['sleep_until'] == ((seconds * 100) + tick))
assert(current_task['state'] == sleeping_state)

print(f'\033[92m[+] MULTITASKING sleep({seconds}) executado corretamente.\033[0m')

gdb.execute('delete 6')


utils.GdbStop()

'''
jamesos.write_memory(pname, b'minha task')
idle_state = gdb.parse_and_eval('IDLE')
idle_task = gdb.parse_and_eval(f'create_task(0x1234, {pname}, {idle_state}, 1, 0x4321)')
kernel_directory = gdb.parse_and_eval('kernel_directory')
expected_ebp = last_kmalloc + 0x1000 - 0x4
expected_esp = expected_ebp - 28
assert(idle_task['ebp'] == expected_ebp)
assert(idle_task['esp'] == expected_esp)
assert(idle_task['page_dir'] == kernel_directory)
assert(idle_task['next_task'] == 0)
assert(idle_task['pid'] == 0)
assert(idle_task['state'] == idle_state)
assert(utils.TestMemoryRegion(b'minha task', idle_task['pname'].address))
print(f'\033[92m[+] MULTITASKING create_task(0x1234, {pname}, {idle_state}, 1, 0x4312) executado corretamente.\033[0m')

jamesos.write_memory(pname, b'custom task')
custom_state = gdb.parse_and_eval('READY_TO_RUN')
custom_task = gdb.parse_and_eval(f'create_task(0x1234, {pname}, {custom_state}, 1, 0x4321)')
kernel_directory = gdb.parse_and_eval('kernel_directory')
expected_ebp = last_kmalloc + 0x2000 - 0x4
expected_esp = expected_ebp - 28
ready_to_run_counter = gdb.parse_and_eval('ready_to_run_counter')
assert(custom_task['ebp'] == expected_ebp)
assert(custom_task['esp'] == expected_esp)
assert(custom_task['page_dir'] == kernel_directory)
assert(custom_task['next_task'] == 0)
assert(custom_task['pid'] == 2)
assert(custom_task['state'] == custom_state)
assert(utils.TestMemoryRegion(b'custom task', custom_task['pname'].address))
assert(ready_to_run_counter == 0x1)
print(f'\033[92m[+] MULTITASKING create_task(0x1234, {pname}, {custom_state}, 1, 0x4312) executado corretamente.\033[0m')
'''
