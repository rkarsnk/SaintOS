TARGET = kernel.elf
OBJS = main.o font.o framebuffer.o operator.o logger.o \
	   lib/newlib_support.o lib/libcxx_support.o \
	   c_graphics.o c_console.o c_mouse.o \
	   pci.o interrupt.o segment.o pageing.o mmgr.o\
	   layer.o window.o \
	   hankaku.o \
	   asmfunc.o \
	   kernel_entry.o \
	   usb/memory.o usb/device.o \
	   usb/xhci/ring.o usb/xhci/trb.o usb/xhci/xhci.o \
       usb/xhci/port.o usb/xhci/device.o usb/xhci/devmgr.o usb/xhci/registers.o \
       usb/classdriver/base.o usb/classdriver/hid.o usb/classdriver/keyboard.o \
       usb/classdriver/mouse.o

DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

WORKDIR=../..
LIBC_DIR=$(WORKDIR)/tools/x86_64-elf
EDK2_DIR=$(WORKDIR)/tools/edk2
MKFONT_CMD=$(WORKDIR)/tools/mkfont/makefont.py
SOURCE_DIR=${WORKDIR}/src

CPPFLAGS += -I$(LIBC_DIR)/include/c++/v1 -I$(LIBC_DIR)/include -I$(LIBC_DIR)/include/freetype2 \
            -I$(EDK2_DIR)/MdePkg/Include -I$(EDK2_DIR)/MdePkg/Include/X64 \
			-I$(SOURCE_DIR)/bootloader/Include \
			-I$(SOURCE_DIR)/kernel/include \
			-nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS\
			 -DEFIAPI='__attribute__((ms_abi))'
CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	        -fno-exceptions -fno-rtti -std=c++17
LDFLAGS += -L${LIBC_DIR}/lib --entry KernelMain -z norelro -z separate-code --image-base 0x100000  --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJS) .*.d *.bin kernel.elf

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS)  -o kernel.elf $(OBJS) -lc -lc++ -lc++abi

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@
	mv $@ ./depends

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.%.d: %.c
	clang $(CPPFLAGS) $(CFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.c=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@
	mv $@ ./depends

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

hankaku.bin: font/hankaku.txt
	$(MKFONT_CMD) -o $@ $<

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
	rm -rf $<

.%.d: %.bin
	touch $@
	mv $@ ./depends


.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

#-include $(DEPENDS)
