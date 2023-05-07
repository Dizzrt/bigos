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

.PHONY:run
run:
	@$(BOCHS_RUN) -f $(BOCHSRC_PATH) -q
