export SHELLOPTS := igncr

.PHONY : help all gnu arm clean
help:
	@echo "Valid targets:"
	@echo "   all   : build all projects with GCC and ARM RealView compiler"
	@echo "   gnu   : build all projects with GCC"
	@echo "   arm   : build all projects with ARM RealView compiler"
	@echo "   clean : delete all o-files, elf-files and bin-files"
	@echo "   usraccess.gnu : build named project 'usraccess.gnu.bin' with GNU compiler"
	@echo "   usraccess.arm : build named project 'usraccess.arm.bin' with ARM RealView compiler"
	@echo "   cp15.gnu      : build named project 'cp15.gnu.bin' with GNU compiler"
	@echo "   cp15.arm      : build named project 'cp15.arm.bin' with ARM RealView compiler"
all: gnu arm

gnu: usraccess.gnu cp15.gnu
arm: usraccess.arm cp15.arm

.PHONY: usraccess.arm usraccess.gnu
usraccess.gnu:
	$(MAKE) -f usraccess.mk gnu
usraccess.arm:
	$(MAKE) -f usraccess.mk arm

.PHONY: cp15.arm cp15.gnu
cp15.gnu:
	$(MAKE) -f cp15.mk gnu
cp15.arm:
	$(MAKE) -f cp15.mk arm

clean:
	$(RM) -fv *.o
	$(RM) -fv *.elf
	$(RM) -fv *.bin
