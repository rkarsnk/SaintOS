TARGET = kernel.elf
OBJS = main.o
WORKDIR=../..
BASEDIR=${WORKDIR}/devenv/x86_64-elf
EDK2DIR=${WORKDIR}/edk2

CPPFLAGS += -I${BASEDIR}/include/c++/v1 -I${BASEDIR}/include -I${BASEDIR}/include/freetype2 \
            -I${EDK2DIR}/MdePkg/Include -I${EDK2DIR}/MdePkg/Include/X64 \
			-I${WORKDIR}/StOS/Include \
			-nostdlibinc  -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS\
			 -DEFIAPI='__attribute__((ms_abi))'
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	        -fno-exceptions -fno-rtti -std=c++17
LDFLAGS += -L${BASEDIR}/lib --entry KernelMain -z norelro -z separate-code --image-base 0x100000  --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o kernel.elf

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS)  -o kernel.elf $(OBJS)

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< 


