# Makefile for StOS
SHELL=/bin/bash
WORKDIR=$(CURDIR)
SOURCE_DIR=$(WORKDIR)/src
EDK2_DIR=$(WORKDIR)/tools/edk2

OVMF_BASE=$(EDK2_DIR)/Build/OvmfX64/DEBUG_CLANG38
OVMF_CODE=$(OVMF_BASE)/FV/OVMF_CODE.fd
OVMF_VARS=$(OVMF_BASE)/FV/OVMF_VARS.fd


.PHONY: all
all: Loader.efi kernel.elf

.PHONY: clean
clean: 
	make -C ${SOURCE_DIR}/kernel clean WORKDIR=${WORKDIR}
	rm -rf kernel.elf
	rm -rf $(EDK2_DIR)/LoaderPkg
	rm -rf $(EDK2_DIR)/Build/LoaderX64
	rm -rf disk.img
	rm -rf debug.log
	rm -rf Loader.*


# build kernel.elf
KERN_MAKEDIR=$(SOURCE_DIR)/kernel
KERN_MAKEFILE=$(KERN_MAKEDIR)/Makefile 
.PHONY: kernel
kernel:kernel.elf
kernel.elf: $(KERN_MAKEFILE)
	make -C $(KERN_MAKEDIR) all WORKDIR=${WORKDIR}
	cp ${SOURCE_DIR}/kernel/kernel.elf $(WORKDIR)/kernel.elf


#----------------------------------------------------------
# Build bootloader (Loader.efi)
#----------------------------------------------------------
.PHONY: Loader ovmf edk2tool
Loader: Loader.efi
Loader.efi: edk2tool $(EDK2_DIR)/edksetup.sh
	rm -rf $(EDK2_DIR)/LoaderPkg
	ln -s $(SOURCE_DIR)/bootloader $(EDK2_DIR)/LoaderPkg
	WORKSPACE=$(EDK2_DIR) source $(EDK2_DIR)/edksetup.sh ;\
		WORKSPACE=$(EDK2_DIR) build -p LoaderPkg/LoaderPkg.dsc -b DEBUG -a X64 -t CLANG38
	cp ${EDK2_DIR}/Build/LoaderX64/DEBUG_CLANG38/X64/Loader.efi ./Loader.efi
	cp ${EDK2_DIR}/Build/LoaderX64/DEBUG_CLANG38/X64/Loader.debug ./Loader.debug

# build OVMF file
ovmf: edk2tool $(EDK2_DIR)/edksetup.sh
	WORKSPACE=$(EDK2_DIR) source $(EDK2_DIR)/edksetup.sh --reconfig;\
		WORKSPACE=$(EDK2_DIR) build -p OvmfPkg/OvmfPkgX64.dsc -b DEBUG -a X64 -t CLANG38

# build EDK2 C-lang tools
.PHONY: edk2tool
edk2tool: $(EDK2_DIR)/BaseTools/Source/C/Makefile
	make -C $(EDK2_DIR)/BaseTools/Source/C


#----------------------------------------------------------
# QEMU
#----------------------------------------------------------
QEMU_IMG=disk.img
QEMU_MOUNT=$(WORKDIR)/mnt
QEMU_FAT=$(WORKDIR)/esp
QEMU=qemu-system-x86_64
QEMUFLAGS=-m 1G \
	  -drive if=pflash,format=raw,readonly,file=$(OVMF_CODE) \
	  -drive if=pflash,format=raw,file=$(OVMF_VARS) \
	  -drive if=ide,index=0,media=disk,format=raw,file=$(QEMU_IMG) \
	  -device nec-usb-xhci,id=xhci \
	  -device usb-mouse -device usb-kbd \
	  -debugcon file:debug.log -global isa-debugcon.iobase=0x402 \
	  -monitor stdio 

# create disk.img
disk.img: Loader.efi kernel.elf 
	rm -rf $@
	qemu-img create -f raw ${QEMU_IMG} 200M
	mkfs.fat -n 'StOS' -s 2 -f 2 -R 32 -F 32 ${QEMU_IMG}
	mkdir -p ${QEMU_MOUNT} 
	sudo mount -o loop ${QEMU_IMG} ${QEMU_MOUNT}
	sleep 0.5
	sudo mkdir -p ${QEMU_MOUNT}/EFI/BOOT
	sudo cp Loader.efi ${QEMU_MOUNT}/EFI/BOOT/BOOTX64.EFI
	sudo cp kernel.elf ${QEMU_MOUNT}/
	sleep 0.5
	sudo umount ${QEMU_MOUNT}
	sleep 0.5
	rmdir ${QEMU_MOUNT}

# QEMU
.PHONY: run debug
run: disk.img ovmf	
	sudo $(QEMU) $(QEMUFLAGS) -s
debug: disk.img ovmf
	sudo $(QEMU) $(QEMUFLAGS) -s -S


.PHONY: prep
prep: Makefile
	git clone --depth 1 --recursive -b edk2-stable202102 https://github.com/tianocore/edk2.git  ${EDK2_DIR}
	