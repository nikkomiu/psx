# If you change this to exe, you'll have to rename the file ./thirdparty/nugget/ps-exe.ld too.
TYPE = ps-exe

NUGGETLIB := /usr/local/nugget
PSYQLIB := /usr/local/psyq

THISDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SRCS += $(NUGGETLIB)/common/crt0/crt0.s
SRCS += $(NUGGETLIB)/common/syscalls/printf.s 

CPPFLAGS += -I$(PSYQLIB)/include 
LDFLAGS += -L$(NUGGETLIB)/psyq/lib -L$(PSYQLIB)/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -lcd
LDFLAGS += -Wl,--end-group


include $(NUGGETLIB)/common.mk

define OBJCOPYME
$(PREFIX)-objcopy -I binary --set-section-alignment .data=4 --rename-section .data=.rodata,alloc,load,readonly,data,contents -O $(FORMAT) -B mips $< $@
endef

# convert TIM file to bin
%.o: %.tim
	$(call OBJCOPYME)

# convert VAG files to bin
%.o: %.vag
	$(call OBJCOPYME)
	
# convert HIT to bin
%.o: %.HIT
	$(call OBJCOPYME)