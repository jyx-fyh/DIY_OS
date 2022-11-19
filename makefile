BUILD= ./build
SRC=./src
INC=./inc
AS=nasm
DISK=hd.img
vpath %.s $(SRC)
vpath %.inc $(INC)
vpath %.bin $(BUILD)
$(BUILD)/loader.bin: ASFLAGS= -p $(INC)/loader.inc -p $(INC)/boot.inc -f bin
$(BUILD)/mbr.bin: ASFLAGS= -p $(INC)/loader.inc -f bin

CFLAGS:= -m32 # 32 位的程序
CFLAGS+= -masm=intel
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
CFLAGS+= -nostdinc		# 不需要标准头文件
CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
CFLAGS:=$(strip $(CFLAGS))

DEBUG:= -g

all: hd.img

$(BUILD)/%.bin : %.s
	$(AS) $(ASFLAGS) $^ -o $@

./$(DISK):$(BUILD)/mbr.bin $(BUILD)/loader.bin $(BUILD)/pure_kernel.bin
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@
	dd if=$(BUILD)/mbr.bin of=$@ bs=512  conv=notrunc
	dd if=$(BUILD)/loader.bin of=$@ bs=512  seek=2 conv=notrunc
	dd if=$(BUILD)/pure_kernel.bin of=$@ bs=512 seek=50 count=200 conv=notrunc

$(BUILD)/pure_kernel.bin: $(BUILD)/kernel.bin
	objcopy -O binary $(BUILD)/kernel.bin $@

$(BUILD)/kernel.bin: $(BUILD)/guide.o $(BUILD)/main.o
	ld  -m elf_i386 $^ -o $@ -Ttext 0x00001500

$(BUILD)/main.o: ./kernel/main.c
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

$(BUILD)/guide.o: $(SRC)/guide.s
	nasm -f elf32 -g $< -o $@

bochs:
	$(RM) ./*.lock
	bochs -q -f bochsrc

clean:
	rm -rf $(BUILD)/*.bin  $(DISK) $(BUILD)/*.o

qemu:
	qemu-system-i386 \
	-m 32M \
	-boot c \
	-hda ./hd.img

qemug: hd.img
	qemu-system-x86_64 -m 32M -hda ./hd.img -S -s