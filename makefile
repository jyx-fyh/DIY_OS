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

all: hd.img

$(BUILD)/%.bin : %.s
	$(AS) $(ASFLAGS) $^ -o $@

./$(DISK):$(BUILD)/mbr.bin $(BUILD)/loader.bin
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat $@
	dd if=$(BUILD)/mbr.bin of=$@ bs=512  conv=notrunc
	dd if=$(BUILD)/loader.bin of=$@ bs=512  seek=2 conv=notrunc

bochs:
	$(RM) ./*.lock
	bochs -q -f bochsrc

clean:
	rm -rf $(BUILD)/*.bin  $(DISK)
