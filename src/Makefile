# list of c source files
C_SOURCES = $(wildcard kcore/*.c libc/*.c drivers/*.c cpu/*.c multitask/*.c kernel_tasks/*.c network/*.c) 
# header files
HEADERS = $(wildcard include/*.h)
# list ofobject files to create
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o multitask/dispatcher.o libc/print_stack_asm.o} 


# set cross compiler
CC_BASE_PATH = ./scripts/i686-elf/opt/cross/bin/
CC = $(CC_BASE_PATH)i686-elf-gcc 
# default flags
CFLAGS= -ffreestanding -c -I include -nostdlib -lgcc -w -ggdb3
# set gdb
GDB = i686-elf-gdb


# run the os
run: os-image.bin
	sudo qemu-system-i386 -drive format=raw,file=os-image.bin,index=0,if=floppy -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no -device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01 -object filter-dump,id=f1,netdev=mynet0,file=dump.dat -serial stdio -qmp tcp:0.0.0.0:4444,server=on,wait=off -s -S

# build the image
build: os-image.bin

# creating the os-image.bin
os-image.bin: boot/boot_sector.bin kcore/kernel_main.bin 
	cat $^ > os-image.bin

# creating the binary kernel
kcore/kernel_main.bin: kcore/kernel_entry.o ${OBJ}
	$(CC_BASE_PATH)i686-elf-ld -z muldefs -o $@ -Ttext 0x20000 $^ --oformat binary 
	$(CC_BASE_PATH)i686-elf-ld -z muldefs -o $@.elf -Ttext 0x20000 $^

# compiling c source codes
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} $< -o $@ 

# creating rules for creating binary files
%.bin: %.asm 
	nasm $< -f bin -o $@

# creating rules for creating ELF files
%.o: %.asm 
	nasm $< -f elf -o $@

# cleaning binary/ELF files
clean: 
	rm -rf *.bin *.dat boot/*.bin kcore/*.bin
	rm -rf kcore/*.o libc/*.o drivers/*.o cpu/*.o multitask/*.o kernel_tasks/*.o network/*.o
	rm -rf kcore/*.013t.cfg libc/*.013t.cfg drivers/*.013t.cfg cpu/*.013t.cfg multitask/*.013t.cfg kernel_tasks/*.013t.cfg network/*.013t.cfg
	rm -rf kcore/*.013t.cfg.dot libc/*.013t.cfg.dot drivers/*.013t.cfg.dot cpu/*.013t.cfg.dot multitask/*.013t.cfg.dot kernel_tasks/*.013t.cfg.dot network/*.013t.cfg.dot
	rm -rf kcore/*.elf
