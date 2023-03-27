ifeq ($(ARCH), CYGWIN)
	PWD_PATH = $(shell cygpath -w $(CURDIR))
else
	PWD_PATH = $(CURDIR)
endif

SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SRCS))
INCLUDE_PARM = $(foreach path, $(INCLUDE_PATH), -I$(path))

OBJ_PATH := $(OBJ_PATH)/kernel
PATH_CHECK = $(OBJ_PATH)

all: $(PATH_CHECK) $(OBJS)

$(PATH_CHECK):
	mkdir -p $@

%.o: %.cc
	$(CC) $(CXXFLAGS) $(INCLUDE_PARM) $< -o $(OBJ_PATH)/$@
