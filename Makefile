# Makefile for StOS
SHELL=/bin/bash
WORKDIR=$(shell pwd)
SOURCEDIR=${WORKDIR}/StOS
BASEDIR=${WORKDIR}/devenv/x86_64-elf
EDK2DIR=${WORKDIR}/edk2
CPPFLAGS += -I${BASEDIR}/include/c++/v1 -I${BASEDIR}/include -I${BASEDIR}/include/freetype2 \
            -I${EDK2DIR}/MdePkg/Include -I${EDK2DIR}/MdePkg/Include/X64 -nostdlibinc \
             -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS -DEFIAPI='__attribute__((ms_abi))'
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	        -fno-exceptions -fno-rtti -std=c++17
LDFLAGS += -L${BASEDIR}/lib


kernel:
	cd ${SOURCEDIR}/$@; clang++ ${CPPFLAGS} ${CXXFLAGS} -c main.cpp ;\
	ld.lld ${LDFLAGS} --entry KernelMain -z norelro --image-base 0x100000 --static -o kernel.elf main.o

# checkout mikanos-build
osbook:
	git clone https://github.com/uchan-nos/mikanos-build.git $@


# checkout edk2-stable202102
edk2:
	git clone --recursive https://github.com/tianocore/edk2.git -b edk2-stable202102 $@
	make -C edk2/BaseTools/Source/C

edk2/StOSLoaderPkg: edk2 StOS/StOSLoaderPkg
	cd edk2; ln -s ../StOS/StOSLoaderPkg ./

StOSLoaderPkg: edk2 edk2/StOSLoaderPkg Loader.patch
	cd $<; source edksetup.sh --reconfig;\
	patch -n Conf/target.txt < ../Loader.patch; \
	build

# create empty qemu disk image
disk.img:
	qemu-img create -f raw $@ 200M
	mkfs.fat -n 'STOS' -s 2 -f 2 -R 32 -F 32 $@


clean:
	rm -rf disk.img
	rm -rf edk2/Build
	rm -rf edk2/StOSLoaderPkg
	rm -rf StOS/kernel/main.o


clean_all:
	rm -rf edk2
	rm -rf osbook

