TARGET = kernel.elf
OBJS = main.o c_graphics.o font.o hankaku.o framebuffer.o operator.o global.o \
	   newlib_support.o c_console.o mouse.o printk.o pci.o logger.o
#	   xhc.o
DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

WORKDIR=../..
LIBC_DIR=$(WORKDIR)/tools/x86_64-elf
EDK2_DIR=$(WORKDIR)/tools/edk2
MKFONT_CMD=$(WORKDIR)/tools/mkfont/makefont.py
SOURCE_DIR=${WORKDIR}/src

CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
CPPFLAGS += -I$(LIBC_DIR)/include/c++/v1 -I$(LIBC_DIR)/include -I$(LIBC_DIR)/include/freetype2 \
            -I$(EDK2_DIR)/MdePkg/Include -I$(EDK2_DIR)/MdePkg/Include/X64 \
			-I$(SOURCE_DIR)/bootloader/Include \
			-I$(SOURCE_DIR)/kernel/include \
			-nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS\
			 -DEFIAPI='__attribute__((ms_abi))'
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	        -fno-exceptions -fno-rtti -std=c++17
LDFLAGS += -L${LIBC_DIR}/lib --entry KernelMain -z norelro -z separate-code --image-base 0x100000  --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o .*.d *.bin kernel.elf

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS)  -o kernel.elf $(OBJS) -lc

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< 

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@
	mv $@ ./depends

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $<

.%.d: %.c
	clang $(CPPFLAGS) $(CFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.c=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@
	mv $@ ./depends

hankaku.bin: font/hankaku.txt
	$(MKFONT_CMD) -o $@ $<

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

.%.d: %.bin
	touch $@
	mv $@ ./depends


.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

#-include $(DEPENDS)
