# list of c source files
C_SOURCES = $(wildcard kernel/*.c libc/*.c drivers/*.c cpu/*.c multitask/*.c kernel_tasks/*.c network/*.c) 
# header files
HEADERS = $(wildcard include/*.h)
# list ofobject files to create
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o multitask/task_switch.o libc/print_stack_asm.o} 

# set cross compiler
CC = i686-elf-gcc 
# default flags
CFLAGS= -g -ffreestanding -c -I include -nostdlib
# set gdb
GDB = i686-elf-gdb


# run the os
run: os-image.bin
	sudo qemu-system-i386 -fda os-image.bin -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no -device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01 -object filter-dump,id=f1,netdev=mynet0,file=dump.dat

# build the image
build: os-image.bin

# creating the os-image.bin
os-image.bin: boot/boot_sector.bin kernel/kernel_main.bin 
	cat $^ > os-image.bin  

# creating the binary kernel
kernel/kernel_main.bin: kernel/kernel_entry.o ${OBJ}
	i686-elf-ld -z muldefs -o $@ -Ttext 0x1000 $^ --oformat binary 

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
	rm -rf *.bin *.dat boot/*.bin kernel/*.bin
	rm -rf kernel/*.o libc/*.o drivers/*.o cpu/*.o multitask/*.o kernel_tasks/*.o network/*.o
