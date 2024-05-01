SRC_DIR     = src
BUILD_DIR   = build
BOOT_PATH   = boot/boot
KERNEL_PATH = kernel/kernel
LOADER_PATH = boot/kernel_loader
DISK_IMG    = ${BUILD_DIR}/disk.img


define qemu
	qemu-system-x86_64 \
	-drive format=raw,file=${DISK_IMG} \
	$1
endef

OPT ?= 0
qemu: dd
	clear;	
	if [ $(OPT) = 0 ];\
	then $(call qemu,);\
	else $(call qemu,-gdb tcp::3007 -S);\
	fi	


fdisk:
	clear;\
	fdisk -l ${DISK_IMG};\
	echo "\n"; hd ${DISK_IMG};


dd: ld
	echo "\t------------------";\
	dd if=/dev/zero of=${DISK_IMG} bs=1M  count=4;\
	dd if=${BUILD_DIR}/${BOOT_PATH}.bin   of=${DISK_IMG} \
	   bs=512 count=1 seek=0 conv=notrunc;\
	dd if=${BUILD_DIR}/${KERNEL_PATH}.bin of=${DISK_IMG} \
	   bs=512 count=1 seek=1 conv=notrunc;


ld: gcc fasm
	ld -m elf_i386 -o ${BUILD_DIR}/${KERNEL_PATH}.bin -Ttext 0x1000 \
	   ${BUILD_DIR}/${LOADER_PATH}.o ${BUILD_DIR}/${KERNEL_PATH}.o \
	   --oformat binary


gcc:
	gcc -fno-pie -ffreestanding -m32 \
    -c ${SRC_DIR}/${KERNEL_PATH}.c \
    -o ${BUILD_DIR}/${KERNEL_PATH}.o


define fasm
	fasm2 ${SRC_DIR}/$1.asm ${BUILD_DIR}/$1.$2
endef

fasm:
	echo "\t------------------";\
	$(call fasm,${BOOT_PATH},bin);\
	$(call fasm,${LOADER_PATH},o);


rm:
	find . -regex '.*\.\(o\|bin\|img\)$$' -type f -delete
