boot_path = src/boot

qemu-run: fasm
	qemu-system-x86_64 \
	-drive format=raw,file=$(boot_path)

qemu-gdb: fasm
	qemu-system-x86_64 \
	-drive format=raw,file=$(boot_path) \
	-gdb tcp::3007 -S

hd: fasm
	hd $(boot_path)

fasm:
	fasm2 $(boot_path).asm $(boot_path)
