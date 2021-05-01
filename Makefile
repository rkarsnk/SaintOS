# Makefile for StOS
SHELL=/bin/bash
WORKDIR=$(shell pwd)
SOURCEDIR=${WORKDIR}/StOS
EDK2DIR=${WORKDIR}/edk2

LOADER_PATCH=${WORKDIR}/Loader.patch

.PHONY: all
all: Loader kernel

.PHONY: clean
clean: 
	make -C ${SOURCEDIR}/kernel clean WORKDIR=${WORKDIR}
	rm -rf edk2/Build/*
	rm -rf edk2/StOSLoaderPkg



# build Loader.efi
.PHONY: Loader
Loader: edk2-c-tools StOSLoaderPkg Loader.patch
	cd ${EDK2DIR}; source edksetup.sh --reconfig;\
	patch -n Conf/target.txt < ${LOADER_PATCH}; \
	build

.PHONY: StOSLoaderPkg
StOSLoaderPkg: edk2-c-tools
	ln -s ${SOURCEDIR}/StOSLoaderPkg ${EDK2DIR}/StOSLoaderPkg

.PHONY: edk2-c-tools
edk2-c-tools: Makefile ${EDK2DIR}
	make -C ${EDK2DIR}/BaseTools/Source/C

# build kernel.elf 
.PHONY: kernel
kernel: ${SOURCEDIR}/kernel
	make -C $< all WORKDIR=${WORKDIR}

# .PHONY: x86_64-elf
# x86_64-elf: Dockerfile

.PHONY: edk2
edk2: Makefile
	git clone --depth 1 --recursive -b edk2-stable202102 https://github.com/tianocore/edk2.git  ${EDK2DIR}

.PHONY: osbook
osbook: Makefile
	git clone https://github.com/uchan-nos/mikanos-build.git osbook
