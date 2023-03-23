ARCH=$(findstring CYGWIN, $(shell uname -s))

ifeq ($(ARCH), CYGWIN)
	TOP_PATH = $(shell cygpath -w $(CURDIR))
else
	TOP_PATH = $(CURDIR)
endif

OUTPUT_PATH = $(TOP_PATH)/..

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = x86_64-elf-as

all: crts

crts: crt0 crti crtn

crt0: crt0.s
	$(AS) -c $< -o $(OUTPUT_PATH)/crt0.o

crti: crti.s
	$(AS) -c $< -o $(OUTPUT_PATH)/crti.o

crtn: crtn.s
	$(AS) -c $< -o $(OUTPUT_PATH)/crtn.o
