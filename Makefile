C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c network/*.c)
HEADERS = $(wildcard include/*.h)

# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

CC = i686-elf-gcc
GDB = i686-elf-gdb

# -g: Use debugging symbols in gcc
CFLAGS = -g

# First rule is run by default
os-image.bin: boot/boot_sector.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: kernel/kernel_entry.o ${OBJ} kernel/kernel_end.o
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

run: os-image.bin
	#qemu-system-i386 -hda hd -fda os-image.bin
	#qemu-system-i386 -hda hd -netdev tap,helper=/usr/lib/qemu/qemu-bridge-helper,id=os_net0 -device model=rtl8139,netdev=os_net0,id=os_nic0 -fda os-image.bin
	#qemu-system-i386 -net nic -nic model=rtl8139 -fda os-image.bin
	#sudo qemu-system-x86_64 -cdrom archlinux-2020.02.01-x86_64.iso -boot d -enable-kvm  -m 1024 -hda hd -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no -device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01
	sudo qemu-system-i386 -fda os-image.bin -netdev tap,id=mynet0,ifname=tap0,script=no,downscript=no -device rtl8139,netdev=mynet0,mac=52:55:00:d1:55:01 -object filter-dump,id=f1,netdev=mynet0,file=dump.dat

## Used for debugging purposes
#kernel.elf: boot/kernel_entry.o ${OBJ}
#	i686-elf-ld -o $@ -Ttext 0x1000 $^ 
#
## Open the connection to qemu and load our kernel-object file with symbols
#debug: os-image.bin kernel.elf
#	qemu-system-i686 -s -fda os-image.bin -d guest_errors,int &
#	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o network/*.o
