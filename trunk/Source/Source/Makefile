#makefile

FINAL	= bin/kernel.bin
IMAGE	= bin/phoenix-os-onyx.img
START	= start.so
OBJS	=*.o
CC	=tools/gcc
LD	=tools/ld
AS	=tools/nasm
dd = cp
EMU = "c:/program files/Bochs-2.3/"

CCFLAGS	= -O -w -fno-builtin -nostdlib -ffreestanding -I ./include -c 
LDFLAGS	= -Map map.txt -T link.ld -o $(FINAL) $(START) $(OBJS)
ASFLAGS = -f elf 

all: compile clean run

check:
	vfd remove #make sure floppy was removed	
	
compile: *.c
	#Compile source
	$(CC) $(CCFLAGS) *.c
	$(CC) $(CCFLAGS) */*.c
	#Assembly
	$(AS) $(ASFLAGS) asm/inc.asm -o inc.o
	$(AS) $(ASFLAGS) asm/start.asm -o start.so
	#Link
	$(LD) -Ttext 0x1000 -o bin/api.lib $(OBJS)
	$(LD) $(LDFLAGS)
	
clean:
	rm *.o
	rm *.so
		
run:
	vfd open $(IMAGE)
	$(CP) if=$(FINAL) of=a:/boot/kernel.bin
	vfd remove
	$(CP) if=$(IMAGE) of="C:/program Files/Bochs-2.3/phoenix-os-onyx.img"
	$(EMU)bochs -q
	

	