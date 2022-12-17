BUILD= ./build
AS=nasm
DISK=hd.img
KERNEL=build/guide.o  build/print.o  build/main.o build/interrupt.o build/idt.o build/port_io.o \
       build/timer.o  build/intrmgr.o  build/debug.o  build/string.o  build/memory.o  build/bitmap.o \
	   build/thread.o
$(BUILD)/loader.bin:      ASFLAGS= -p ./boot/inc/loader.inc -p ./boot/inc/boot.inc -f bin
$(BUILD)/mbr.bin:         ASFLAGS= -p ./boot/inc/loader.inc -f bin
$(BUILD)/guide.o:         ASFLAGS= -f elf32 -g
$(BUILD)/print.o:         ASFLAGS= -f elf32 -g
$(BUILD)/interrupt.o:     ASFLAGS= -f elf32 -g
$(BUILD)/port_io.o:   	  ASFLAGS= -f elf32 -g
$(BUILD)/timer.o:         ASFLAGS= -f elf32 -g


CFLAGS:= -m32 # 32 位的程序
CFLAGS+= -masm=intel
CFLAGS+= -fno-builtin	# 不需要 gcc 内置函数
CFLAGS+= -nostdinc		# 不需要标准头文件
CFLAGS+= -fno-pic		# 不需要位置无关的代码  position independent code
CFLAGS+= -fno-pie		# 不需要位置无关的可执行程序 position independent executable
CFLAGS+= -nostdlib		# 不需要标准库
CFLAGS+= -fno-stack-protector	# 不需要栈保护
#CFLAGS+= -DNDEBUG      # release模式

DEBUG:= -g

all: $(DISK)

$(BUILD)/%.bin : ./boot/asm/%.s
	$(AS) $(ASFLAGS) $^ -o $@

./$(DISK):$(BUILD)/mbr.bin $(BUILD)/loader.bin $(BUILD)/pure_kernel.bin
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@
	dd if=$(BUILD)/mbr.bin of=$@ bs=512  conv=notrunc
	dd if=$(BUILD)/loader.bin of=$@ bs=512  seek=2 conv=notrunc
	dd if=$(BUILD)/pure_kernel.bin of=$@ bs=512 seek=50 count=200 conv=notrunc

$(BUILD)/pure_kernel.bin: $(BUILD)/kernel.bin
	objcopy -O binary $(BUILD)/kernel.bin $@

$(BUILD)/kernel.bin: $(KERNEL)
	ld  -m elf_i386 $^ -o $@ -Ttext 0x00001500

$(BUILD)/main.o: kernel/init/main.c
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

$(BUILD)/%.o: kernel/src/%.c
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

$(BUILD)/%.o: ./kernel/asm/%.s
	nasm $(ASFLAGS) $< -o $@

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