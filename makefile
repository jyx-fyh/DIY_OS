all:mbr.bin loader.bin allInDisk

mbr.bin:mbr.s
	nasm -f bin mbr.s -o mbr.bin
loader.bin:loader.s
	nasm -f bin loader.s -o loader.bin
hd.img:
	bximage -q -hd=16 -func=create -sectsize=512 -imgmode=flat ./hd.img
loaderInDisk:hd.img
	dd if=loader.bin of=hd.img bs=512 seek=2 conv=notrunc
mbrInDisk:hd.img
	dd if=mbr.bin of=hd.img bs=512  conv=notrunc
allInDisk:mbrInDisk loaderInDisk
bochs:
	bochs -q -f bochsrc
clean:
	rm -rf *.bin *.img
