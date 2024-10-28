import gdb
import os
import sys
sys.path.append(os.path.dirname('..'))
import james_utils

utils = james_utils.JamesUtils()
utils.GdbInit()

## TEST isr.c

### CLEAR_SCREEN
gdb.execute('call clear_screen()')

### KHEAP_INIT
gdb.execute('call kheap_init()')
software_interrupt = gdb.parse_and_eval('kmalloc_u(2)')

### ISR_INSTALL
gdb.execute('call isr_install()')
for i in range(0, 32):
    isr = gdb.parse_and_eval(f'isr{i}')
    low_offset = gdb.parse_and_eval(f'idt[{i}].low_offset')
    high_offset = gdb.parse_and_eval(f'idt[{i}].high_offset')
    handler = int(low_offset) + (int(high_offset) << 16)
    assert(handler == isr)
    print(f'\033[92m[+] CPU isr_handler {i} instalado corretamente.\033[0m')

for i in range(32, 48):
    irq = gdb.parse_and_eval(f'irq{i - 32}')
    low_offset = gdb.parse_and_eval(f'idt[{i}].low_offset')
    high_offset = gdb.parse_and_eval(f'idt[{i}].high_offset')
    handler = int(low_offset) + (int(high_offset) << 16)
    assert(handler == irq)
    print(f'\033[92m[+] CPU irq_handler {i - 32} instalado corretamente.\033[0m')


### BREAKPOINTS
gdb.execute('break isr_handler')
gdb.execute('break irq_handler')
gdb.execute(f'break *{software_interrupt + 2}')

### COMMON INTERRUPT VALIDATOR
def validate_interrupt_handlers(interrupt):
    gdb.execute(f'set $pc = {software_interrupt}')
    gdb.execute(f'set *{software_interrupt} = 0x{interrupt:x}cd')
    gdb.execute('continue')
    interrupt_number = gdb.parse_and_eval('r.int_no')
    #error_code = gdb.parse_and_eval('r.err_code')
    assert(interrupt_number == interrupt)
    #assert(error_code == 0x0)
    print(f'\033[92m[+] CPU interrupcao 0 executada corretamente.\033[0m')
    gdb.execute('continue')

### ISR_HANDLER
validate_interrupt_handlers(0)

### IRQ_HANDLER
validate_interrupt_handlers(40)


utils.GdbStop()
