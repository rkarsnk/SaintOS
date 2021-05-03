# Makefile for StOS
SHELL=/bin/bash
WORKDIR=$(shell pwd)
SOURCEDIR=${WORKDIR}/StOS
EDK2DIR=${WORKDIR}/edk2


OVMFBASE=edk2/Build/OvmfX64/DEBUG_CLANG38
OVMFCODE=${OVMFBASE}/FV/OVMF_CODE.fd
OVMFVARS=${OVMFBASE}/FV/OVMF_VARS.fd
QEMUDISK=disk.img
QEMU=qemu-system-x86_64
QEMUFLAGS=-m 1G \
	  -drive if=pflash,format=raw,readonly,file=$(OVMFCODE) \
	  -drive if=pflash,format=raw,file=$(OVMFVARS) \
	  -drive if=ide,index=0,media=disk,format=raw,file=${QEMUDISK} \
	  -device nec-usb-xhci,id=xhci \
	  -device usb-mouse -device usb-kbd \
	  -debugcon file:debug.log -global isa-debugcon.iobase=0x402 \
	  -monitor stdio 

MOUNTPOINT=mnt


.PHONY: all 
all: Loader kernel

.PHONY: clean
clean: 
	make -C ${SOURCEDIR}/kernel clean WORKDIR=${WORKDIR}
	rm -rf edk2/Build/StOSLoaderX64
	rm -rf edk2/StOSLoaderPkg
	rm -rf Loader.*
	rm -rf kernel.elf
	rm -rf disk.img
	rm -rf debug.log

# build Loader.efi
.PHONY: Loader
Loader: edk2-c-tools StOSLoaderPkg
	cd ${EDK2DIR}; source edksetup.sh --reconfig;\
	build -p StOSLoaderPkg/StOSLoaderPkg.dsc -b DEBUG -a X64 -t CLANG38
	cp ${EDK2DIR}/Build/StOSLoaderX64/DEBUG_CLANG38/X64/Loader.efi ./Loader.efi
	cp ${EDK2DIR}/Build/StOSLoaderX64/DEBUG_CLANG38/X64/Loader.debug ./Loader.debug

.PHONY: StOSLoaderPkg
StOSLoaderPkg: ${SOURCEDIR}/StOSLoaderPkg 
	ln -s ${SOURCEDIR}/StOSLoaderPkg ${EDK2DIR}

# build OVMF file
ovmf: edk2-c-tools
	cd ${EDK2DIR}; source edksetup.sh --reconfig;\
	build -p OvmfPkg/OvmfPkgX64.dsc -b DEBUG -a X64 -t CLANG38 \

# build EDK2 C TOOL
.PHONY: edk2-c-tools
edk2-c-tools: Makefile ${EDK2DIR}
	make -C ${EDK2DIR}/BaseTools/Source/C

# build kernel.elf 
.PHONY: kernel
kernel: ${SOURCEDIR}/kernel
	make -C $< all WORKDIR=${WORKDIR}
	cp ${SOURCEDIR}/kernel/kernel.elf ./kernel.elf

# create disk.img
disk.img: Loader.efi kernel.elf 
	rm -rf $@
	qemu-img create -f raw ${QEMUDISK} 200M
	mkfs.fat -n 'StOS' -s 2 -f 2 -R 32 -F 32 ${QEMUDISK}
	mkdir -p ${MOUNTPOINT} 
	sudo mount -o loop ${QEMUDISK} ${MOUNTPOINT}
	sleep 0.5
	sudo mkdir -p ${MOUNTPOINT}/EFI/BOOT
	sudo cp Loader.efi ${MOUNTPOINT}/EFI/BOOT/BOOTX64.EFI
	sudo cp kernel.elf ${MOUNTPOINT}/
	sleep 0.5
	sudo umount ${MOUNTPOINT}
	sleep 0.5
	rmdir ${MOUNTPOINT}


# QEMU
.PHONY: run debug
run: disk.img	
	sudo $(QEMU) $(QEMUFLAGS) -s
debug: disk.img
	sudo $(QEMU) $(QEMUFLAGS) -s -S

# .PHONY: x86_64-elf
# x86_64-elf: Dockerfile

.PHONY: edk2
edk2: Makefile
	git clone --depth 1 --recursive -b edk2-stable202102 https://github.com/tianocore/edk2.git  ${EDK2DIR}

.PHONY: osbook
osbook: Makefile
	git clone https://github.com/uchan-nos/mikanos-build.git osbook

