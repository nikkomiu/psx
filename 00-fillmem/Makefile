fillmem.ps-exe: fillmem.bin
	python3 ../scripts/bin2exe.py fillmem.bin fillmem.ps-exe
	rm fillmem.bin

fillmem.bin:
	armips fillmem.s

clean:
	-rm fillmem.bin fillmem.ps-exe
