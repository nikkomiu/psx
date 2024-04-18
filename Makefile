all: fillmem mips-warmup factorial graphics gpu

fillmem:
	$(MAKE) -C 00-fillmem

mips-warmup:
	$(MAKE) -C 01-mips-warmup

factorial:
	$(MAKE) -C 02-factorial

graphics:
	$(MAKE) -C 03-graphics

gpu:
	$(MAKE) -C 04-gpu

clean:
	$(MAKE) -C 00-fillmem clean
	$(MAKE) -C 01-mips-warmup clean
	$(MAKE) -C 02-factorial clean
	$(MAKE) -C 03-graphics clean
	$(MAKE) -C 04-gpu clean
