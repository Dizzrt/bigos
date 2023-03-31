SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SRCS))
INCLUDE_PARM = $(foreach path, $(INCLUDE_PATH), -I$(path))

OBJ_PATH := $(OBJ_PATH)/drivers
PATH_CHECK = $(OBJ_PATH)

export INCLUDE_PARM OBJ_PATH

all: $(PATH_CHECK) $(OBJS)
	make -C video
	
$(PATH_CHECK):
	mkdir -p $@

%.o: %.cc
	$(CC) $(CXXFLAGS) $(INCLUDE_PARM) $< -o $(OBJ_PATH)/$@
