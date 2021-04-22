# Makefile for StOS

edk2:
	git clone --recursive https://github.com/tianocore/edk2.git $@
	cd $@; git checkout edk2-stable202102
	cd $@; make -C BaseTools/Source/C
