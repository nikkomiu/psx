ASMCC := armips
TOEXE := python3 ../scripts/bin2exe.py

THISDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

%.ps-exe: %.bin
	$(TOEXE) $*.bin $*.ps-exe

%.bin:
	$(ASMCC) $*.s

%.bin:
	$(ASMCC) $*.s

clean:
	@-rm *.bin *.ps-exe
