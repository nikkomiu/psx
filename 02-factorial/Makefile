all: factorial.ps-exe factorialsub.ps-exe

factorial.ps-exe: factorial.bin
	python3 ../scripts/bin2exe.py factorial.bin factorial.ps-exe
	rm factorial.bin

factorial.bin:
	armips factorial.s

factorialsub.ps-exe: factorialsub.bin
	python3 ../scripts/bin2exe.py factorialsub.bin factorialsub.ps-exe
	rm factorialsub.bin

factorialsub.bin:
	armips factorialsub.s

clean:
	-rm factorial.bin factorial.ps-exe factorialsub.bin factorialsub.ps-exe
