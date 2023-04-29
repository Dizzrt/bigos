SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SRCS))

all: $(OBJS)

$(PATH_CHECK):
	mkdir -p $@

%.o: %.cc
	$(CC) $(CXXFLAGS) $(INCLUDE_PARM) $< -o $(OBJ_PATH)/$@
