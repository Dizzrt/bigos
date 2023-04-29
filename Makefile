ARCH = $(shell uname -s)
ifeq ($(findstring CYGWIN, $(ARCH)), CYGWIN)
	ROOT_PATH = $(shell cygpath -w $(CURDIR))
else
	ROOT_PATH = $(CURDIR)
endif

# debug parms
ifeq ($(ARCH), Darwin)
# for macos
BOCHS_RUN = bochs
else
# for windows
BOCHS_RUN = bochsdbg
endif
BOCHSRC_PATH = $(ROOT_PATH)/test/bochsrc.bxrc

# compile parms
MODULES = kernel ktl drivers c++ mm irq

BUILD_PATH = $(ROOT_PATH)/build
OBJ_PATH = $(BUILD_PATH)/temp
BIN_PATH = $(BUILD_PATH)/bin
LIB_PATH = $(ROOT_PATH)/lib
INCLUDE_PATH = $(ROOT_PATH)/include $(ROOT_PATH)/c++/include $(ROOT_PATH)/c++/libsupc++/include $(ROOT_PATH)/drivers/include
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
	@printf "\033[32mdone\033[0m\n"

.PHONY: $(MODULES)
$(MODULES): 
	make -C $(ROOT_PATH)/$@

link: $(PATH_CHECK)
	$(LD) $(LDFLAGS) -T link.lds $(LEADING_FILES) $(OBJS) $(TRAILING_FILES) -o $(BIN_PATH)/kernel

$(PATH_CHECK):
	mkdir -p $@

.PHONY:run
run:
	@$(BOCHS_RUN) -f $(BOCHSRC_PATH) -q
