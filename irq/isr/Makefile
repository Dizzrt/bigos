SRCS = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SRCS))

all: $(OBJS) isr.s.o

$(PATH_CHECK):
	mkdir -p $@

%.o: %.cc
	$(CC) $(CXXFLAGS) $(INCLUDE_PARM) $< -o $(OBJ_PATH)/$@

isr.s.o: isr.s
	$(CC) $(CXXFLAGS) $(INCLUDE_PARM) $< -o $(OBJ_PATH)/$@
