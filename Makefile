all:os-image

os-image:boot/boot_sector.bin kernel/kernel.bin
	cat $^ > $@

kernel/kernel.bin:kernel/kernel_entry.o kernel/kernel.o libc/mem.o libc/string.o drivers/ports.o drivers/screen.o cpu/idt.o cpu/isr.o cpu/interrupt.o cpu/timer.o drivers/keyboard.o libc/memory_manager.o
	i686-elf-ld --oformat binary -Ttext 0x1000 -o $@ $^

libc/memory_manager.o:libc/memory_manager.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

drivers/keyboard.o:drivers/keyboard.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

cpu/timer.o:cpu/timer.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

cpu/interrupt.o:cpu/interrupt.asm
	nasm -f elf -o $@ $^

cpu/isr.o:cpu/isr.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

cpu/idt.o:cpu/idt.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

drivers/screen.o:drivers/screen.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

drivers/ports.o:drivers/ports.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

libc/string.o:libc/string.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie
libc/mem.o:libc/mem.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie


kernel/kernel.o:kernel/kernel.c
	i686-elf-gcc -ffreestanding -c -o $@ $^ #-fno-pie

kernel/kernel_entry.o:kernel/kernel_entry.asm
	nasm -f elf -o $@ $^


boot/boot_sector.bin:boot/boot_sector.asm
	nasm -f bin -o $@ $^

run:all
	truncate -s 1200K os-image
	qemu-system-i386 os-image

clear_kernel:
	rm kernel/*.bin kernel/*.o
clear_boot:
	rm boot/*.bin
clear_drivers:
	rm drivers/*.o

clear:
	rm -f kernel/*.bin kernel/*.o
	rm -f boot/*.bin
	rm -f drivers/*.o
	rm -f cpu/*.o
	rm -f libc/*.o
	rm -f os-image
