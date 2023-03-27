ARCH=$(findstring CYGWIN, $(shell uname -s))

ifeq ($(ARCH), CYGWIN)
	ROOT_PATH = $(shell cygpath -w $(CURDIR))
else
	ROOT_PATH = $(CURDIR)
endif

MODULES = kernel
BOCHSCON_PATH = $(ROOT_PATH)/test/bochsrc.bxrc

BUILD_PATH = $(ROOT_PATH)/build
OBJ_PATH = $(BUILD_PATH)/temp
BIN_PATH = $(BUILD_PATH)/bin
LIB_PATH = $(ROOT_PATH)/lib
INCLUDE_PATH = $(ROOT_PATH)/include
PATH_CHECK = $(BIN_PATH)

LEADING_FILES = $(LIB_PATH)/crt0.o $(LIB_PATH)/crti.o $(LIB_PATH)/crtbegin.o
TRAILING_FILES = $(LIB_PATH)/crtend.o $(LIB_PATH)/crtn.o

CC = x86_64-elf-gcc
AS = x86_64-elf-as
AR = x86_64-elf-ar
LD = x86_64-elf-ld

CXXFLAGS = -c -O2 -mno-sse -mno-sse2 -mno-mmx -mcmodel=kernel -ffreestanding -mno-red-zone -fno-rtti -fno-exceptions -std=c++17
LDFLAGS = -nostdlib -lgcc -L./lib

export ROOT_PATH BUILD_PATH OBJ_PATH BIN_PATH LIB_PATH INCLUDE_PATH
export ARCH CC AS AR LD
export CXXFLAGS

OBJS = $(foreach module, $(MODULES), ${wildcard $(OBJ_PATH)/$(module)/*.o})

all: $(MODULES)
	@make link

.PHONY: $(MODULES)
$(MODULES): 
	make -C $(ROOT_PATH)/$@

link: $(PATH_CHECK)
	$(LD) $(LDFLAGS) -T link.lds $(LEADING_FILES) $(OBJS) $(TRAILING_FILES) -o $(BIN_PATH)/kernel

$(PATH_CHECK):
	mkdir -p $@

.PHONY:run
run:
	@bochsdbg -f $(BOCHSCON_PATH) -q
